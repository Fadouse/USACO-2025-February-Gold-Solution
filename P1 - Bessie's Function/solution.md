### 核心思路

1. 循环检测:
   遍历每个节点,沿着唯一的出边前进。当revisit一个节点时,就检测到了一个循环。标记循环中的所有节点并存储。

2. 树形动态规划:
   对于不在循环中的每个节点u,进行DFS计算:
 
   
   $$dp[u][1] = c_u + \sum_{v\in child(u)} \min\{dp[v][0], dp[v][1]\}$$
   
 
   $$dp[u][0] = \sum_{v\in child(u)} dp[v][1]$$
   

3. 循环动态规划:
   对于循环中的每个节点u,先计算其树形子树的代价:
 
   $$T[u][0] = \sum_{v\in child(u)\setminus cycle} dp[v][1]$$
 
   $$T[u][1] = \sum_{v\in child(u)\setminus cycle} \min\{dp[v][0], dp[v][1]\}$$
   
 
   然后设置循环节点u的权重:
 
   $$w(u,0) = T[u][0]$$
 
   $$`
   w(u,1) = \begin{cases}
   T[u][1] & \text{if } a(u)=u \\
   c_u + T[u][1] & \text{otherwise}
   \end{cases}
   `$$
 
   最后在循环上运行DP(如果没有强制节点则使用两种情况的方法,如果有强制节点则"打破"循环)。

4. 输出:
   对所有循环组件的代价求和,得到使 $f(f(x))=f(x)$ 成立的最小总代价。

### 复杂度分析
该解法的时间复杂度为 $`O(N)`$ ,空间复杂度也为 $`O(N)`$ 。
