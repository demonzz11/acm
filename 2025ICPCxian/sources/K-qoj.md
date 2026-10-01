Title: statement_14691.pdf

URL Source: https://qoj.ac/problem/14691

Published Time: Wed, 27 May 2026 10:28:22 GMT

Number of Pages: 2

Markdown Content:
# Killing Bits 

Input file: standard input 

Output file: standard output 

Time limit: 4 seconds Memory limit: 512 megabytes You are given two arrays a and b, both consisting of n non-negative integers. You can perform the following operation on the array a an arbitrary number of times (possibly, zero): • First, you select a permutation p of 0, 1, . . . , n − 1;• Then, for each 1 ≤ i ≤ n, you set ai to ai & pi. Here, & denotes the bitwise AND operation. You have to determine whether it is possible to transform a into b.

# Input 

The input consists of multiple test cases. The first line contains an integer t (1 ≤ t ≤ 10 4), the number of test cases. For each test case: • The first line contains a single integer n (1 ≤ n ≤ 5 · 10 4), which is the length of arrays a and b.• The second line contains n integers a1, a 2, . . . , a n (0 ≤ ai ≤ n − 1), which are the elements of a.• The third line contains n integers b1, b 2, . . . , b n (0 ≤ bi ≤ n − 1), which are the elements of b.It is guaranteed that the sum of n over all test cases does not exceed 5 · 10 4.

# Output 

For each test case, print “ Yes ” in a single line if it is possible to transform a into b. Otherwise, print “ No ”. You can output the answer in any case (upper or lower). For example, the strings “ yEs ”, “ yes ”, “ Yes ”, and “YES ” will be recognized as positive responses. 

# Example 

standard input standard output 430 1 2 2 1 0 51 0 1 3 4 0 0 1 1 4 81 2 3 4 5 6 7 7 1 2 3 4 5 6 7 7 87 7 7 7 7 7 7 7 1 2 3 4 5 6 7 7 No Yes Yes No 

# Note 

In the first test case, we need to use at least one operation to transform a into b. Note that a1 & p1 is always 0 because a1 = 0 . However, b1 > 0, so it is impossible to make a1 = b1, no matter how the permutations are selected during the operations. 

Page 1 of 2 In the second test case, you can select p = [2 , 0, 3, 1, 4] . After this operation, a is transformed into b.In the third test case, a = b, so we do not need any operations. 

Page 2 of 2
