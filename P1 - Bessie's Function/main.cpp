#include <bits/stdc++.h>
using namespace std;

/* 21/21 passed, Results:
+-------+---------+-------+
| Index |  Size   | Time  |
+-------+---------+-------+
|   1   | 3.4mb   | 2ms   |
|   2   | 3.4mb   | 2ms   |
|   3   | 3.4mb   | 2ms   |
|   4   | 40.0mb  | 95ms  |
|   5   | 39.5mb  | 98ms  |
|   6   | 37.1mb  | 88ms  |
|   7   | 39.8mb  | 115ms |
|   8   | 39.7mb  | 115ms |
|   9   | 37.1mb  | 106ms |
|  10   | 47.9mb  | 137ms |
|  11   | 52.0mb  | 140ms |
|  12   | 64.0mb  | 146ms |
|  13   | 45.8mb  | 157ms |
|  14   | 57.5mb  | 163ms |
|  15   | 64.0mb  | 162ms |
|  16   | 46.1mb  | 135ms |
|  17   | 52.5mb  | 142ms |
|  18   | 37.1mb  | 96ms  |
|  19   | 50.8mb  | 157ms |
|  20   | 47.2mb  | 166ms |
|  21   | 37.1mb  | 117ms |
+-------+---------+-------+
*/

