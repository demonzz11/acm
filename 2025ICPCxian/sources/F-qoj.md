Title: statement_14686.pdf

URL Source: https://qoj.ac/problem/14686

Published Time: Mon, 17 Aug 2026 10:38:54 GMT

Number of Pages: 2

Markdown Content:
# Follow the Penguins 

Input file: standard input 

Output file: standard output 

Time limit: 3 seconds Memory limit: 512 megabytes There are n penguins standing on a number line. The i-th penguin is initially located at coordinate ai. It is guaranteed that all ai-s are pairwise distinct. Each penguin chooses a target penguin, denoted by ti (1 ≤ ti ≤ n, ti̸ = i). At time 0, all penguins start moving simultaneously. Each one runs towards the current position of its target penguin at a constant speed of 0.5 units per second. When penguin i meets penguin ti, it stops immediately. For every penguin, determine the time when it stops moving. Here, penguin i meets penguin ti if and only if they are at the same coordinate at the same time. It can be proven that every penguin will stop moving within a finite amount of time, and the stopping time is always an integer. 

# Input 

The first line of the input contains a single integer n (2 ≤ n ≤ 5 · 10 5), which is the number of penguins. The second line of the input contains n integers t1, t 2, . . . , t n (1 ≤ ti ≤ n, ti̸ = i), where ti is the target chosen by the i-th penguin. The third line of the input contains n distinct integers a1, a 2, . . . , a n (−5 · 10 8 ≤ ai ≤ 5 · 10 8), where ai is the initial coordinate of the i-th penguin. 

# Output 

Output a single line containing n integers, where the i-th integer represents the time (in seconds) when the i-th penguin stops moving. 

# Examples 

standard input standard output 32 3 1 -1 2 3 7 1 4 10 8 3 6 7 1 8 2 10 8 1 0 -14 5 -3 14 -12 11 8 -18 17 25 21 17 14 14 49 29 9 61 17 

# Note 

In the example, initially, since the second penguin is in the positive direction of the first penguin, the first penguin runs in the positive direction. Similarly, the second penguin runs in the positive direction, while the third penguin runs in the negative direction. The initial positions of the three penguins on the number line are shown in the figure below: 

1 2 3

−1 0 2 3

Page 1 of 2 At second 1, the second penguin and the third penguin meet at x = 2 .5, at which point the second penguin stops moving. 

1 2/3  

> −0.5

0 2.5

At this moment, the first penguin is at −0.5, and the second penguin is at 2.5. The distance between them is 3. Since the first penguin runs at a speed of 0.5 units per second, it will take 6 more seconds to reach the second penguin. Therefore, the first penguin stops moving at second 7.Before the first penguin stops, the third penguin meets it at second 4. So the answers are 7, 1, and 4,respectively. 

Page 2 of 2
