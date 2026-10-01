Title: statement_14693.pdf

URL Source: https://qoj.ac/problem/14693

Published Time: Sun, 19 Oct 2025 01:50:41 GMT

Number of Pages: 1

Markdown Content:
# Mystique as Iris 

Input file: standard input 

Output file: standard output 

Time limit: 1 second Memory limit: 512 megabytes The following two steps, on an array x consisting of positive integers, are called an operation: 1. Select any two adjacent elements in x, decrease one of them by 1, and set the other to 0.2. Remove all zeros from x.We call x mystic if and only if it can be transformed into an empty sequence after a finite number of operations (possibly zero). You are given an array a consisting of n integers, as well as an integer m. Each element of a is either an integer from 1 to m or −1. Your task is to replace every occurrence of −1 in a with any integer from 1 to 

m.Determine the number of distinct mystic arrays a that can be obtained after the replacement. Since the answer can be very large, output it modulo 10 9 + 7 .

# Input 

The first line of the input contains two integers n and m (2 ≤ n ≤ 10 6, 1 ≤ m ≤ 10 8). The second line of the input contains n integers a1, a 2, . . . , a n (1 ≤ ai ≤ m or ai = −1). 

# Output 

Output a single integer, representing the number of distinct mystic sequences a that can be obtained, modulo 10 9 + 7 .

# Examples 

standard input standard output 2 2 -1 -1 36 10 -1 -1 -1 -1 1 7 9125 

# Note 

In the first test, the array a is [−1, −1] . By replacing both −1-s, one possible result is [1 , 2] . In this case, we select the two adjacent numbers: decrease the first by 1 and set the second to 0, obtaining [0 , 0] . After removing all zeros, the sequence becomes empty. Hence, [1 , 2] is a mystic sequence. Similarly, if we replace the −1-s with [1 , 1] or with [2 , 1] , both sequences can also be reduced to empty. Therefore, the total number of distinct mystic sequences is 3.

Page 1 of 1
