Title: statement_14687.pdf

URL Source: https://qoj.ac/problem/14687

Published Time: Sun, 19 Oct 2025 01:48:01 GMT

Number of Pages: 1

Markdown Content:
# Grand Voting 

Input file: standard input 

Output file: standard output 

Time limit: 1 second Memory limit: 512 megabytes Dada organized a contest, but it received heavy downvotes. He decided to start manipulating the comments. This contest has s votes, initially set to 0.There are n participants, each with a voting parameter ai. When it’s their turn to vote: • If s ≥ ai, they cast an upvote, incrementing s by 1.• If s < a i, they cast a downvote, decrementing s by 1.Dada can control the voting order of these n people. He wants to know the maximum and minimum possible vote count s in this contest. 

# Input 

The first line of input contains a single integer n (1 ≤ n ≤ 10 5), representing the number of voters. The next line of input contains n integers a1, a 2, · · · , a n (|ai| ≤ 10 5), separated by spaces. 

# Output 

Output one line containing two integers separated by a space, representing the maximum and minimum vote count s in this contest. 

# Example 

standard input standard output 5-1 0 1 2 3 5 -5 

# Note 

For example, if you rearrange a to [−1, 0, 1, 2, 3] , initially s = 0 . Since s ≥ a1 = −1, the first voter casts an upvote, making s = 1 . Similarly, the remaining four voters also satisfy s ≥ ai, so all cast upvotes. The final value of s is 5, which is the maximum possible. Conversely, if you rearrange a to [1 , 2, 0, 3, −1] , then for each voter from left to right, s < a i holds, so all cast downvotes, resulting in s = −5. This is the minimum possible. Another arrangement such as 

[3 , 2, 1, 0, −1] also leads to s = −5.

Page 1 of 1
