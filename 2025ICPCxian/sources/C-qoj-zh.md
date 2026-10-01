Title: 抓住怪物 - Problem - QOJ.ac

URL Source: https://qoj.ac/problem/14683/statement/zh_cn

Markdown Content:
一只史前怪兽通过 Peter 博士的时间旅行机器来到了地球。你需要帮助 Peter 博士抓住它。

怪兽藏在一个有 $n$ 个顶点和 $m$ 条边的森林中。这里，森林是一个无环无向图，可能由多棵树组成。你可以通过执行以下操作来捕捉怪兽：

*   首先，你选择一个顶点 $x$ ($1 \leq x \leq n$)。
*   然后，如果怪兽当前在顶点 $x$，它就被抓住了。
*   否则，怪兽仍未被抓住，在操作之后，它可以移动到与当前顶点相邻的任意顶点（顶点 $x$ 除外）。或者它也可以选择不移动，留在同一个顶点。

我们定义一个森林是**美妙的**，当且仅当存在一个有限的顶点序列 $a$，使得无论怪兽的初始位置如何以及它如何移动，按照 $a$ 的顺序选择顶点执行操作，都能保证怪兽被抓住。

现在，你需要回答 Peter 博士的 $q$ 个问题。在每个问题中，他会给你一个区间 $\left[\right. l , r \left]\right.$ ($1 \leq l \leq r \leq n$)。你需要告诉他，由索引在 $\left[\right. l , r \left]\right.$ 内的顶点导出的子森林（即仅保留这些顶点以及它们之间的边所形成的图）是否是美妙的。

### 输入格式

第一行包含三个整数 $n$、$m$ 和 $q$ ($2 \leq n \leq 10^{6}$，$1 \leq m \leq n - 1$，$1 \leq q \leq 10^{6}$)，其中 $n$ 是森林中的顶点数，$m$ 是森林中的边数，$q$ 是询问次数。

接下来的 $m$ 行，每行包含两个整数 $u$ 和 $v$ ($1 \leq u , v \leq n$，$u \neq v$)，表示森林中的一条边。

接下来的 $q$ 行，每行包含两个整数 $l$ 和 $r$ ($1 \leq l \leq r \leq n$)，表示一次询问。

### 输出格式

对于每次询问，如果该子森林是美妙的，输出一行 "Yes"；否则，输出一行 "No"。

你可以输出任意大小写的答案（大写或小写）。例如，字符串 "yEs"、"yes"、"Yes" 和 "YES" 都会被识别为肯定回答。

### 样例

#### 输入 1

10 9 3
1 2
1 3
1 8
2 5
2 6
2 7
3 4
8 9
8 10
1 3
2 6
1 10
#### 输出 1

Yes
Yes
No
#### 输入 2

100000 1 1
1 2
1 9999
#### 输出 2

Yes
### 说明

在第一个样例中：

*   在第一次询问中，对于子森林 $\left[\right. 1 , 3 \left]\right.$，你可以设置 $a = \left[\right. 3 , 1 , 2 \left]\right.$。
*   在第二次询问中，对于子森林 $\left[\right. 2 , 6 \left]\right.$，你可以设置 $a = \left[\right. 3 , 4 , 5 , 2 , 6 \left]\right.$。
*   在第三次询问中，对于子森林 $\left[\right. 1 , 10 \left]\right.$，可以证明不存在有限的有效序列 $a$。

在第二个样例中：

*   在唯一的一次询问中，对于子森林 $\left[\right. 1 , 9999 \left]\right.$，你可以设置 $a = \left[\right. 1 , 2 , \ldots , 9999 \left]\right.$。

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
