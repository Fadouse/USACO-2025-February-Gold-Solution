#include <bits/stdc++.h>
using namespace std;

/* 18/18 passed, Results:
+-------+--------+---------+
| Index |  Size  |   Time  |
+-------+--------+---------+
|   1   | 3.4mb  |  2ms    |
|   2   | 3.4mb  |  2ms    |
|   3   | 3.4mb  |  2ms    |
|   4   | 3.4mb  |  2ms    |
|   5   | 3.4mb  |  2ms    |
|   6   | 3.4mb  |  2ms    |
|   7   | 3.4mb  |  2ms    |
|   8   | 3.4mb  |  2ms    |
|   9   | 3.4mb  |  2ms    |
|  10   | 3.4mb  |  3ms    |
|  11   | 3.4mb  |  5ms    |
|  12   | 3.5mb  | 10ms    |
|  13   | 3.4mb  | 23ms    |
|  14   | 3.6mb  | 58ms    |
|  15   | 3.6mb  | 58ms    |
|  16   | 3.6mb  | 58ms    |
|  17   | 3.6mb  | 59ms    |
|  18   | 3.6mb  | 59ms    |
+-------+--------+---------+
*/


int n, m;
int g[20][20];

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> m;
    memset(g, 0, sizeof(g));
    // 读入边，构建无向图邻接矩阵
    for (int i = 0; i < m; i++){
        int u, v;
        cin >> u >> v;
        u--; v--;
        g[u][v] = g[v][u] = 1;
    }
    // 计算总边数差：完全图边数 - 实际边数
    int totalPairs = n*(n-1)/2;
    int constant = totalPairs - m;

    int Nmask = 1 << n;
    vector<int> blockCost(Nmask, 0);
    // 预处理每个子集的块代价
    for (int mask = 0; mask < Nmask; mask++){
        vector<int> verts;
        for (int i = 0; i < n; i++){
            if(mask & (1 << i))
                verts.push_back(i);
        }
        int cost = 0;
        int sz = verts.size();
        if(sz < 2){
            blockCost[mask] = 0;
            continue;
        }
        // 对子集内每对顶点计算代价
        for (int i = 0; i < sz; i++){
            for (int j = i+1; j < sz; j++){
                int u = verts[i], v = verts[j];
                cost += (2 * g[min(u,v)][max(u,v)] - 1);
            }
        }
        blockCost[mask] = cost;
    }

    const int INF = 1 << 28;
    vector<int> dp(Nmask, INF);
    dp[0] = 0;
    // 状态dp
    // mask表示已分块的顶点集合，求最小代价
    for (int mask = 1; mask < Nmask; mask++){
        int least = __builtin_ctz(mask); // 取mask中最低位顶点
        // 枚举mask的所有子集sub，其中必须包含least
        for (int sub = mask; sub; sub = (sub - 1) & mask){
            if(!(sub & (1 << least))) continue;
            dp[mask] = min(dp[mask], blockCost[sub] + dp[mask ^ sub]);
        }
    }

    // 答案：constant + dp[全集]
    int answer = constant + dp[Nmask - 1];
    cout << answer << "\n";
    return 0;
}
