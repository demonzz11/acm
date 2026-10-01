Title: 有向无环图 - Problem - QOJ.ac

URL Source: https://qoj.ac/problem/14684/statement/zh_cn

Markdown Content:
给定两个整数 $n$ 和 $m$，你需要构造一个包含恰好 $n$ 个顶点和 $m$ 条边的有向无环图（DAG） $G = \left(\right. V , E \left.\right)$。

在图 $G$ 中，当且仅当存在一条从顶点 $u$ 开始并以顶点 $v$ 结束的路径时，称顶点 $v$ 是从顶点 $u$**可达**的。

对于非空顶点集 $A \subseteq V$，当且仅当顶点 $w$ 从 $A$ 中的每个顶点都是可达的时，定义顶点 $w$ 对于 $A$ 是**优秀的**（good）。我们用 $f \left(\right. A \left.\right)$ 表示对于 $A$ 的所有优秀顶点的集合。

你构造的图应该同时满足以下两个约束条件：

*   对于每个顶点 $i$（$1 \leq i \leq n$），它都是从顶点 $1$ 可达的。
*   存在 $k$ 个不同的非空顶点集 $S_{1} , S_{2} , \ldots , S_{k}$，使得 $f \left(\right. S_{1} \left.\right) , f \left(\right. S_{2} \left.\right) , \ldots , f \left(\right. S_{k} \left.\right)$ 两两不同。注意 $f \left(\right. S_{i} \left.\right)$ 可以为空集。

为了证明你构造的图 $G$ 满足第二个约束条件，你还需要提供满足该约束条件的 $k$ 个集合 $S_{1} , S_{2} , \ldots , S_{k}$。

### 输入格式

输入唯一的一行包含三个整数 $n$，$m$ 和 $k$。

本题中只有 2 个测试点：

1.   $n = 5 , m = 6 , k = 6$；
2.   $n = 100 , m = 128 , k = 16 000$。

### 输出格式

输出的前 $m$ 行描述你构造的图 $G$。每行包含两个整数 $u , v$，表示图中的一条从 $u$ 到 $v$ 的有向边。

接下来的 $k$ 行描述你提供的 $k$ 个顶点集。第 $i$ 行首先包含集合 $S_{i}$ 的大小，后跟 $\left|\right. S_{i} \left|\right.$ 个代表该集合中每个顶点的数字。

### 样例

#### 输入样例 1

5 6 6
#### 输出样例 1

1 2
1 3
2 4
3 5
2 5
3 4
1 1
1 2
1 3
1 4
1 5
2 2 3
### 说明

在样例中，输出构造了一个包含 $n = 5$ 个顶点和 $m = 6$ 条边的图。对应的 $k = 6$ 个集合为 $S_{1} = \left{\right. 1 \left.\right} , S_{2} = \left{\right. 2 \left.\right} , S_{3} = \left{\right. 3 \left.\right} , S_{4} = \left{\right. 4 \left.\right} , S_{5} = \left{\right. 5 \left.\right} , S_{6} = \left{\right. 2 , 3 \left.\right}$。

这里，顶点 4 和 5 都可以从 $S_{6} = \left{\right. 2 , 3 \left.\right}$ 中的任意元素到达，因此 $f \left(\right. \left{\right. 2 , 3 \left.\right} \left.\right) = \left{\right. 4 , 5 \left.\right}$。连同 $f \left(\right. S_{1} \left.\right) = \left{\right. 1 , 2 , 3 , 4 , 5 \left.\right}$，$f \left(\right. S_{2} \left.\right) = \left{\right. 2 , 4 , 5 \left.\right}$，$f \left(\right. S_{3} \left.\right) = \left{\right. 3 , 4 , 5 \left.\right}$，$f \left(\right. S_{4} \left.\right) = \left{\right. 4 \left.\right}$，$f \left(\right. S_{5} \left.\right) = \left{\right. 5 \left.\right}$，这些集合两两不同，满足约束条件。

![Image 1](https://qoj.ac/problem/14684/statement/IMAGE_PLACEHOLDER)

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
