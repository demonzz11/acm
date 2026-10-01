Title: 一月的颜色 - Problem - QOJ.ac

URL Source: https://qoj.ac/problem/14690/statement/zh_cn

Markdown Content:
给定一个以节点 1 为根的 $n$ 个节点的有根树。保证树中没有节点恰好有一个子节点。换句话说，每个节点要么是叶子节点，要么至少有两个子节点。

你拥有树中的一些节点。

你手中可能有一些节点。你可以通过以下两种方式获得新节点：

*   直接以 $c_{i}$ 的代价从银行购买节点 $i$。
*   从手中选择两个具有相同父节点的**不同**节点，将它们丢弃，并免费获得它们的父节点。

现在，你有 $m$ 次询问。在每次询问中，你最初手中恰好有节点 $x$，你希望最终手中恰好只有节点 $y$，且不剩下任何其他节点。求实现这一目标所需的最小代价，如果无解则报告无解。

### 输入格式

输入包含多组测试数据。第一行包含一个整数 $t$ ($1 \leq t \leq 10^{5}$)，表示测试数据的组数。对于每组测试数据：

*   第一行包含两个整数 $n$ 和 $m$ ($3 \leq n \leq 3 \cdot 10^{5}$, $1 \leq m \leq 3 \cdot 10^{5}$)，其中 $n$ 是树中节点的数量，$m$ 是询问的数量。
*   第二行包含 $n$ 个整数 $c_{1} , c_{2} , \ldots , c_{n}$ ($1 \leq c_{i} \leq 10^{9}$)，表示从银行购买每个节点的代价。
*   接下来的 $n - 1$ 行，每行包含两个整数 $u$ 和 $v$ ($1 \leq u , v \leq n$, $u \neq v$)，表示树中的一条边。
*   接下来的 $m$ 行，每行包含两个整数 $x$ 和 $y$ ($1 \leq x , y \leq n$, $x \neq y$)，表示一次询问。

保证所有测试数据的 $n$ 之和以及 $m$ 之和均不超过 $3 \cdot 10^{5}$。

### 输出格式

对于每次询问，输出一个整数，表示该次询问的最小额外代价。如果无法最终只剩下节点 $y$，则输出 $- 1$。

### 样例

#### 输入样例 1

3
5 5
1 2 3 4 5
1 2
1 3
2 4
2 5
3 1
2 1
4 1
5 1
5 2
5 5
1 5 1 1 1
1 2
1 3
2 4
2 5
3 1
2 1
4 1
5 1
2 5
6 5
9 9 8 2 4 4
1 2
1 3
1 4
1 5
1 6
2 1
3 1
4 1
5 1
6 1
#### 输出样例 1

2
3
8
7
4
2
1
2
2
-1
2
2
4
2
2
### 说明

对于第一组测试数据的第一次询问，你最初拥有节点 3，并希望获得节点 1。最便宜的方案是直接以 $c_{2} = 2$ 的代价购买节点 2，然后丢弃节点 2 和 3，免费获得它们的共同父节点 1。可以证明这是代价最小的方案。

对于第二组测试数据的第一次询问，你同样从节点 3 开始，并希望获得节点 1。你没有购买节点 2，而是选择以总代价 $c_{4} + c_{5} = 2$ 购买节点 4 和 5，然后丢弃节点 4 和 5，免费获得它们的父节点 2。接着，丢弃节点 2 和 3，免费获得它们的父节点 1。可以证明这是代价最小的方案。

注意，你不能再购买一个节点 3，然后将它与原本的节点 3 一起丢弃来获得节点 1，因为这两个节点 3 不满足“两个不同节点”的要求。

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
