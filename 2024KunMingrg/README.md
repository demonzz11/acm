# 2024 ICPC 亚洲区域赛（昆明）题解

比赛：[QOJ 1871](https://qoj.ac/contest/1871?v=1)，区域赛日期为 2024 年 12 月 1 日。对应训练场次为 [第三届 Universal Cup，第 20 站：Kunming](https://ucup.ac/archive/season3/)，训练日期为 2024 年 12 月 7–8 日。

本目录收录正式赛 **A–M 共 13 题**，每题提供中文题解、算法推导、正确性证明、复杂度与可独立提交的 C++17 参考实现。目录中已有的 C、G、J 源码另存为快照，并进行审查；全场题意、约束和样例按官方资料核对。

## 全场题目索引

| 题号 | 英文题名 | 中文题名 | 题解 | C++17 | 主要方法 / 状态 |
| --- | --- | --- | --- | --- | --- |
| A | Antivirus | 防毒 | [A](editorials/A.md) | [A.cpp](solutions/A.cpp) | 支配树、树链剖分、特殊懒标记 |
| B | Brackets | 括号 | [B](editorials/B.md) | [B.cpp](solutions/B.cpp) | 括号栈、删除并查集、确定性倍增排名 |
| C | Coin | 金币 | [C](editorials/C.md) | [C.cpp](solutions/C.cpp) | 逆推位置、整除分块 |
| D | Dolls | 套娃 | [D](editorials/D.md) | [D.cpp](solutions/D.cpp) | 栈判定、最长前缀贪心、倍增二分 |
| E | Extracting Weights | 提取权值 | [E](editorials/E.md) | [E.cpp](solutions/E.cpp) | **交互题**，路径异或、GF(2) 消元 |
| F | Flowers | 花 | [F](editorials/F.md) | [F.cpp](solutions/F.cpp) | 不同质因子计数、Min_25、DFS |
| G | GCD | 最大公因数 | [G](editorials/G.md) | [G.cpp](solutions/G.cpp) | 公因数归一化、迭代加深、失败状态缓存 |
| H | Horizon Scanning | 扫描地平线 | [H](editorials/H.md) | [H.cpp](solutions/H.cpp) | 极角排序、环上最大间隔 |
| I | Items | 物品 | [I](editorials/I.md) | [I.cpp](solutions/I.cpp) | 移位生成函数、截断布尔卷积、NTT |
| J | Just another Sorting Problem | 又一个排序问题 | [J](editorials/J.md) | [J.cpp](solutions/J.cpp) | 排列博弈、不变量 |
| K | Key Recovery | 密钥恢复 | [K](editorials/K.md) | [K.cpp](solutions/K.cpp) | **交互题**，积分攻击、候选密钥验证 |
| L | Last Chance: Threads of Despair | 绝望线缕 | [L](editorials/L.md) | [L.cpp](solutions/L.cpp) | 攻击预算、爆炸闭包、排序贪心 |
| M | Matrix Construction | 矩阵构造 | [M](editorials/M.md) | [M.cpp](solutions/M.cpp) | 副对角线构造 |

英文题名来自官方题面，中文题名采用官方中文题解。

## 难度与补题顺序

按官方题解记录的**区域赛现场通过队数**降序排列，作为由易到难的补题参考。通过队数相同的题并列；官方难度分档见题解第 2 页。现场通过数同时受知识储备、读题顺序和比赛时间影响，因此应结合分档与题目方法选择。

| 排名 | 题号 / 题面 | 题名 / 题解 | 官方分档 | 现场通过队数 | 核心方法 | 题解与参考代码 |
| --- | --- | --- | --- | ---: | --- | --- |
| 1 | [M](https://ucup.ac/statements/statements-3-20.pdf#page=25) | [矩阵构造](https://ucup.ac/tutorials/tutorials-3-20-zh.pdf#page=4) | 简单 | 449 | 按副对角线构造 | [题解](editorials/M.md) · [M.cpp](solutions/M.cpp) |
| 2 | [J](https://ucup.ac/statements/statements-3-20.pdf#page=19) | [又一个排序问题](https://ucup.ac/tutorials/tutorials-3-20-zh.pdf#page=8) | 简单 | 448 | 排列博弈；统计未归位位置 | [题解](editorials/J.md) · [J.cpp](solutions/J.cpp) |
| 3 | [H](https://ucup.ac/statements/statements-3-20.pdf#page=16) | [扫描地平线](https://ucup.ac/tutorials/tutorials-3-20-zh.pdf#page=13) | 简单 | 333 | 极角排序；循环角度差 | [题解](editorials/H.md) · [H.cpp](solutions/H.cpp) |
| 4 | [L](https://ucup.ac/statements/statements-3-20.pdf#page=23) | [绝望线缕](https://ucup.ac/tutorials/tutorials-3-20-zh.pdf#page=16) | 简单 | 196 | 排序；贪心分配攻击次数 | [题解](editorials/L.md) · [L.cpp](solutions/L.cpp) |
| 5 | [G](https://ucup.ac/statements/statements-3-20.pdf#page=15) | [最大公因数](https://ucup.ac/tutorials/tutorials-3-20-zh.pdf#page=27) | 中等 | 144 | 最优步数上界；搜索 | [题解](editorials/G.md) · [G.cpp](solutions/G.cpp) |
| 6 | [C](https://ucup.ac/statements/statements-3-20.pdf#page=7) | [金币](https://ucup.ac/tutorials/tutorials-3-20-zh.pdf#page=21) | 中等 | 138 | 逆推胜者位置；按商分段跳跃 | [题解](editorials/C.md) · [C.cpp](solutions/C.cpp) |
| 7 | [E](https://ucup.ac/statements/statements-3-20.pdf#page=11) | [提取权值](https://ucup.ac/tutorials/tutorials-3-20-zh.pdf#page=32) | 中等 | 49 | 交互；异或方程组；线性无关基 | [题解](editorials/E.md) · [E.cpp](solutions/E.cpp) |
| 8 | [F](https://ucup.ac/statements/statements-3-20.pdf#page=13) | [花](https://ucup.ac/tutorials/tutorials-3-20-zh.pdf#page=42) | 困难 | 9 | 不同质因子计数；Min_25 筛；搜索 | [题解](editorials/F.md) · [F.cpp](solutions/F.cpp) |
| 9 | [B](https://ucup.ac/statements/statements-3-20.pdf#page=5) | [括号](https://ucup.ac/tutorials/tutorials-3-20-zh.pdf#page=54) | 困难 | 7 | 括号栈；残余匹配；倍增排名 | [题解](editorials/B.md) · [B.cpp](solutions/B.cpp) |
| 10（并列） | [A](https://ucup.ac/statements/statements-3-20.pdf#page=2) | [防毒](https://ucup.ac/tutorials/tutorials-3-20-zh.pdf#page=48) | 困难 | 3 | 支配树；树链剖分；线段树优化 DP | [题解](editorials/A.md) · [A.cpp](solutions/A.cpp) |
| 10（并列） | [D](https://ucup.ac/statements/statements-3-20.pdf#page=9) | [套娃](https://ucup.ac/tutorials/tutorials-3-20-zh.pdf#page=35) | 中等 | 3 | 贪心合并；倍增与二分区间 | [题解](editorials/D.md) · [D.cpp](solutions/D.cpp) |
| 12 | [I](https://ucup.ac/statements/statements-3-20.pdf#page=18) | [物品](https://ucup.ac/tutorials/tutorials-3-20-zh.pdf#page=59) | 困难 | 1 | 生成函数；截断多项式倍增；FFT / NTT | [题解](editorials/I.md) · [I.cpp](solutions/I.cpp) |
| 13 | [K](https://ucup.ac/statements/statements-3-20.pdf#page=20) | [密钥恢复](https://ucup.ac/tutorials/tutorials-3-20-zh.pdf#page=64) | 困难 | 0 | 交互；分组构造询问；枚举与过滤密钥 | [题解](editorials/K.md) · [K.cpp](solutions/K.cpp) |

建议先按分档补题，同档内参考现场通过数：

- 简单：**M → J → H → L**。
- 中等：**G → C → E → D**。D 虽被官方列为中等，现场仅 3 队通过，建议留到本档最后。
- 困难：**F → B → A → I → K**。先复习对应算法，再独立尝试题目。

如果已接触 C、G、J，可先独立重做，再从 M、H、L 开始补其余题目。表中的代码链接指向 `solutions/` 参考实现，在线 AC 状态以自己的评测记录为准。

通过数来源：官方题解中各题“现场情况”页，分别为 M 4、J 8、H 13、L 16、G 27、C 21、E 32、F 42、B 54、A 48、D 35、I 59、K 64 页；分档来源为第 2 页。资料核对日期：2026 年 10 月 7 日。

## 原代码审查

- **C**：在官方样例、小规模穷举和大数独立对拍中均正确。参考实现把原来的分支合并为统一的整除分块，便于证明和复核。
- **G**：保存快照中的质因子跳转搜索漏掉了合法路径。输入 `11 86`，该版本输出 `6`，正确答案为 `5`。参考实现保留每一步的两种操作，用归一化、失败状态缓存与常数时间 GCD 查询加速。
- **J**：在合法输入中判定正确。参考实现去掉了初始已升序的不可达分支，并补全 Bob 能持续阻止排序的证明。

G 的五步最优方案为：

```text
(11, 86) -> (11, 85) -> (10, 85) -> (5, 85) -> (0, 85) -> (0, 0)
```

[original/](original/) 保存整理时的原代码快照，供比较使用；提交评测请使用 [solutions/](solutions/) 中的参考实现。根目录的 [C.cpp](C.cpp)、[G.cpp](G.cpp)、[J.cpp](J.cpp) 另保留本次修改前的本地实现。

## 目录与使用

```text
2024KunMingrg/
├── README.md
├── VALIDATION.md
├── editorials/         # 中文题解、推导、证明、复杂度
├── solutions/          # 可独立提交的 C++17 实现
├── original/           # 原代码快照
├── sources/            # 官方题面、中文题解与提取文本
├── tests/              # 各题独立暴力、结构检查与交互模拟器
└── verify.py           # 可复现的样例、暴力对拍和边界检查
```

单题编译与运行：

```bash
g++ -std=c++17 -O2 solutions/C.cpp -o C
./C
```

在 Windows 上可将输出名称改成 `C.exe`，然后运行 `./C.exe`。

验证全部 13 题：

```bash
python verify.py
python verify.py --stress
```

验证需要 Python 3.10+ 和 PATH 中的 `g++`，不需要第三方 Python 包。脚本在系统临时目录编译，结束后清理，不在仓库中生成可执行文件。`--stress` 增加最大规模输入，并将 J 的排列博弈穷举扩展到长度 8。

E、K 必须由交互器提供回复，不能直接把静态样例重定向后当成普通题运行。验证脚本会分别启动本地交互模拟器，检查格式、询问次数、刷新输出和最终答案。K 使用官方积分攻击的候选筛选思路，随机批次用于排除伪候选，输出前再以完整加密过程核验；概率边界见其题解。

## 来源与验证

官方资料见 [sources/README.md](sources/README.md)。算法结论参考官方中文题解，证明与代码在此展开、独立实现；官方材料保留原署名。

本地验证覆盖官方样例、独立暴力对拍、原 G 反例及合法最大规模输入。具体结果见 [VALIDATION.md](VALIDATION.md)。参考实现尚未取得在线评测 AC 记录。

文档使用 GitHub Markdown、相对链接和 GitHub 支持的数学公式，可连同本目录直接提交到代码仓库。行内公式统一采用美元符号加反引号写法，避免中文标点和下划线影响 GitHub 识别；独立公式采用 `math` 围栏，保留花括号转义与换行；行内比较采用 `\lt`、`\gt`，避免 HTML 实体影响公式显示。
