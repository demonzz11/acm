Title: statement_14681.pdf

URL Source: https://qoj.ac/problem/14681

Published Time: Sat, 26 Sep 2026 13:41:03 GMT

Number of Pages: 2

Markdown Content:
# Azalea Garden 

Input file: standard input 

Output file: standard output 

Time limit: 6 seconds Memory limit: 512 megabytes In a serene garden of azaleas, n dangerous creatures have appeared. Each creature possesses an attack power and a defense power . Initially, the i-th creature ( 1 ≤ i ≤ n) has an attack power of ai and a defense power of bi.You, the guardian of the azalea garden, can mentally imagine a war between them. A war consists of several (possibly, 0) battles . In each imagined battle , you choose two living creatures i and j (1 ≤ i, j ≤ n,

i̸ = j), and: • If the attack power of creature i is greater than or equal to the defense power of creature j (i.e. 

ai ≥ bj ), then i can defeat j in the battle, and j is considered eliminated. • Otherwise, nothing happens. Note that the wars are imaginary; that is, a creature eliminated cannot be used for future battles in the same war , but it regains its vigor at the beginning of the next war , and thus can participate in future 

battles of consequent wars .The creatures are volatile and undergo mutations over time. You are given q mutations. After each of the mutations, the attributes of a creature change. Specifically, in the i-th mutation, the vi-th creature has its attack power updated to xi and its defense power updated to yi. Note that the mutations are persistent; After the i-th mutation, the impacts of the first i − 1 mutations are accumulated. Since each remaining creature poses an ongoing threat to the flowers, you want to find the minimum possible number of creatures that remain after an optimal sequence of an imagined war . You need to answer the question for all states before all mutations and after each mutation. 

# Input 

The first line of the input contains two integers n and q (1 ≤ n ≤ 4 · 10 5, 0 ≤ q ≤ 4 · 10 5), where n is the number of creatures and q is the number of mutations. The next n lines of the input describe all the creatures. The i-th line of these contains two integers ai and 

bi (1 ≤ ai, b i ≤ 10 9), where ai is the attack power and bi is the defense power of the i-th creature. The next q lines of the input describe all the mutations. The i-th line of these contains three integers vi,

xi, and yi (1 ≤ vi ≤ n, 1 ≤ xi, y i ≤ 10 9), where xi is the new attack power of creature vi and yi is the new defense power of creature vi.

# Output 

Output q + 1 lines, each containing a single integer, which are the answers before any mutations and after each mutation in sequence. 

# Example 

standard input standard output 3 1 1 1 2 2 3 3 2 2 4 12

Page 1 of 2 Note 

In the example, before the mutations begin, the third creature can defeat the first and second creatures. Clearly, this is the optimal sequence of imagined battles, so the answer is 1.After the first mutation ends, the attack power of the second creature becomes 2, and its defense power becomes 4. Now the third creature can only defeat the first creature, leaving 2 creatures remaining. It can be proved that no better solution exists, so the answer is 2.

Page 2 of 2
