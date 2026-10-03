# K · No more regrets

[题单与难度](../README.md) · [C++17 参考代码](../std/K.cpp) · 官方题面 PDF 第 17–18 页

## 题意与约束

维护数组，支持区间加、区间赋值，以及查询

```math
\sum_{i=l}^r\left(\min_{j=l}^i a_j\right)\left(\max_{j=l}^i a_j\right)\pmod{2^{64}}.
```

$`n,q\le2\cdot10^5`$，操作过程中所有值始终在 $`[0,10^9]`$。区间加可以为负。注意查询是“所有前缀极值乘积之和”，不是区间最小值乘最大值。

## 切入点：前面只通过两个极值影响后面

顺序拼接两段时，左段对右段贡献的影响只由左段最小值 $`u`$ 与最大值 $`v`$ 决定。对节点区间定义

```math
C(N,u,v)=\sum_i\min(\operatorname{prefMin}_i,u)\max(\operatorname{prefMax}_i,v).
```

如果能够快速算这个函数，就可以合并线段树节点及处理查询。

## 节点信息

保存区间最小值 `mn`、最大值 `mx`、第一个元素 `first`，以及

- $`S_{\min}=\sum\operatorname{prefMin}`$；
- $`S_{\max}=\sum\operatorname{prefMax}`$；
- $`S_{\times}=\sum\operatorname{prefMin}\cdot\operatorname{prefMax}`$。

另外保存区间长度与赋值、加法懒标记。

先实现单侧函数

```math
L(N,u)=\sum\min(\operatorname{prefMin}_i,u),
\quad R(N,v)=\sum\max(\operatorname{prefMax}_i,v).
```

前缀最小值单调下降，前缀最大值单调上升，所以每层只递归一侧：以 $`L`$ 为例，若 $`u`$ 不超过左段最小值，左段被压平，递归右段；否则右段的真实前缀已不大于 $`u`$，右段贡献可直接由父节点 $`S_{\min}`$ 减去左节点 $`S_{\min}`$ 得到，只递归左段。每次 $`O(\log n)`$。

## 双侧函数的四种情况

设左儿子的最小、最大值为 $`p,q`$，左长为 $`t`$。

| 条件 | 左儿子贡献 | 右儿子处理 |
| --- | --- | --- |
| $`u\le p,v\ge q`$ | 全被压平，$`tuv`$ | 继续 $`C(\mathrm{right},u,v)`$ |
| $`u\ge p,v\le q`$ | 继续 $`C(\mathrm{left},u,v)`$ | 原节点的右段贡献，即父 $`S_\times`$ 减左 $`S_\times`$ |
| $`u<p,v<q`$ | $`u\,R(\mathrm{left},v)`$ | 继续 $`C(\mathrm{right},u,q)`$ |
| $`u>p,v>q`$（剩余情况） | $`v\,L(\mathrm{left},u)`$ | 继续 $`C(\mathrm{right},p,v)`$ |

关键是每层只有一条 $`C`$ 递归链，另一个儿子可以直接求值或转成单侧函数。因此一次 $`C`$ 为 $`O(\log^2 n)`$。

边界等号由代码按顺序归入对应条件，数学上结果一致。整段常数、整段完全压平，或者 $`u\ge\text{first},v\le\text{first}`$ 时可以直接返回已有信息。

## 合并、更新和查询

合并两个儿子时：

```math
S_\times=S_{\times,\mathrm{left}}+C(\mathrm{right},\mathrm{mn}_{\mathrm{left}},\mathrm{mx}_{\mathrm{left}}),
```

两个单侧和同理合并。

整段赋值 $`z`$ 时三种和变成 $`tz,tz,tz^2`$。整段增加 $`d`$ 时每个前缀极值同加 $`d`$，所以

```math
S_\times\leftarrow S_\times+d(S_{\min}+S_{\max})+td^2,
\quad S_{\min},S_{\max}\leftarrow S_{\min},S_{\max}+td.
```

查询按从左到右遍历覆盖节点，维护已经处理部分的极值 $`u,v`$，把每个节点贡献 $`C(N,u,v)`$ 加入答案，再更新 $`u,v`$。

## 正确性与复杂度

单侧和双侧递归均按“左段能否改变进入右段的极值”分类，四种情况完整覆盖且保留精确前缀贡献；由节点长度归纳可知三个和正确。整段变换公式逐项成立，懒标记不会改变节点相对前缀极值结构。按顺序拼接查询节点正是原查询的前缀序列，因此输出正确。

单次修改/查询 $`O(\log^3 n)`$，总上界 $`O((n+q)\log^3 n)`$，空间 $`O(n)`$。建树的实际界可更紧，这里使用统一上界。官方题面 PDF 标为 4 秒，题解说明讨论过 6 秒；应以提交站当前限制为准。本地通过不保证不同评测机运行时间。

所有和与答案使用 `unsigned long long`，自然溢出即模 $`2^{64}`$。实际数值、最值及加法懒标记仍用有符号 `long long`；把负 $`d`$ 转成无符号只发生在模运算公式中。

## 可迁移总结

这类题要先写“拼接时左边如何影响右边”，再决定节点维护什么，避免靠增加字段盲目凑合并。前缀极值的单调性支持单侧递归，左右贡献相减可以复用节点内部的预计算。相近专题：区间前缀最值和、带上下界的贡献查询、线段树单侧递归。短期冲三题时可暂缓，等基础线段树与 DP 熟练后再研究。