typedef long long ll;
const ll INF = 1e18;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

  //处理输入数据
    int N;
    cin >> N;
    vector<int> a(N+1);
    for (int i = 1; i <= N; i++){
        cin >> a[i];
    }
    vector<ll> c(N+1);
    for (int i = 1; i <= N; i++){
        cin >> c[i];
    }
    vector<vector<int>> children(N+1);
    for (int i = 1; i <= N; i++){
        children[a[i]].push_back(i);
    }

    vector<int> state(N+1, 0);
    vector<bool> inCycle(N+1, false);
    vector<int> cycleId(N+1, -1);
    int cycleCount = 0;
    vector<vector<int>> cycles;


    for (int i = 1; i <= N; i++) {
        if (state[i] != 0) continue;  // 已处理则跳过
        int cur = i;
        vector<int> path;           // 当前访问路径

        while (true) {
            if (state[cur] == 0) {  // 未访问
                state[cur] = 1;     // 标记为访问中
                path.push_back(cur);
                cur = a[cur];       // 移动到下一个节点
            } else if (state[cur] == 1) {  // 发现环
                int pos = 0;
                while (path[pos] != cur) pos++;  // 找到环起点
                vector<int> cyc;
                for (int j = pos; j < (int)path.size(); j++) {
                    cyc.push_back(path[j]);
                    inCycle[path[j]] = true;
                    cycleId[path[j]] = cycleCount;
                }
                cycles.push_back(cyc);
                cycleCount++;
                for (auto node : path)
                    state[node] = 2;  // 标记为处理完成
                break;
            } else {  // 已处理过的节点
                for (auto node : path)
                    state[node] = 2;  // 标记路径上节点为完成
                break;
            }
        }
    }

    // 树状DP
    vector<vector<ll>> dp(N+1, vector<ll>(2,0));
    vector<bool> dpComputed(N+1, false);
    function<void(int)> dfsTree = [&](int u) {
        dpComputed[u] = true;
        dp[u][1] = c[u];  // 选择节点u
        dp[u][0] = 0;     // 不选择节点u
        for (int v : children[u]) {
            if(inCycle[v]) continue;  // 排除环中节点
            if(!dpComputed[v]) dfsTree(v);
            dp[u][1] += min(dp[v][0], dp[v][1]);  // 子树任选最小值
            dp[u][0] += dp[v][1];                  // 不选u则必须选子节点
        }
    };
    // 对非环节点执行树状DP
    for (int i = 1; i <= N; i++){
        if(!inCycle[i] && !dpComputed[i])
            dfsTree(i);
    }

    vector<vector<ll>> T(N+1, vector<ll>(2, 0));
    // 计算环节点的树状DP值
    for (int u = 1; u <= N; u++){
        if(!inCycle[u]) continue;
        ll sum0 = 0, sum1 = 0;
        for (int v : children[u]){
            if(inCycle[v]) continue;
            sum0 += dp[v][1];
            sum1 += min(dp[v][0], dp[v][1]);
        }
        T[u][0] = sum0;
        T[u][1] = sum1;
    }

    vector<ll> weight0(N+1, 0), weight1(N+1, 0);
    vector<bool> forced(N+1, false);
    // 计算每个环节点的权值及是否强制选取
    for (int u = 1; u <= N; u++){
        if(!inCycle[u]) continue;
        if(a[u] == u){  // 自环节点
            forced[u] = true;
            weight0[u] = INF;
            weight1[u] = 0 + T[u][1];
        } else {
            forced[u] = false;
            weight0[u] = T[u][0];
            weight1[u] = c[u] + T[u][1];
        }
    }

    ll ans = 0;
    vector<bool> cycleProcessed(cycleCount, false);
    // 对所有环进行环状DP求解
    for (int i = 1; i <= N; i++){
        if(!inCycle[i]) continue;
        int cid = cycleId[i];
        if(cycleProcessed[cid]) continue;
        const vector<int>& cyc = cycles[cid];
        vector<int> order;
        {   // 构造环的顺序序列
            int start = cyc[0];
            int cur = start;
            do{
                order.push_back(cur);
                cur = a[cur];
            } while(cur != start);
        }

        int m = order.size();
        ll cycCost = 0;
        if(m == 1){
            // 单个节点的环
            cycCost = weight1[order[0]];
        } else {
            bool hasForced = false;
            int forcedIndex = -1;
            for (int j = 0; j < m; j++){
                if (forced[order[j]]){
                    hasForced = true;
                    forcedIndex = j;
                    break;
                }
            }
            if(hasForced){
                // 环中存在必须选取的节点，转为线性DP
                vector<int> lin;
                for (int j = 0; j < m; j++){
                    lin.push_back(order[(forcedIndex + j) % m]);
                }
                vector<vector<ll>> dpCycle(m, vector<ll>(2, INF));
                dpCycle[0][1] = weight1[lin[0]];
                dpCycle[0][0] = INF;
                for (int j = 1; j < m; j++){
                    int u = lin[j];
                    dpCycle[j][1] = weight1[u] + min(dpCycle[j-1][0], dpCycle[j-1][1]);
                    dpCycle[j][0] = weight0[u] + dpCycle[j-1][1];
                }
                cycCost = min(dpCycle[m-1][0], dpCycle[m-1][1]);
            } else {
                // 无强制选取节点，需两种情况取最小
                vector<vector<ll>> dpCycleA(m, vector<ll>(2, INF));
                dpCycleA[0][1] = weight1[order[0]];
                dpCycleA[0][0] = INF;
                for (int j = 1; j < m; j++){
                    int u = order[j];
                    dpCycleA[j][1] = weight1[u] + min(dpCycleA[j-1][0], dpCycleA[j-1][1]);
                    dpCycleA[j][0] = weight0[u] + dpCycleA[j-1][1];
                }
                ll candA = min(dpCycleA[m-1][0], dpCycleA[m-1][1]);

                vector<vector<ll>> dpCycleB(m, vector<ll>(2, INF));
                dpCycleB[0][0] = weight0[order[0]];
                dpCycleB[0][1] = INF;
                for (int j = 1; j < m; j++){
                    int u = order[j];
                    dpCycleB[j][1] = weight1[u] + min(dpCycleB[j-1][0], dpCycleB[j-1][1]);
                    dpCycleB[j][0] = weight0[u] + dpCycleB[j-1][1];
                }
                ll candB = dpCycleB[m-1][1];
                cycCost = min(candA, candB);
            }
        }
        ans += cycCost;
        cycleProcessed[cid] = true;
    }

    cout << ans << "\n";
    return 0;
}
