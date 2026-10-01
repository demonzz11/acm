Title: 大投票 - Problem - QOJ.ac

URL Source: https://qoj.ac/problem/14687/statement/zh_cn

Markdown Content:
Dada 举办了一场比赛，但收到了大量的差评。他决定开始操纵评论。

这场比赛有 $s$ 个投票，初始时设为 0。

有 $n$ 个参与者，每个人都有一个投票参数 $a_{i}$。当轮到他们投票时：

*   如果 $s \geq a_{i}$，他们会投赞成票，使 $s$ 增加 1。
*   如果 $s < a_{i}$，他们会投反对票，使 $s$ 减少 1。

Dada 可以控制这 $n$ 个人的投票顺序。他想知道这场比赛中可能得到的最大和最小投票数 $s$。

### 输入格式

输入的第一行包含一个整数 $n$ ($1 \leq n \leq 10^{5}$)，表示投票者的数量。

第二行包含 $n$ 个整数 $a_{1} , a_{2} , \hdots , a_{n}$ ($\left|\right. a_{i} \left|\right. \leq 10^{5}$)，用空格隔开。

### 输出格式

输出一行，包含两个由空格隔开的整数，分别表示这场比赛中可能得到的最大和最小投票数 $s$。

### 样例

#### 输入样例 1

5
-1 0 1 2 3
#### 输出样例 1

5 -5
### 说明

例如，如果你将 $a$ 重新排列为 $\left[\right. - 1 , 0 , 1 , 2 , 3 \left]\right.$，初始时 $s = 0$。因为 $s \geq a_{1} = - 1$，第一个投票者投赞成票，使得 $s = 1$。类似地，其余四个投票者也满足 $s \geq a_{i}$，因此全部投赞成票。最终的 $s$ 值为 5，这是可能的最大值。

相反，如果你将 $a$ 重新排列为 $\left[\right. 1 , 2 , 0 , 3 , - 1 \left]\right.$，那么对于从左到右的每个投票者，均满足 $s < a_{i}$，因此全部投反对票，导致 $s = - 5$。这是可能的最小值。另一种排列如 $\left[\right. 3 , 2 , 1 , 0 , - 1 \left]\right.$ 也会导致 $s = - 5$。

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
