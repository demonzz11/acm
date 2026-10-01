"""Normalize Markdown for Typora / GitHub / Zhihu rendering.

Two independent passes:

1. Heading hierarchy -- publishable prose gets a sane outline
   (# title / ## section / ### sub-sample), raw scrape artifacts get
   their flat h1 soup demoted below the document title.
2. CJK spacing -- insert a space between CJK characters and adjacent
   ASCII words, numbers, inline code spans, math delimiters and ASCII
   sentence punctuation, matching the style already used in this repo.
   Math/code content is never touched.

Usage:
    python tools/mdformat.py --dry-run
    python tools/mdformat.py --write
"""

from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent

# ---------------------------------------------------------------- spacing

# CJK text in the broad sense, used only for the "does this line need checking"
# fast path.
CJK = r'\u2e80-\u2eff\u3000-\u303f\u3040-\u30ff\u3400-\u4dbf\u4e00-\u9fff\uf900-\ufaff\uff01-\uff5d\uffe0-\uffe6'
CJK_RE = re.compile(f'[{CJK}]')
IDEOGRAPHS = r'\u2e80-\u2eff\u3000-\u303f\u3040-\u30ff\u3400-\u4dbf\u4e00-\u9fff\uf900-\ufaff\uff10-\uff19\uff21-\uff3a\uff41-\uff5a'
IDEO_RE = re.compile(f'[{IDEOGRAPHS}]')

# Brackets and separators already own their whitespace, so a space beside them
# reads as a gap in the wrong place. They also have to stay out of the CJK class
# used for spacing: a fullwidth bracket is itself a CJK character, so leaving it
# in would pair 图 with （ in 图（DAG） and produce the lopsided "图 （DAG）".
BRACKETS = '（）［］｛｝【】〔〕〈〉《》「」『』\u201c\u201d\u2018\u2019\u201b\u201f'
# Separators are split around the bracket characters so each opening mark stays
# its own token: `（` never matches `）`, and the `other` catch-all would glue a
# bracket to whatever follows it.
SEPARATORS = '、。，；：！？…—·' + '）］｝】」』〕〉》”’'
OPENING = set('（［｛【「『〔〈《“‘')
CLOSING = set('。，；：！？）］｝】」』〕〉》”’…')
# Sentence punctuation that Typora and Zhihu want padded away from CJK.
# Deliberately excluded: / \ - _ ( ) [ ] { } | * # : ; " < > + = & @'
ASCII_PUNCT = set(',.!?%')
CODE_MARK, MATH_MARK, EMPHASIS_MARK, TEXT_MARK, OPAQUE_MARK = 'code', 'math', 'em', 'text', 'opaque'
ALNUM = 'A-Za-z0-9'

# One alternation over everything that matters: fenced-style code spans, inline
# math, CJK characters, separators, Latin/number runs, and every other single
# character. Order matters; the backreference in the code span keeps ``a`b`` in
# one piece. `_` is matched alone so an emphasis delimiter can be identified.
TOKEN_RE = re.compile(
    r'(?P<code>`+)(?:(?!`)[^\n])*?(?P=code)'
    r'|(?P<math>\${1,2})(?:(?!\${1,2})[^\n])*?(?P=math)'
    rf'|(?P<cjk>[{IDEOGRAPHS}])'
    rf'|(?P<punct>[{SEPARATORS}])'
    rf'|(?P<latin>[{ALNUM}]+)'
    r'|(?P<other>_|[^`$])'
)
FENCE_RE = re.compile(r'`{3,}|~{3,}')
STAR_RUN_RE = re.compile(r'\*+')
UNDERSCORE_RE = re.compile(r'_(?!\s)([^\n]*?)(?<!\s)_')
_MARKUP = (CODE_MARK, MATH_MARK, EMPHASIS_MARK)


def _is_cjk(ch: str) -> bool:
    return bool(IDEO_RE.fullmatch(ch)) or ch in CLOSING


def _is_latin(ch: str) -> bool:
    return ch.isascii() and (ch.isalnum() or ch in ASCII_PUNCT)


