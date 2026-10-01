Title: statement_14684.pdf

URL Source: https://qoj.ac/problem/14684

Published Time: Sun, 19 Oct 2025 01:45:24 GMT

Number of Pages: 2

Markdown Content:
# Directed Acyclic Graph 

Input file: standard input 

Output file: standard output 

Time limit: 1 second Memory limit: 512 megabytes Given two integers n and m, you need to construct a directed acyclic graph (DAG) G = ( V, E ) with exactly n vertices and m edges. In the graph G, a vertex v is called reachable from vertex u if and only if there exists a path in the graph that starts at vertex u and ends at vertex v.For a non-empty set of vertices A ⊆ V , a vertex w is defined as good for A if and only if it is reachable from every vertex in A, and we denote f (A) as the set of all good vertices for A.The graph you constructed should satisfy both of the following constraints: • For every vertex i (1 ≤ i ≤ n), it is reachable from vertex 1.• There exist k distinct non-empty sets of vertices S1, S 2, . . . , S k, such that f (S1), f (S2), . . . , f (Sk)

are pairwise distinct. Note that f (Si) can be empty. To prove the graph G you constructed satisfies the second constraint, you also need to provide k sets 

S1, S 2, . . . , S k that satisfy the second constraint. 

# Input 

The only line of the input contains three integers n, m , and k.There are only 2 tests in this problem: 1. n = 5 , m = 6 , k = 6 ;2. n = 100 , m = 128 , k = 16 000 .

# Output 

The first m lines of the output describe the graph G you construct. Each line contains two integers u, v 

representing an edge from u to v in the graph. The next k lines of the output describe the k sets of vertices you provide. The i-th line first contains the size of the set Si, followed by the |Si| numbers representing each vertex in the set. 

# Example 

standard input standard output 5 6 6 1 2 1 3 2 4 3 5 2 5 3 4 1 1 1 2 1 3 1 4 1 5 2 2 3 

Page 1 of 2 Note 

In the example, the output constructs a graph with n = 5 vertices and m = 6 edges. The corresponding 

k = 6 sets are S1 = {1}, S 2 = {2}, S 3 = {3}, S 4 = {4}, S 5 = {5}, S 6 = {2, 3}.Here, vertices 4 and 5 can both be reached from any element in S6 = {2, 3}, so f ({2, 3}) = {4, 5}. Together with f (S1) = {1, 2, 3, 4, 5}, f (S2) = {2, 4, 5}, f (S3) = {3, 4, 5}, f (S4) = {4}, f (S5) = {5}, these sets are all distinct, satisfying the constraints. 12 34 5   

> f({2})f({3})

Page 2 of 2
