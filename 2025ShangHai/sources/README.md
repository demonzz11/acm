# 官方来源与文本提取

| 文件 | 内容 | 公开来源 |
| --- | --- | --- |
| [statements.pdf](statements.pdf) | A–M 英文题面，21 页 | [Universal Cup PDF](https://ucup.ac/statements/statements-4-12.pdf) |
| [statements-en.txt](statements-en.txt) | 题面 PDF 的逐页提取文本 | 同上 |
| [editorial-zh.pdf](editorial-zh.pdf) | 官方中文题解，29 页 | [Universal Cup PDF](https://ucup.ac/tutorials/tutorials-4-12-zh.pdf) |
| [editorial-zh.txt](editorial-zh.txt) | 修复中文映射后的逐页提取文本 | 同上 |

上海站与 [QOJ Contest 2908](https://qoj.ac/contest/2908?v=1) 的对应关系经 [Universal Cup 第四届归档](https://ucup.ac/archive/season4/) 核对。QOJ 直接请求返回站点验证页面，因此题意、输入输出协议、约束与难度排序使用公开官方 PDF 核对，没有进行账号登录或在线提交。

题面封面给出 Universal Cup 场次日期 2026 年 1 月 10–11 日，题面正文页眉给出 2025 年 11 月 29 日；题解封面给出 2025 年 11 月 26 日。保留原始文件以便核对。

文本使用 `pypdf` 提取。题解中文字体没有 `ToUnicode` 映射，直接提取会乱码；整理时根据字体声明的 `Adobe-GB1` 字符集补入 [Adobe 官方映射](https://github.com/adobe-type-tools/mapping-resources-pdf/blob/master/pdf2unicode/Adobe-GB1-UCS2)，再提取中文。字体映射数据仅用于临时提取，不是本目录代码依赖。

PDF 中公式的上下标、排版和图片不能被纯文本完整恢复，例如 M 的样例配图。涉及公式歧义时以 PDF 视觉内容为准；中文题解中的公式已按含义重新排版，不直接照抄错位提取结果。

官方题解的署名为清华大学。本目录保留来源与署名，不宣称这些 PDF 或其中的算法结论为原创。`../std/` 是依据结论独立实现的参考程序。
