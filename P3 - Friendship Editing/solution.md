### 核心思路

1. **问题重新表述**
   - 有效图必须是完全多部图或空图
   - 目标是找到一个分区方案,使得边翻转次数最少

2. **代价函数定义**
   对于一个分区 $P$,总代价为:
   
   $`
   \text{cost}(H) = \binom{n}{2} - (\text{G中的边数}) + \sum_{S\in P}f(S)
   `$
   其中 $`f(S)`$ 是块内代价:

   $`
   f(S) = \sum_{i<j,\; i,j\in S}(2G(i,j)-1)
   `$

4. **动态规划**
   - 预计算所有子集 $`S`$ 的 $`f(S)`$
   - DP状态: $`dp[mask]`$ 表示覆盖 $`mask`$ 顶点的最小分区代价
   - 转移方程:

     $`
     dp[mask] = \min_{\substack{S\subseteq mask\\ S\neq \emptyset,\, i\in S}}\{f(S) + dp[mask\setminus S]\}
     `$
     其中 $`i`$ 是 $`mask`$ 中最小的顶点

5. **最终答案**

   $`
   \text{answer} = \binom{n}{2} - m + dp[(1<<n)-1]
   `$

### 复杂度分析
这个方法的时间复杂度是 $O(3^n)$,对于 $n\leq 16$ 的情况可以在2秒内解决。
