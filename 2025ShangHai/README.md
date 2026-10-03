# 2025 ICPC 亚洲区域赛上海站 · 补题笔记

题单：[QOJ Contest 2908](https://qoj.ac/contest/2908?v=1)。对应 [The 4th Universal Cup, Stage 12: Shanghai](https://ucup.ac/archive/season4/)，Universal Cup 场次日期为 2026 年 1 月 10–11 日。题面 PDF 页眉标注原场日期为 2025 年 11 月 29 日；官方题解封面日期为 2025 年 11 月 26 日，二者用途不同。

本目录覆盖 A–M 全部 13 题，提供中文题意摘要、推导、正确性解释、复杂度、切入点和独立编写的 C++17 参考实现。`std` 表示本目录参考代码，不代表出题人原始标准程序或已在线 AC；本地验证范围见 [VALIDATION.md](VALIDATION.md)。A 为交互题，不能直接把静态样例当普通输入运行。

**如果这场只做出两题：先补 D，再复盘 G、H 的证明，然后在 B、J 中选择一题冲击第三题。** 完整训练与临场建议见 [TRAINING.md](TRAINING.md)。目录里的原始 G、H 源码并不能证明这两题的提交结果，本文只据此安排复盘重点。

## 目录

```text
2025ShangHai/
├── README.md          # 索引、难度、使用方法
├── TRAINING.md        # 从两题向三至四题进步的补题和临场计划
├── VALIDATION.md      # 本地验证结果与限制
├── original/          # 原 G.cpp、H.cpp、调试程序及 SHA-256 清单
├── solutions/         # A–M 中文题解
├── std/               # A–M 独立 C++17 参考实现
├── sources/           # 官方题面、题解 PDF 和提取文本
└── verify.py          # 编译、样例、独立暴力、交互模拟及压力检查
```

## 难度与补题顺序

官方题解第 2 页给出：

- **预期难度**：`D < G H < B I J < A E < K L M < C F`。
- **实际难度**：`D H < A G J < B I < E K < F L M < C`。

同一档没有严格先后。这是官方题解中的定性排序，不是本文统计通过人数得到的排名，也不等同于 Codeforces 分数。

下面的“建议顺序”针对已经接触 G、H、临近区域赛且目前大约能做两题的选手，综合学习收益、实现成本和时间预算；因此 A 虽然官方实际难度较低，仍因交互训练成本放在 B、J 后。

| 建议顺序 | 题号与题名 | 官方实际档位 | 核心方法 | 题解 | C++17 |
| --- | --- | --- | --- | --- | --- |
| 1 | D · Not a subset sum | 1 | 三进制状态、递归合并 | [D](solutions/D.md) | [D.cpp](std/D.cpp) |
| 2 | H · AGI | 1 | 相同元素配对、策略模仿 | [H](solutions/H.md) | [H.cpp](std/H.cpp) |
| 3 | G · Gemcrate | 2 | 三组合并、线性基 | [G](solutions/G.md) | [G.cpp](std/G.cpp) |
| 4 | B · Hamu | 3 | 连通性、奇环、DFS 奇偶修复 | [B](solutions/B.md) | [B.cpp](std/B.cpp) |
| 5 | J · Yet another mailbox problem | 2 | 字典序遍历、惰性兄弟扩展 | [J](solutions/J.md) | [J.cpp](std/J.cpp) |
| 6 | A · Menji, we miss you! | 2 | 交互、深度二分、带权树分割 | [A](solutions/A.md) | [A.cpp](std/A.cpp) |
| 7 | I · Round screws | 3 | 保留点 DP、异或高低位分治 | [I](solutions/I.md) | [I.cpp](std/I.cpp) |
| 8 | E · Flower’s land 3 | 4 | 小汉明距离生成树、增量维护 | [E](solutions/E.md) | [E.cpp](std/E.cpp) |
| 9 | K · No more regrets | 4 | 前缀极值、单侧递归线段树 | [K](solutions/K.md) | [K.cpp](std/K.cpp) |
| 10 | F · Flower’s land 4 | 5 | 双向扫描线、倒数坐标凸包 | [F](solutions/F.md) | [F.cpp](std/F.cpp) |
| 11 | L · Yet another permutation problem | 5 | 最左合法切分、区间 DP 去重 | [L](solutions/L.md) | [L.cpp](std/L.cpp) |
| 12 | M · Yet another 01 problem | 5 | 樱桃节点、容斥、匹配多项式、NTT | [M](solutions/M.md) | [M.cpp](std/M.cpp) |
| 13 | C · Singularity | 6 | 二值化、倍增构造、小规模例外 | [C](solutions/C.md) | [C.cpp](std/C.cpp) |

B 和 J 可以按个人基础交换：图论 DFS 熟练先 B；擅长字典序、模拟和实现优化先 J。近期收益主要集中在前 7 题，后 6 题适合作为长期专题，避免在赛前挤占基础训练时间。

## 编译与验证

```bash
g++ -std=c++17 -O2 -Wall -Wextra std/D.cpp -o D
python verify.py
python verify.py --exhaustive
python verify.py --stress
```

脚本需要 Python 3.10+ 和 PATH 中的 g++，使用固定随机种子，编译产物保存在系统临时目录。可以用 `--only DGH` 选择对拍题目；13 份代码仍会统一编译。A 用本地交互器模拟，B、C 检查输出构造的合法性，其他题检查固定输出与独立暴力。

## 原代码保留与发布

原来的 `G.cpp`、`H.cpp`、`G.debug.exe`、`H.debug.exe` 已按原字节移动到 [original/](original/)，SHA-256 摘要保存在 [original/SHA256.json](original/SHA256.json)。题解中的重写代码放在 `std/`，便于逐行对照。`original/*.exe` 被本目录 `.gitignore` 忽略，仍保留在本地；上传 GitHub 时默认只发布源码和文档。

本文使用标准 Markdown、相对链接和 GitHub 数学公式语法。行内公式使用 `$` 加反引号包裹的形式，独立公式使用 `$$`。题面权威版本见官方 PDF；中文摘要不替代完整输入输出协议。

## 来源

- [QOJ 比赛链接](https://qoj.ac/contest/2908?v=1)。本次直接抓取遇到站点验证，未登录、未提交。
- [Universal Cup 第四届归档](https://ucup.ac/archive/season4/)：确认上海站与 QOJ 2908 对应关系。
- [官方英文题面 PDF](https://ucup.ac/statements/statements-4-12.pdf)，本地副本 [sources/statements.pdf](sources/statements.pdf)。
- [官方中文题解 PDF](https://ucup.ac/tutorials/tutorials-4-12-zh.pdf)，本地副本 [sources/editorial-zh.pdf](sources/editorial-zh.pdf)。

题解依据官方结论展开，具体实现与解释由本目录整理。官方 PDF 保留原作者署名；转载时请保留来源链接。
