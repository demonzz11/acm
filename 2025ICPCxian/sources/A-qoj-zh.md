Title: 杜鹃花园 - Problem - QOJ.ac

URL Source: https://qoj.ac/problem/14681/statement/zh_cn

Markdown Content:
在一个宁静的杜鹃花园中，出现了 $n$ 只危险的生物。每只生物都拥有攻击力和防御力。最初，第 $i$ 只生物（$1 \leq i \leq n$）的攻击力为 $a_{i}$，防御力为 $b_{i}$。

你作为杜鹃花园的守护者，可以在脑海中想象它们之间的一场战争。一场战争由若干次（可能为 $0$ 次）战斗组成。在每次想象的战斗中，你选择两只存活的生物 $i$ 和 $j$（$1 \leq i , j \leq n$，$i \neq j$），并且：

*   如果生物 $i$ 的攻击力大于或等于生物 $j$ 的防御力（即 $a_{i} \geq b_{j}$），那么 $i$ 可以在战斗中击败 $j$，并且 $j$ 被视为被消灭。
*   否则，什么也不会发生。

请注意，战争是想象出来的；也就是说，在同一场战争中被消灭的生物不能用于该场战争后续的战斗，但它会在下一场战争开始时恢复活力，从而可以参与后续战争的战斗。

这些生物是不稳定的，会随着时间的推移发生变异。给你 $q$ 次变异。在每次变异后，某只生物的属性会发生变化。具体来说，在第 $i$ 次变异中，第 $v_{i}$ 只生物的攻击力更新为 $x_{i}$，防御力更新为 $y_{i}$。请注意，变异是持久的；在第 $i$ 次变异之后，前 $i - 1$ 次变异的影响会累积。

由于每只残留的生物都会对花朵构成持续的威胁，你希望找到在想象战争的最佳战斗序列后，剩余生物的最小可能数量。你需要回答在所有变异开始前以及每次变异后的这一问题。

### 输入格式

输入的第一行包含两个整数 $n$ 和 $q$（$1 \leq n \leq 4 \cdot 10^{5}$，$0 \leq q \leq 4 \cdot 10^{5}$），其中 $n$ 是生物的数量，$q$ 是变异的次数。

接下来的 $n$ 行描述了所有的生物。其中的第 $i$ 行包含两个整数 $a_{i}$ 和 $b_{i}$（$1 \leq a_{i} , b_{i} \leq 10^{9}$），其中 $a_{i}$ 是第 $i$ 只生物的攻击力，$b_{i}$ 是其防御力。

接下来的 $q$ 行描述了所有的变异。其中的第 $i$ 行包含三个整数 $v_{i}$，$x_{i}$ 和 $y_{i}$（$1 \leq v_{i} \leq n$，$1 \leq x_{i} , y_{i} \leq 10^{9}$），其中 $x_{i}$ 是生物 $v_{i}$ 的新攻击力，$y_{i}$ 是生物 $v_{i}$ 的新防御力。

### 输出格式

输出 $q + 1$ 行，每行包含一个整数，依次表示在任何变异发生之前以及每次变异之后的答案。

### 样例

#### 输入格式 1

3 1
1 1
2 2
3 3
2 2 4
#### 输出格式 1

1
2
#### 说明

在样例中，在变异开始之前，第三只生物可以击败第一只和第二只生物。显然，这是想象战斗的最佳序列，因此答案为 $1$。

在第一次变异结束后，第二只生物的攻击力变为 $2$，防御力变为 $4$。现在第三只生物只能击败第一只生物，剩下 $2$ 只生物。可以证明不存在更好的方案，因此答案为 $2$。

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
