# 2025 ICPC 亚洲区域赛（西安）

比赛日期：2025 年 10 月 19 日。QOJ 比赛编号：[2562](https://qoj.ac/contest/2562)，也收录为 [The 4th Universal Cup, Extra Stage 1: Xi'an](https://ucup.ac/archive/season4/)，Universal Cup 场次日期为 2025 年 11 月 5 日。

本目录收录正式赛 A–M 共 13 题，每题提供完整中文题面、中文题解和独立 C++17 实现。热身题不属于这个比赛的正式题单。

## 题目索引

| 题号 | 题目 | 中文题面 | 题解 | C++17 | 主要方法 |
| --- | --- | --- | --- | --- | --- |
| A | Azalea Garden | [A](statements/A.md) | [A](solutions/A.md) | [A.cpp](A.cpp) | 最大攻击力、动态区间覆盖、线段树 |
| B | Beautiful Dangos | [B](statements/B.md) | [B](solutions/B.md) | [B.cpp](B.cpp) | 双指针、带边界的三色重排 |
| C | Catch the Monster | [C](statements/C.md) | [C](solutions/C.md) | [C.cpp](C.cpp) | 毛毛虫森林、动态度数、双指针 |
| D | Directed Acyclic Graph | [D](statements/D.md) | [D](solutions/D.md) | [D.cpp](D.cpp) | 前后缀可达链、四进制构造 |
| E | Epilogue of Happiness | [E](statements/E.md) | [E](solutions/E.md) | [E.cpp](E.cpp) | 加权树链剖分、分块、分治预处理 |
| F | Follow the Penguins | [F](statements/F.md) | [F](solutions/F.md) | [F.cpp](F.cpp) | 相遇事件最小堆 |
| G | Grand Voting | [G](statements/G.md) | [G](solutions/G.md) | [G.cpp](G.cpp) | 排序贪心 |
| H | Heart of Darkness | [H](statements/H.md) | [H](solutions/H.md) | [H.cpp](H.cpp) | 有根森林计数、Stirling 数、NTT |
| I | Imagined Holly | [I](statements/I.md) | [I](solutions/I.md) | [I.cpp](I.cpp) | 三点路径异或恢复祖先关系 |
| J | January's Color | [J](statements/J.md) | [J](solutions/J.md) | [J.cpp](J.cpp) | 树形 DP、子树区间、路径费用前缀 |
| K | Killing Bits | [K](statements/K.md) | [K](solutions/K.md) | [K.cpp](K.cpp) | 子集格网络流、完美匹配 |
| L | Let's Make a Convex! | [L](statements/L.md) | [L](solutions/L.md) | [L.cpp](L.cpp) | 多边形不等式、排序、双指针 |
| M | Mystique as Iris | [M](statements/M.md) | [M](solutions/M.md) | [M.cpp](M.cpp) | 不可清空数组的补集计数、线性 DP |

## 阅读与发布

文档使用标准 Markdown 标题、列表、表格和代码块。行内公式采用 GitHub 支持的美元符号加反引号写法，例如 ``$`a_i`$``，避免中文标点及下划线影响公式识别；独立公式仍使用单独成行的 `$$` 标记。

公式语法参见 [GitHub 官方文档](https://docs.github.com/en/get-started/writing-on-github/working-with-advanced-formatting/writing-mathematical-expressions)。在 Typora 等本地编辑器中阅读时，请启用内联公式；若编辑器不识别带反引号的写法，可在本地阅读副本中改为 `$...$` 并在公式外保留空格。发布到知乎时，可从编辑器的渲染视图复制正文，再检查公式、图片与代码块；未保留的公式可在知乎公式编辑器中粘贴对应的 LaTeX 源码。

## 来源与整理方式

- 比赛与年份通过 QOJ 公开比赛索引及 Universal Cup 第四届归档交叉确认。
- QOJ 题号为 14681–14693，顺序对应 A–M。
- 中文完整题面来自 QOJ 的 `/problem/<id>/statement/zh_cn`。整理时去掉站点讨论区，为样例补上代码块，保留题目原文。
- 官方题解来源：[Universal Cup 中文题解 PDF](https://ucup.ac/tutorials/tutorials-4-e1-zh.pdf)，其文本版本保存在 [sources/editorial-proxy.md](sources/editorial-proxy.md)。本目录题解基于官方结论展开，实现独立编写。
- [sources/](sources/) 保存中英文原始抓取、比赛身份记录及官方题解文本，便于核对。站点直接访问返回 403 时，经 `r.jina.ai` 读取公开内容；没有进行账号登录或在线提交。

## 编译与验证

单题编译示例：

```bash
g++ -std=c++17 -O2 A.cpp -o A
```

运行完整验证：

```bash
python verify.py
python verify.py --stress
python stress.py
```

需要 Python 3.10+ 和 PATH 中的 g++。所有可执行文件及大规模输入生成在系统临时目录，不写入题目目录。

验证覆盖：13 题均用 C++17、`-O2 -Wall -Wextra` 编译；16 组有固定输出的官方样例；每题独立的小规模暴力或结构检查。B 枚举长度不超过 7 的全部三色串，D 检查全部 16000 个公共可达集合，H 检查全部官方样例；`--stress` 检查 K、H 的最大输入规模，`stress.py` 检查 E 的最大规模链、星形和随机树，并对各自 200 个答案作直接核对。

详细结果见 [VALIDATION.md](VALIDATION.md)。这些是本地验证结果，未获得在线评测的 AC 记录；E 的实际运行时间取决于评测机。
