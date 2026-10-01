Title: 跟随企鹅 - Problem - QOJ.ac

URL Source: https://qoj.ac/problem/14686/statement/zh_cn

Markdown Content:
有 $n$ 只企鹅站在数轴上。第 $i$ 只企鹅的初始坐标为 $a_{i}$。保证所有的 $a_{i}$ 两两不同。

每只企鹅选择一只目标企鹅，记为 $t_{i}$（$1 \leq t_{i} \leq n$ 且 $t_{i} \neq i$）。在时刻 $0$，所有企鹅同时开始移动。每只企鹅都以每秒 $0.5$ 单位长度的恒定速度向其目标企鹅的当前位置奔跑。

当企鹅 $i$ 与企鹅 $t_{i}$ 相遇时，它会立即停止移动。对于每只企鹅，确定它停止移动的时间。这里，企鹅 $i$ 与企鹅 $t_{i}$ 相遇当且仅当它们在同一时刻处于同一坐标。

可以证明，每只企鹅都会在有限时间内停止移动，且停止时间总是整数。

### 输入格式

输入的第一行包含一个整数 $n$（$2 \leq n \leq 5 \cdot 10^{5}$），表示企鹅的数量。

第二行包含 $n$ 个整数 $t_{1} , t_{2} , \ldots , t_{n}$（$1 \leq t_{i} \leq n$ 且 $t_{i} \neq i$），其中 $t_{i}$ 是第 $i$ 只企鹅选择的目标企鹅。

第三行包含 $n$ 个互不相同的整数 $a_{1} , a_{2} , \ldots , a_{n}$（$- 5 \cdot 10^{8} \leq a_{i} \leq 5 \cdot 10^{8}$），其中 $a_{i}$ 是第 $i$ 只企鹅的初始坐标。

### 输出格式

输出一行，包含 $n$ 个整数，其中第 $i$ 个整数表示第 $i$ 只企鹅停止移动的时间（以秒为单位）。

### 样例

#### 输入样例 1

3
2 3 1
-1 2 3
#### 输出样例 1

7 1 4
#### 输入样例 2

10
8 3 6 7 1 8 2 10 8 1
0 -14 5 -3 14 -12 11 8 -18 17
#### 输出样例 2

25 21 17 14 14 49 29 9 61 17
### 说明

在样例 1 中，最初由于第二只企鹅在第一只企鹅的正方向，第一只企鹅向正方向奔跑。类似地，第二只企鹅向正方向奔跑，而第三只企鹅向负方向奔跑。这三只企鹅在数轴上的初始位置如下图所示：

![Image 1](https://qoj.ac/problem/14686/statement/IMAGE_PLACEHOLDER)
在第 $1$ 秒时，第二只企鹅和第三只企鹅在 $x = 2.5$ 处相遇，此时第二只企鹅停止移动。

![Image 2](https://qoj.ac/problem/14686/statement/IMAGE_PLACEHOLDER)
此时，第一只企鹅位于 $- 0.5$，第二只企鹅位于 $2.5$。它们之间的距离为 $3$。由于第一只企鹅以每秒 $0.5$ 单位长度的速度奔跑，它还需要 $6$ 秒才能到达第二只企鹅的位置。因此，第一只企鹅在第 $7$ 秒停止移动。

在第一只企鹅停止之前，第三只企鹅在第 $4$ 秒与它相遇。因此，答案分别为 $7$、$1$ 和 $4$。

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
