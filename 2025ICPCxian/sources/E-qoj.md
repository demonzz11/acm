Title: statement_14685.pdf

URL Source: https://qoj.ac/problem/14685

Published Time: Tue, 21 Oct 2025 07:23:16 GMT

Number of Pages: 2

Markdown Content:
# Epilogue of Happiness 

Input file: standard input 

Output file: standard output 

Time limit: 7 seconds Memory limit: 1024 megabytes As the competition was nearing its end, a banquet was held. For decoration, there was a tree with n light bulbs numbered from 1 to n on it, where the i-th bulb has a beauty of wi. The bulbs are connected by 

n − 1 circuits. Specifically, for each i from 2 to n, there is a circuit connecting the i-th bulb and the fi-th bulb ( 1 ≤ fi < i ). There is a row of m switches that control the bulbs’ lighting. Pressing the i-th switch toggles the state of all bulbs on the simple path from bulb 1 to bulb oi on the tree (i.e., bulbs that are on turn off, and those that are off turn on). 

q children will interact with the tree. The process for the i-th child is described by three integers li, ri,and xi:• Initially, all bulbs are off. • Then, the switches from the li-th to the ri-th are pressed in sequence. • Finally, a photo is taken of the bulbs on the simple path from 1 to xi.The total beauty of a photo is the sum of the beauty of all bulbs that are on in the photo. Your task is to compute this value for each of the q children. 

# Input 

The first line of the input contains three integers n, m, and q (1 ≤ n, m, q ≤ 5 · 10 5), where n is the number of bulbs, m is the number of switches, and q is the number of children. The second line of the input contains n − 1 integers f2, f 3, . . . , f n (1 ≤ fi < i ), where fi represents a circuit between the i-th bulb and the fi-th bulb. The third line of the input contains n integers w1, w 2, . . . , w n (0 ≤ wi ≤ 1000 ), where wi is the beauty of the i-th bulb. The next m lines of the input describe the switches. The i-th line of these contains a single integer oi

(1 ≤ oi ≤ n). The next q lines of the input describe the interaction process of the children. The i-th line of these contains three integers li, ri, and xi (1 ≤ li ≤ ri ≤ m, 1 ≤ xi ≤ n). 

# Output 

Output q lines, each containing an integer, representing the total beauty of each photo. 

Page 1 of 2 Example 

standard input standard output 5 3 7 1 2 3 3 907 609 48 670 184 2351 2 5 1 3 3 1 3 2 2 3 1 2 3 3 2 3 4 3 3 5 48 1516 1516 0001748 

# Note 

For the first child: • First, he pressed the first switch, causing the first and the second bulbs to turn on. • Then, he pressed the second switch, causing the first and the second bulbs to turn off while the third bulb turns on. • He took a photo of bulbs 1, 2, 3, and 5. In the photo, only the 3rd bulb was lit, resulting in a beauty of 48 .

Page 2 of 2
