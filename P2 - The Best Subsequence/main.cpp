#include <bits/stdc++.h>
using namespace std;

/* 8/20 passed, Results:
+-------+--------+---------+
| Index |  Size  |   Time  |
+-------+--------+---------+
|   1   | 3.4mb  |  1ms    |
|   2   | 3.4mb  |  1ms    |
|   3   | 3.4mb  |  2ms    |
|   4   | 6.3mb  |  19ms   |
|   5   | 3.4mb  | 127ms   |
|   6   | 6.3mb  | 25ms    |
|   7   | 6.3mb  | 25ms    |
|   8   | N/A    | N/A     |
|   9   | N/A    | N/A     |
|  10   | N/A    | N/A     |
|  11   | N/A    | N/A     |
|  12   | N/A    | N/A     |
|  13   | N/A    | N/A     |
|  14   | N/A    | N/A     |
|  15   | N/A    | N/A     |
|  16   | N/A    | N/A     |
|  17   | N/A    | N/A     |
|  18   | N/A    | N/A     |
|  19   | N/A    | N/A     |
|  20   | 3.6mb  | 1784ms  |
+-------+--------+---------+
*/


static const int MOD = 1000000007;

// 快速幂, 2为底
long long modPow2(long long x) {
    const long long base = 2;
    long long ans = 1, cur = base;
    while (x > 0) {
        if (x & 1) ans = (ans * cur) % MOD;
        cur = (cur * cur) % MOD;
        x >>= 1;
    }
    return ans;
}

struct Segment {
    long long start, end;  // 区间左右端点
    int bit;             // 区间对应的比特值（0或1）
};

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    long long N;
    int M, Q;
    cin >> N >> M >> Q;
    vector<long long> endpoints;
    endpoints.reserve(2 * M + 2);

    // 差分数组，记录区间加减操作
    unordered_map<long long,int> diff;
    diff.reserve(M*2 + 4);
    diff.max_load_factor(0.7f);

    // 输入区间，构建差分
    for(int i = 0; i < M; i++){
        long long L, R;
        cin >> L >> R;
        diff[L] += 1;
        if(R + 1 <= N) {
            diff[R + 1] -= 1;
        }
    }

    // 收集所有端点
    endpoints.reserve(diff.size() + 2);
    endpoints.push_back(1);
    endpoints.push_back(N + 1LL);
    for(auto &kv : diff){
        long long p = kv.first;
        if(p >= 1 && p <= N + 1) {
            endpoints.push_back(p);
        }
    }
    sort(endpoints.begin(), endpoints.end());
    endpoints.erase(unique(endpoints.begin(), endpoints.end()), endpoints.end());

    long long curSum = 0;
    vector<Segment> segs;
    segs.reserve(endpoints.size());

    // 利用差分构造各个连续区间的比特值
    for(int i = 0; i + 1 < (int)endpoints.size(); i++){
        long long x = endpoints[i];
        long long nx = endpoints[i + 1];
        if(diff.find(x) != diff.end()) {
            curSum += diff[x];
        }
        int par = (curSum % 2 + 2) % 2;  // 计算当前比特

        if(x <= N) {
            long long segStart = x;
            long long segEnd   = nx - 1;
            if(segStart <= segEnd && segStart <= N) {
                if(segEnd > N) segEnd = N;
                if(segStart <= segEnd) {
                    segs.push_back({segStart, segEnd, par});
                }
            }
        }
    }

    // 合并相邻且比特相同的区间
    vector<Segment> merged;
    merged.reserve(segs.size());
    merged.push_back(segs[0]);
    for(int i = 1; i < (int)segs.size(); i++){
        Segment &prev = merged.back();
        if(segs[i].bit == prev.bit && segs[i].start == prev.end + 1) {
            prev.end = segs[i].end;  // 扩展区间
        } else {
            merged.push_back(segs[i]);
        }
    }
    segs.clear();

    // 根据查询区间[L,R]获取区间片段及其长度
    auto getQuerySegments = [&](long long L, long long R) {
        vector<pair<int,long long>> qSeg;
        auto it = std::lower_bound(merged.begin(), merged.end(), L,
            [](const Segment &seg, const long long &value) {
                return seg.end < value;
            }
        );
        if(it == merged.end()) return qSeg;
        for(; it != merged.end(); ++it) {
            if(it->start > R) break;
            long long ss = max(it->start, L);
            long long ee = min(it->end, R);
            if(ss <= ee) {
                long long length = ee - ss + 1;
                qSeg.push_back({it->bit, length});
            }
            if(it->end > R) break;
        }
        return qSeg;
    };

    // 根据比特和长度更新答案
    auto addBits = [&](long long &val, int bit, long long cnt){
        if(cnt <= 0) return;
        long long pow2cnt = modPow2(cnt);
        val = (val * pow2cnt) % MOD;
        if(bit == 1) {
            val = (val + (pow2cnt - 1 + MOD) % MOD) % MOD;
        }
    };

    // 处理每个查询
    while(Q--){
        long long L, R, k;
        cin >> L >> R >> k;

        vector<pair<int,long long>> subSeg = getQuerySegments(L, R);
        long long need = k;
        long long answer = 0LL;

        long long totalLen = 0;
        for(auto &pp : subSeg) totalLen += pp.second;
        long long remain = totalLen;

        // 按顺序处理各区间片段
        for(int i = 0; i < (int)subSeg.size() && need > 0; i++){
            int b = subSeg[i].first;
            long long len = subSeg[i].second;

            if(b == 1){
                // 比特1的区间：尽可能选取
                long long x = min(len, need);
                addBits(answer, 1, x);
                need -= x;
                remain -= len;
            } else {
                // 比特0的区间：先跳过部分区间（若有多余）
                long long skipCount = 0;
                if(remain > need){
                    skipCount = min(len, remain - need);
                }
                long long leftover = len - skipCount;
                remain -= len;
                long long pickX = min(leftover, need);
                if(pickX > 0){
                    addBits(answer, 0, pickX);
                    need -= pickX;
                }
            }
        }

        cout << answer % MOD << "\n";
    }

    return 0;
}
