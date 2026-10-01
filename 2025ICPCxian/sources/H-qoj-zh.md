Title: 黑暗之心 - Problem - QOJ.ac

URL Source: https://qoj.ac/problem/14688/statement/zh_cn

Markdown Content:
对于一棵树 T，定义 v(T) 为将 T 的顶点染成黑色和白色的方案数，且满足以下条件：

*   对于 T 中的所有黑色顶点 u,v，从 u 到 v 的简单路径上的所有顶点也都是黑色。
*   存在至少 k 条无向边 (u,v)，满足 u 和 v 颜色不同。

对于所有含有 n 个顶点的带标号无根树 T，计算 v(T) 的总和模 998244353 的值。

### 输入格式

仅一行，包含两个正整数 n,k (1≤n≤10 7, 1≤k≤5000)。

### 输出格式

输出一个整数，表示答案。

### 样例

#### 输入样例 1

3 1
#### 输出样例 1

15
#### 输入样例 2

6 2
#### 输出样例 2

17286
#### 输入样例 3

30 9
#### 输出样例 3

434031055
#### 输入样例 4

114514 2520
#### 输出样例 4

136362204
### 说明

对于第一个样例，仅有 3 种不同的树 T，它们都是链，因此它们具有相同的 v(T)。用 0 表示黑色，1 表示白色，共有 5 种方案：(0,0,1)，(1,0,0)，(1,1,0)，(0,1,1)，(1,0,1)。我们强调，方案 (1,1,1) 和 (0,0,0) 不满足第二个条件，而 (0,1,0) 不满足第一个条件。

因此，答案为 3⋅5=15。

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
