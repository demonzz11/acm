Title: Epilogue of Happiness - Problem

URL Source: https://qoj.ac/problem/14685/statement/zh_cn

Markdown Content:
随着比赛接近尾声，一场宴会举行了。为了装饰，有一棵挂着 $n$ 个灯泡的树，灯泡编号为 $1$ 到 $n$，其中第 $i$ 个灯泡的漂亮度为 $w_{i}$。灯泡由 $n - 1$ 条电路连接。具体来说，对于每个从 $2$ 到 $n$ 的 $i$，有一条电路连接第 $i$ 个灯泡和第 $f_{i}$ 个灯泡（$1 \leq f_{i} < i$）。

有一排共 $m$ 个开关控制灯泡的亮灭。按下第 $i$ 个开关会翻转树上从灯泡 $1$ 到灯泡 $o_{i}$ 的简单路径上所有灯泡的状态（即亮着的灯泡熄灭，熄灭的灯泡点亮）。

有 $q$ 个孩子将与这棵树进行交互。第 $i$ 个孩子的交互过程由三个整数 $l_{i}$、$r_{i}$ 和 $x_{i}$ 描述：

*   最初，所有灯泡都是熄灭的。
*   然后，依次按下从第 $l_{i}$ 个到第 $r_{i}$ 个开关。
*   最后，对从 $1$ 到 $x_{i}$ 的简单路径上的灯泡进行拍照。

一张照片的总漂亮度是照片中所有亮着的灯泡的漂亮度之和。你的任务是为这 $q$ 个孩子中的每一个计算这个值。

### 输入格式

第一行包含三个整数 $n$、$m$ 和 $q$（$1 \leq n , m , q \leq 5 \cdot 10^{5}$），其中 $n$ 是灯泡的数量，$m$ 是开关的数量，$q$ 是孩子的数量。

第二行包含 $n - 1$ 个整数 $f_{2} , f_{3} , \ldots , f_{n}$（$1 \leq f_{i} < i$），其中 $f_{i}$ 表示连接第 $i$ 个灯泡和第 $f_{i}$ 个灯泡的电路。

第三行包含 $n$ 个整数 $w_{1} , w_{2} , \ldots , w_{n}$（$0 \leq w_{i} \leq 1000$），其中 $w_{i}$ 是第 $i$ 个灯泡的漂亮度。

接下来的 $m$ 行描述开关。其中的第 $i$ 行包含一个整数 $o_{i}$（$1 \leq o_{i} \leq n$）。

接下来的 $q$ 行描述孩子的交互过程。其中的第 $i$ 行包含三个整数 $l_{i}$、$r_{i}$ 和 $x_{i}$（$1 \leq l_{i} \leq r_{i} \leq m$，$1 \leq x_{i} \leq n$）。

### 输出格式

输出 $q$ 行，每行包含一个整数，表示每张照片的总漂亮度。

### 样例

#### 输入样例 1

5 3 7
1 2 3 3
907 609 48 670 184
2
3
5
1 2 5
1 3 3
1 3 2
2 3 1
2 3 3
2 3 4
3 3 5
#### 输出样例 1

48
1516
1516
0
0
0
1748
### 说明

对于第一个孩子：

*   首先，他按下第一个开关，导致第一个和第二个灯泡点亮。
*   然后，他按下第二个开关，导致第一个和第二个灯泡熄灭，而第三个灯泡点亮。
*   他对灯泡 1, 2, 3 和 5 进行了拍照。在照片中，只有第 3 个灯泡是亮着的，因此漂亮度为 48。

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
