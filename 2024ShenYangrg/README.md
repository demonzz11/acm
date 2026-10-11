# 2024 ICPC 沈阳区域赛 · 全场题解与补题路线

题单：[QOJ Contest 1865](https://qoj.ac/contest/1865)。对应 [第三届 Universal Cup，第 19 站：Shenyang](https://ucup.ac/archive/season3/)，训练场日期为 2024 年 11 月 30 日至 12 月 1 日；官方区域赛题解封面日期为 2024 年 11 月 24 日。

本目录覆盖 **A–M 全部 13 题**：中文题意、完整算法推导、正确性解释、复杂度、C++17 参考实现、难度排序与补题建议。参考实现已通过官方静态样例及独立本地验证；**没有登录 QOJ 提交，不把本地通过写成在线 AC**。G 为交互题，附真实双向交互测试器。

你原来的 [B.cpp](B.cpp)、[D.cpp](D.cpp) 保持原样，重写参考实现放在 [solutions/](solutions/)。原调试程序留在本地并由 `.gitignore` 排除。已有源码只能说明接触过这两题，不能据此推定实际赛场成绩。

## 全场索引

| 题号 | 英文题名 | 主要方法 | 中文题解 | C++17 |
| --- | --- | --- | --- | --- |
| A | Safety First | 两种 DP 的根号分治、滚动数组 | [A](editorials/A.md) | [A.cpp](solutions/A.cpp) |
| B | Magical Palette | 零色所在行列、互质、同余构造 | [B](editorials/B.md) | [B.cpp](solutions/B.cpp) |
| C | Crisis Event: Meteorite | 时间倒流、单调区间、三状态覆盖 DP | [C](editorials/C.md) | [C.cpp](solutions/C.cpp) |
| D | Dot Product Game | 相对排列、逆序奇偶、置换环 | [D](editorials/D.md) | [D.cpp](solutions/D.cpp) |
| E | Light Up the Grid | 集合状态、反图、多源查询共用最短路 | [E](editorials/E.md) | [E.cpp](solutions/E.cpp) |
| F | Light Up the Hypercube | 异或群、最小生成树、格雷码、SOS DP | [F](editorials/F.md) | [F.cpp](solutions/F.cpp) |
| G | Guess the Polygon | **交互**、截面积分、模数处理超长分数 | [G](editorials/G.md) | [G.cpp](solutions/G.cpp) |
| H | Guide Map | 固定 DFS 树计数、换根、可持久化线段树 | [H](editorials/H.md) | [H.cpp](solutions/H.cpp) |
| I | Growing Tree | 最深冲突、二分支搜索、后缀日程容量 | [I](editorials/I.md) | [I.cpp](solutions/I.cpp) |
| J | Make Them Believe | 淘汰赛半区最大值 | [J](editorials/J.md) | [J.cpp](solutions/J.cpp) |
| K | Fragile Pinball | 镜像展开、反射顺序枚举、凸多边形裁剪 | [K](editorials/K.md) | [K.cpp](solutions/K.cpp) |
| L | The Grand Contest | 分段线性函数、窗口最大值、二分最短长度 | [L](editorials/L.md) | [L.cpp](solutions/L.cpp) |
| M | Obliviate, Then Reincarnate | 余数图、SCC、势能一致性、反向可达 | [M](editorials/M.md) | [M.cpp](solutions/M.cpp) |

## 难度与补题顺序

官方题解第 2 页给出的预期分档是：

- **Easy**：J。
- **Medium-Easy**：B、D、E、M（+）。
- **Medium**：A（+）、G（−）、H、I。
- **Medium-Hard**：C、F、K、L。

括号中的加减号是官方相对难度标记。它是出题人的预期，不能与现场通过数或个人学习顺序混为一谈。尤其 A 预期为 Medium，但现场仅两队通过。

下面按官方**赛中通过队数**降序排序；相同通过数并列，题号先后不表示实际难度有先后。全场共 320 支队伍（包含打星队伍），表格不是 Universal Cup 训练榜统计。

| 现场排名 | 题号 / 题解 | 现场通过 | 封榜前通过 | 官方预期 | 核心学习内容 |
| --- | --- | ---: | ---: | --- | --- |
| 1 | [J](editorials/J.md) | 320 | 320 | Easy | 读题、签表模拟 |
| 2 | [E](editorials/E.md) | 188 | 169 | Medium-Easy | 把未知初态提升为集合状态 |
| 3 | [D](editorials/D.md) | 180 | 145 | Medium-Easy | 所有终局路径的步数奇偶 |
| 4 | [B](editorials/B.md) | 165 | 120 | Medium-Easy | 从零色推出必要条件 |
| 5 | [M](editorials/M.md) | 94 | 59 | Medium-Easy（+） | 非零权回路与势能 |
| 6 | [G](editorials/G.md) | 47 | 27 | Medium（−） | 分段线性截面与交互 |
| 7 | [I](editorials/I.md) | 19 | 10 | Medium | 局部最深冲突缩小分支 |
| 8 | [H](editorials/H.md) | 16 | 8 | Medium | DFS 树计数与换根拆贡献 |
| 9（并列） | [A](editorials/A.md) | 2 | 2 | Medium（+） | 两种慢 DP 的互补优化 |
| 9（并列） | [F](editorials/F.md) | 2 | 2 | Medium-Hard | 线性空间上的最小生成树 |
| 11（并列） | [C](editorials/C.md) | 1 | 0 | Medium-Hard | 反向状态单调性与多角色合并 |
| 11（并列） | [L](editorials/L.md) | 1 | 1 | Medium-Hard | 连续时间函数与整数最优区间 |
| 13 | [K](editorials/K.md) | 0 | 0 | Medium-Hard | 有限候选轨迹与数值几何 |

通过数来源是官方题解各题首张幻灯片：J 5、E 7、D 9、B 12、M 15、G 18、I 21、H 23、A 26、F 30、L 36、C 39、K 43 页。

**建议学习顺序：J → D → B → E → M → G → I → H → A → F → L → C → K。** D、B 先复盘已有代码，再将新增题目的主要精力放在 E、M；G 提前练交互。后五题作为专题，不按代码长度判断难度。详细时间安排和赛场取舍见 [TRAINING.md](TRAINING.md)。

## 使用与可复现验证

```bash
g++ -std=c++17 -O2 -Wall -Wextra solutions/E.cpp -o E
python verify.py
python verify.py --stress --original
python verify.py --only CGL
```

需要 Python 3.10+ 和 PATH 中的 g++。脚本在系统临时目录编译，测试后清理，适用于 Windows 和 Linux。`--only` 只缩小独立随机检查范围，仍统一编译全部代码、验证官方静态样例；G 由脚本交互驱动。`--original` 额外检查保留的 B、D。

验证范围、运行记录与限制见 [VALIDATION.md](VALIDATION.md)。完整本地输出保存在 [sources/validation.log](sources/validation.log)。GitHub 公式、表格和链接检查结果见 [DISPLAY.md](DISPLAY.md)。

## 目录

```text
2024ShenYangrg/
├── README.md             # 全场索引、官方分档与通过数排序
├── TRAINING.md           # 针对已有 B、D 代码的补题路线
├── VALIDATION.md         # 独立校验方式、边界和验证限制
├── DISPLAY.md            # GitHub 显示检查记录
├── B.cpp / D.cpp         # 原有代码，保持不变
├── editorials/           # A–M 中文题解
├── solutions/            # A–M C++17 参考实现
├── tests/                # 官方样例、陨石原规则模拟器
├── sources/              # 官方资料、提取文本、验证日志
└── verify.py             # 统一编译、对拍、交互及压力检查
```

## 来源与格式约定

- [QOJ 1865](https://qoj.ac/contest/1865)：本次直接读取遇到网页验证，未登录提交。
- [Universal Cup 第三届官方归档](https://ucup.ac/archive/season3/)：第 19 站 Shenyang 的官方资料。
- [官方英文题面](https://ucup.ac/statements/statements-3-19.pdf)，[本地档案](sources/statements-en.pdf)。
- [官方中文题解](https://ucup.ac/tutorials/tutorials-3-19-zh.pdf)，[本地档案](sources/editorial-zh.pdf)，原作者署名“华为云计算与网络创新 Lab”。

中文推导依据官方思路展开，参考代码独立编写。C 的第一轮区间覆盖、L 的长度上限滑窗等具体实现解释见各题，不把它们冒称官方原程序。

正文使用标准 GitHub Markdown。行内公式采用美元符号包裹反引号代码的形式，独立公式使用 `math` 围栏；比较符写成 `\lt`、`\gt`，自定义函数名用 `\mathrm{...}`，避免仓库此前遇到的 HTML 转义和浏览器不支持命令问题。所有代码和样例都带语言标记，站内路径用相对链接。
