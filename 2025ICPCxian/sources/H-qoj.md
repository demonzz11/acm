Title: statement_14688.pdf

URL Source: https://qoj.ac/problem/14688

Published Time: Sun, 19 Oct 2025 01:48:11 GMT

Number of Pages: 1

Markdown Content:
# Heart of Darkness 

Input file: standard input 

Output file: standard output 

Time limit: 3 seconds Memory limit: 512 megabytes For a tree T , define v(T ) as the number of schemes that stain vertices of T in black and white, and satisfy the following conditions: • For all black vertices u, v in T , vertices in the simple path from u to v are all black. • There are at least k undirected edges (u, v ) satisfy u and v has different color. For all n vertices labeled unrooted tree T , calculate the sum of v(T ) modulo 998244353 .

# Input 

A single line contains two positive integers n, k (1 ≤ n ≤ 10 7, 1 ≤ k ≤ 5000 ). 

# Output 

A single integer as the answer. 

# Examples 

standard input standard output 3 1 15 6 2 17286 30 9 434031055 114514 2520 136362204 

# Note 

For the first test case, there are only 3 different T those are chains, so they have the same v(T ). Denote 

0 as black and 1 as white, there are 5 schemes: (0 , 0, 1) , (1 , 0, 0) , (1 , 1, 0) , (0 , 1, 1) , (1 , 0, 1) . We emphasize that schemes (1 , 1, 1) and (0 , 0, 0) don’t satisfy the second condition, and (0 , 1, 0) doesn’t satisfy the first one. Therefore, the answer is 3 · 5 = 15 .

Page 1 of 1
