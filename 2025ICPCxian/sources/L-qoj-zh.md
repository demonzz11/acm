Title: Let's Make a Convex! - Problem

URL Source: https://qoj.ac/problem/14692/statement/zh_cn

Markdown Content:
Kevin 是国际凸多边形锦标赛（ICPC）的主裁判。他为比赛设计了一道几何题。然而，由于缺乏几何方面的经验，他无法为该题生成正确的凸多边形。

为了证明自己的几何水平，Kevin 开始玩起了木棍。他有 $n$ 根木棍，其中第 $i$ 根的长度为 $a_{i}$。他希望选择 $k$ 根木棍，使它们能够拼成一个非退化的凸多边形。

由于需要更强的测试数据，Kevin 想要最大化该多边形的周长（即最大化所选子集中所有木棍长度 $a_{i}$ 的总和）。你能帮他求出对于所有从 $1$ 到 $n$ 的整数 $k$ 的这一最大值吗？如果不存在这样的多边形，则输出单个整数 $0$ 代替。

### 输入格式

输入包含多个测试用例。第一行包含一个整数 $t$（$1 \leq t \leq 10^{5}$），表示测试用例的数量。对于每个测试用例：

*   第一行包含一个整数 $n$（$1 \leq n \leq 2 \cdot 10^{5}$），表示木棍的数量。
*   第二行包含 $n$ 个整数 $a_{1} , \ldots , a_{n}$（$1 \leq a_{i} \leq 10^{9}$），表示每根木棍的长度。

保证所有测试用例中 $n$ 的总和不超过 $2 \cdot 10^{5}$。

### 输出格式

对于每个测试用例，输出一行，包含 $n$ 个整数，表示多边形的最大周长。具体来说，如果对于某个 $k$ 不存在这样的多边形，则在该位置输出单个整数 $0$。

### 样例

#### 样例输入 1

7
5
1 2 3 4 5
5
1 2 4 8 16
5
2 2 2 2 2
4
1 4 10 7
2
1 2
3
2 3 4
4
3 1 2 6
#### 样例输出 1

0 0 12 14 15
0 0 0 0 0
0 0 6 8 10
0 0 21 22
0 0
0 0 9
0 0 0 0
### 说明

在第一个测试用例中，可以证明不存在边数为 1 或 2 的凸多边形。当 $k = 3$ 时，凸多边形的最大周长为 12，因为已知边长为 3、4 和 5 的木棍可以组成一个直角三角形。类似地，当 $k = 4$ 时，多边形的最大周长为 $2 + 3 + 4 + 5 = 14$；当 $k = 5$ 时，选择所有木棍是一个可行方案，得到的周长为 $1 + 2 + 3 + 4 + 5 = 15$。

在第二个测试用例中，可以证明无论如何选择木棍，它们都无法组成凸多边形。

##### About Discussions

**The discussion section is only for posting:** General Discussions (problem-solving strategies, alternative approaches), and Off-topic conversations.

**This is NOT for reporting issues!** If you want to report bugs or errors, please use the **Issues** section below.

##### Open Discussions 0

No discussions in this category.

##### About Issues

If you find any issues with the problem (statement, scoring, time/memory limits, test cases, etc.), you may submit an issue here. A problem moderator will review your issue.

**Guidelines:**

1.   This is not a place to publish discussions, editorials, or requests to debug your code. Issues are only visible to you and problem moderators.
2.   Do not submit duplicated issues.
3.   Issues must be filed in English or Chinese only.

##### Active Issues 0

No issues in this category.

##### Closed/Resolved Issues 0

No issues in this category.