def _padded(prev, nxt) -> bool:
    """Does a space belong between the two chunks either side of this boundary?

    Only two transitions want one: CJK next to Latin, and CJK next to the
    delimiter of a math, code or emphasis token. The asymmetric cases are the
    whole point -- `n、q` stays tight because the gap runs CJK to CJK, while
    `abc个` gains one because it runs Latin to CJK.

    Delimiters that appear in a pair are opaque: the gap between the two `*` of
    `**` is never padded, and neither is the gap between a delimiter and the text
    it wraps.
    """
    if len(prev) < 3 or len(nxt) < 3:
        return False
    lchunk, lkind = prev[0], prev[1]
    rchunk, rkind = nxt[0], nxt[1]
    if lkind == OPAQUE_MARK or rkind == OPAQUE_MARK:
        return False
    left, right = lchunk[-1], rchunk[0]
    # A bracket owns the space on both of its sides or on neither, so a space is
    # never placed against one. That is what keeps （DAG） and （good） symmetrical
    # instead of leaving them padded on a single side.
    if left in BRACKETS or right in BRACKETS:
        return False
    left_cjk, right_cjk = _is_cjk(left), _is_cjk(right)
    # Inside emphasis a delimited run holds together: delimiters bound it but
    # never break it, and CJK against a delimiter still wants its space outside.
    if lkind == EMPHASIS_MARK or rkind == EMPHASIS_MARK:
        delimiter = left if lkind == EMPHASIS_MARK else right
        return (right_cjk if lkind == EMPHASIS_MARK else left_cjk) and delimiter in '*_'
    if lkind in (CODE_MARK, MATH_MARK) or rkind in (CODE_MARK, MATH_MARK):
        delimiter = left if lkind in (CODE_MARK, MATH_MARK) else right
        other_cjk = right_cjk if lkind in (CODE_MARK, MATH_MARK) else left_cjk
        return other_cjk and delimiter in '$`'
    return (left_cjk and _is_latin(right)) or (_is_latin(left) and right_cjk)


def _markup_spans(text: str, tokens) -> set[int]:
    """Indices of chunks that are emphasis delimiters.

    Only the delimiters are opaque; the content between them stays plain text, so
    a space can still be placed against its outer edges. The tempting shortcut --
    treat every `*` as a delimiter -- gets `1*2*3` wrong. Markdown itself is
    lenient here, so this claims only the conservative cases: paired `*` runs, and
    `_` used at word edges.
    """
    # Token index by start offset, so a delimiter found in the text can be mapped
    # back to the chunk that covers it. Runs are tokenized one character at a
    # time, which makes that lookup exact.
    at = {start: i for i, (_, _, start) in enumerate(tokens)}
    marked: set[int] = set()
    for i, (chunk, kind, start) in enumerate(tokens):
        if kind != TEXT_MARK:
            continue
        if chunk == '*':
            run = STAR_RUN_RE.match(text, start)
            length = run.end() - run.start()
            close = text.find('*' * length, run.end()) if run else -1
            # `1*2*3` and `a*b*c` are arithmetic, not emphasis. A doubled run is
            # never multiplication, so only a single one needs the check.
            both_alnum = 0 < start and start + 1 < len(text) and text[start - 1].isascii() \
                and text[start - 1].isalnum() and text[start + 1].isascii() and text[start + 1].isalnum()
            if close < 0 or (length == 1 and both_alnum):
                continue
            # The whole opening and closing runs are delimiters, each side taken
            # as one unit. Marking a single `*` of a `**` would open a gap inside
            # the run and give `** 优秀 **`.
            outside = (text[start - 1] if start else '')
            closing_run = text[close + length] if close + length < len(text) else ''
            if not (outside.isascii() and outside.isalnum()
                    and closing_run.isascii() and closing_run.isalnum()):
                inside = range(run.start(), run.end())
                closing = range(close, close + length)
                for j, (_, _, offset) in enumerate(tokens):
                    if offset in inside or offset in closing:
                        marked.add(j)
        elif chunk == '_':
            before_alnum = start > 0 and text[start - 1].isascii() and text[start - 1].isalnum()
            if not before_alnum and UNDERSCORE_RE.match(text, start):
                marked.add(i)
    return marked



