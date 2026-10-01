Title: statement_14692.pdf

URL Source: https://qoj.ac/problem/14692

Published Time: Tue, 21 Oct 2025 07:36:06 GMT

Number of Pages: 2

Markdown Content:
# Let’s Make a Convex! 

Input file: standard input 

Output file: standard output 

Time limit: 1 second Memory limit: 512 megabytes Kevin is the chief judge of the International Convex Polygon Championship (ICPC) . He proposed a geometry task for the contest. However, due to his lack of experience in geometry, he was unable to generate the correct convex polygons for the task. To prove his geometry skills, Kevin starts playing with sticks. He has n sticks, the i-th of which has a length ai. He would like to select k sticks so that they can be arranged as a non-degenerate convex polygon. Since stronger test data are needed, Kevin wants to maximize the perimeter of the polygon (i.e., maximize the sum of ai of all sticks in the subset). Could you help him find out the value for all integers k from 1

to n? If no such polygon exists, tell him a single integer 0 instead. 

# Input 

The input consists of multiple test cases. The first line contains an integer t (1 ≤ t ≤ 10 5), the number of test cases. For each test case: • The first line contains a single integer n (1 ≤ n ≤ 2 · 10 5), which is the number of sticks. • The second line contains n integers a1, . . . , a n (1 ≤ ai ≤ 10 9), which are the lengths of each stick. It is guaranteed that the sum of n over all test cases does not exceed 2 · 10 5.

# Output 

For each test case, output a single line containing n integers, representing the maximal perimeter of the polygon. Specifically, if no such polygon exists, output a single integer 0.

# Example 

standard input standard output 751 2 3 4 5 51 2 4 8 16 52 2 2 2 2 41 4 10 7 21 2 32 3 4 43 1 2 6 0 0 12 14 15 0 0 0 0 0 0 0 6 8 10 0 0 21 22 0 0 0 0 9 0 0 0 0 

# Note 

In the first test case, it can be shown that there does not exist a convex polygon of 1 or 2 sides. When k = 3 ,the maximal perimeter of the convex polygon is 12 , since sticks of side lengths 3, 4, and 5 are known for 

Page 1 of 2 forming a right triangle. Similarly, when k = 4 , the maximal perimeter of the polygon is 2 + 3 + 4 + 5 = 14 ;when k = 5 , selecting all sticks is a valid scheme and obtains a perimeter of 1 + 2 + 3 + 4 + 5 = 15 .In the second test case, it can be proven that no matter how the sticks are selected, they cannot form a convex polygon. 

Page 2 of 2
