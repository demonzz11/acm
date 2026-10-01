Title: 幻想冬青 - Problem - QOJ.ac

URL Source: https://qoj.ac/problem/14689/statement/zh_cn

Markdown Content:
给定一个大小为 $n \times n$ 的非负整数矩阵 $A$。对于一棵包含 $n$ 个节点（编号为 $1$ 到 $n$）的树，当且仅当满足以下条件时，我们称其为一棵 **holly 树**：

*   对于任意一对节点 $u$ 和 $v$（$1 \leq u , v \leq n$），$A_{u , v}$ 等于该树中从 $u$ 到 $v$ 的简单路径上所有节点编号的按位异或和。

你的任务是构造一棵 holly 树。保证这样的 holly 树总是存在。

### 输入格式

输入的第一行包含一个整数 $n$（$2 \leq n \leq 2000$），表示矩阵 $A$ 的大小。

接下来的 $n$ 行描述矩阵 $A$。第 $i$ 行包含 $n - i + 1$ 个整数 $A_{i , i} , A_{i , i + 1} , \ldots , A_{i , n}$（$0 \leq A_{i , j} < 2^{11}$）。注意，对于所有 $1 \leq i , j \leq n$，均满足 $A_{i , j} = A_{j , i}$。

保证这样的 holly 树总是存在。

### 输出格式

输出 $n - 1$ 行，每行包含两个整数 $u$ 和 $v$（$1 \leq u , v \leq n$），表示 holly 树的一条边。

### 样例

#### 输入 1

2
1 3
2
#### 输出 1

1 2
#### 输入 2

4
1 3 2 5
2 0 7
3 6
4
#### 输出 2

1 2
1 3
1 4
#### 输入 3

6
1 7 4 5 2 3
2 1 6 7 0
3 5 4 3
4 3 2
5 5
6
#### 输出 3

4 1
2 3
6 4
5 2
4 2
### 说明

在第二个样例中，输出的树如下图所示：

![Image 1](https://qoj.ac/problem/14689/statement/IMAGE_PLACEHOLDER)
这棵树是一棵 holly 树。例如，对于节点对 $\left(\right. 2 , 4 \left.\right)$，从 $2$ 到 $4$ 的简单路径包含节点 $2$、$1$ 和 $4$，其按位异或和为 $2 \oplus 1 \oplus 4 = 7$，满足输入中给出的约束条件 $A_{2 , 4} = 7$。

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