def space_segment(text: str) -> str:
    """Insert the Typora/GitHub/Zhihu CJK-Latin spacing in a single pass.

    Every boundary is decided once from the characters actually adjacent to it,
    so a gap can never come out padded on one side only, and no rule ordering or
    repeated application is involved.
    """
    if not IDEO_RE.search(text):
        return text
    kinds = {'code': CODE_MARK, 'math': MATH_MARK}
    tokens = [(m.group(0), kinds.get(m.lastgroup, TEXT_MARK), m.start())
              for m in TOKEN_RE.finditer(text)]
    for i in _markup_spans(text, tokens):
        chunk, _, start = tokens[i]
        tokens[i] = (chunk, EMPHASIS_MARK, start)
    # Opening marks are their own opaque kind, so nothing pads against them.
    tokens = [(chunk, OPAQUE_MARK if kind == TEXT_MARK and chunk in OPENING else kind, start)
              for chunk, kind, start in tokens]
    out: list[str] = []
    for i, token in enumerate(tokens):
        out.append(token[0])
        if i + 1 < len(tokens) and _padded(token, tokens[i + 1]):
            out.append(' ')
    return ''.join(out)


def collapse(text: str) -> str:
    """Content signature: every non-space character, in order."""
    return ''.join(ch for ch in text if ch != ' ')


CHANGES: dict[str, list[tuple[str, str]]] = {}

# (input, expected) pairs: every spacing rule plus the awkward boundary cases.
CASES: list[tuple[str, str]] = [
    ('有abc个生物', '有 abc 个生物'),
    ('第17个测试点', '第 17 个测试点'),
    ('第i个', '第 i 个'),               # a lone variable still gets its space
    ('是**优秀的**（good）', '是 **优秀的**（good）'),
    ('**整数**划分', '**整数** 划分'),
    ('1*2*3=6', '1*2*3=6'),            # multiplication is not emphasis
    ('第 1*2 项', '第 1*2 项'),
    ('`set`维护', '`set` 维护'),
    ('见`verify.py`。', '见 `verify.py` 。'),
    ('包含恰好$n$个顶点', '包含恰好 $n$ 个顶点'),
    ('$n$只', '$n$ 只'),
    ('（DAG）', '（DAG）'),             # fullwidth brackets own their spacing
    ('图（DAG）中', '图（DAG）中'),
    ('图（DAG）的', '图（DAG）的'),
    ('n、q≤10^6', 'n、q≤10^6'),        # enumeration comma: CJK to CJK, no gap
    ('答案为3⋅5=15。', '答案为 3⋅5=15。'),
    ('a_i、b_i', 'a_i、b_i'),           # underscore is not a separator
    ('a_i,b_i', 'a_i, b_i'),
    ('(u,v)', '(u,v)'),                # ASCII brackets and commas stay tight
    ('n≤7，枚举', 'n≤7，枚举'),
    ('K、H', 'K、H'),
    ('答案为5。见下', '答案为5。见下'),   # CJK to CJK across a closing mark
    ('```a`b```', '```a`b```'),        # code span with an inner backtick
]


def self_test() -> None:
    for src, want in CASES:
        got = space_segment(src)
        if got != want:
            raise AssertionError(f'self-test: {src!r} -> {got!r}, expected {want!r}')


def space_line(line: str, path: str) -> str:
    result = space_segment(line)
    if collapse(result) != collapse(line):
        raise AssertionError(f'{path}: spacing broke character sequence: {line!r}')
    return result


def space_text(text: str, path: str) -> str:
    lines = text.split('\n')
    in_fence = False
    fence = ''
    for i, line in enumerate(lines):
        stripped = line.lstrip()
        m = re.match(r'(`{3,}|~{3,})', stripped)
        if m:
            if not in_fence:
                in_fence, fence = True, m.group(1)[0]
            elif stripped[0] == fence:
                in_fence, fence = False, ''
            continue
        if in_fence:
            continue
        # Blank lines and pure-ASCII pipe-layout rows carry no CJK; skip fast.
        if not CJK_RE.search(line):
            continue
        new = space_line(line, path)
        if new != line:
            lines[i] = new
            CHANGES.setdefault(path, []).append((line, new))
    return '\n'.join(lines)


# ---------------------------------------------------------------- headings


