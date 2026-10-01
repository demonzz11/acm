Title: statement_14682.pdf

URL Source: https://qoj.ac/problem/14682

Published Time: Sun, 19 Oct 2025 01:44:05 GMT

Number of Pages: 2

Markdown Content:
# Beautiful Dangos 

Input file: standard input 

Output file: standard output 

Time limit: 1 second Memory limit: 512 megabytes Little Cyan Fish likes to eat Tricolor Dango !

Figure 1: Little Cyan Fish 

Now there are n dangos arranged in a string, and each dango is colored cyan ( C), white ( W), or pink ( P). The dangos are numbered from 1 to n.Little Cyan Fish considers a string of dangos to be beautiful if and only if any two adjacent dangos have different colors. To make this string of tricolor dango more beautiful, Little Cyan Fish decides to select an interval [l, r ]

(1 ≤ l ≤ r ≤ n), and rearrange all dangos in this interval arbitrarily, so that the entire string of dango becomes beautiful after the rearrangement. Little Cyan Fish wants to make the interval he selects as short as possible. Can you help him? You need to output the optimal interval, as well as the whole string after rearrangement. Note that the original string may already be beautiful, or it might be impossible to make it beautiful through any rearrangement. 

# Input 

The input consists of multiple test cases. The first line contains an integer t (1 ≤ t ≤ 10 5), the number of test cases. For each test case: • The first line contains an integer n (1 ≤ n ≤ 2 · 10 6), which is the number of dangos in this string. • The second line contains a string of length n, where the i-th character denotes the color of the i-th dango, with “ C” representing cyan, “ W” representing white, and “ P” representing pink. It is guaranteed that the sum of n over all test cases does not exceed 2 · 10 6.

# Output 

For each test case: • If the string of dangos is already beautiful, output a single line “ Beautiful ”. • Otherwise, if it is impossible to make the string of dangos beautiful through any rearrangement, output a single line “ Impossible ”. 

Page 1 of 2 • Otherwise, output three lines: 

⋄ The first line should contain the word “ Possible ”. 

⋄ The second line should contain two integers l and r, representing the selected interval (1 ≤ l ≤ r ≤ n). 

⋄ The third line should contain a string of length n, representing the colors of all dangos after rearrangement. If there are multiple possible solutions, you may output any of them. 

# Example 

standard input standard output 45CWCWC 6CWCCPW 3PPP 8CWPPCWWC Beautiful Possible 4 5 CWCPCW Impossible Possible 4 6 CWPWCPWC 

# Note 

In the first test case, the string of dangos is already beautiful.     

> 12345

In the second test case, initially, the string of dangos is not beautiful because two adjacent dangos, the third and the fourth, are both cyan. But Little Cyan Fish can resolve this by selecting the interval [4 , 5] 

and swapping the two dangos within it.           

> 123456123456

It can be easily shown that this solution selects an interval of the shortest possible length. In the third test case, Little Cyan Fish can’t make it beautiful through any rearrangement. 

Page 2 of 2