def retitle_statements(lines: list[str], path: str) -> list[str]:
    """statements/*.md : # title / ## section / ### sample case."""
    for i, line in enumerate(lines):
        m = re.match(r'^(#{2,6}) (输入格式|输出格式|样例|说明|提示)\s*$', line)
        if m:
            new = f'## {m.group(2)}'
            if line != new:
                lines[i] = new
                CHANGES.setdefault(path, []).append((line, new))
        m = re.match(r'^(#{3,6}) ((?:输入|输出)(?:格式|样例)? \d+)\s*$', line)
        if m:
            new = f'### {m.group(2)}'
            if line != new:
                lines[i] = new
                CHANGES.setdefault(path, []).append((line, new))
    return lines


EN_SECTIONS = {'Input', 'Output', 'Example', 'Examples', 'Note', 'Interaction', 'Scoring'}
ZH_SECTIONS = re.compile(r'^(输入格式|输出格式|样例|说明|提示)$')
DISCUSSION = re.compile(r'^(About Discussions|Open Discussions|About Issues|Active Issues|Closed/Resolved Issues)')


def retitle_scrape(lines: list[str], path: str) -> list[str]:
    """sources/*-qoj.md and *-qoj-zh.md : keep the problem title at h1."""
    for i, line in enumerate(lines):
        m = re.match(r'^# ([^#].*?)\s*$', line)
        if not m:
            continue
        title = m.group(1).strip()
        if title in EN_SECTIONS or ZH_SECTIONS.match(title):
            new = f'## {title}'
        elif re.match(r'^(输入样例|输出样例|输入|输出|样例输入|样例输出) \d+$', title):
            new = f'### {title}'
        else:
            continue
        if line != new:
            lines[i] = new
            CHANGES.setdefault(path, []).append((line, new))
    # QOJ's discussion boilerplate is appended scrape noise, never an outline.
    for i, line in enumerate(lines):
        m = re.match(r'^#{2,6} (.*)$', line)
        if m and DISCUSSION.match(m.group(1).strip()):
            new = f'#### {m.group(1).strip()}'
            if line != new:
                lines[i] = new
                CHANGES.setdefault(path, []).append((line, new))
    return lines


def retitle_editorial(lines: list[str], path: str) -> list[str]:
    """sources/editorial-proxy.md : one h1 document title, per-problem h2."""
    for i, line in enumerate(lines):
        m = re.match(r'^# ([^#].*)$', line)
        if not m:
            continue
        title = m.group(1).strip()
        if re.match(r'^[A-M]\. ', title):
            new = f'## {title}'
            if line != new:
                lines[i] = new
                CHANGES.setdefault(path, []).append((line, new))
    return lines


def normalize(path: Path, write: bool) -> None:
    text = path.read_text(encoding='utf-8')
    rel = path.relative_to(ROOT).as_posix()
    lines = text.split('\n')

    if path.suffix == '.md' and path.parent.name == 'statements':
        lines = retitle_statements(lines, rel)
    elif path.suffix == '.md' and path.parent.name == 'sources':
        if re.search(r'-qoj(-zh)?\.md$', path.name):
            lines = retitle_scrape(lines, rel)
        elif path.name == 'editorial-proxy.md':
            lines = retitle_editorial(lines, rel)

    text = '\n'.join(lines)
    text = space_text(text, rel)

    if write:
        path.write_text(text, encoding='utf-8')


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument('--write', action='store_true')
    ap.add_argument('--dry-run', action='store_true')
    ap.add_argument('--show', type=int, default=0, help='print up to N changed lines per file')
    args = ap.parse_args()
    if not (args.write or args.dry_run):
        ap.error('pass --dry-run or --write')

    self_test()

    files = sorted(p for p in ROOT.rglob('*.md') if '.git' not in p.parts)
    for path in files:
        normalize(path, write=args.write)

    total = 0
    for rel in sorted(CHANGES):
        edits = CHANGES[rel]
        total += len(edits)
        print(f'{rel}: {len(edits)} line(s)')
        for old, new in edits[: args.show]:
            print(f'  - {old}')
            print(f'  + {new}')
    print(f'\n{len(CHANGES)} file(s), {total} changed line(s)'
          f'{" [written]" if args.write else " [dry-run]"}')
    return 0


if __name__ == '__main__':
    sys.exit(main())
