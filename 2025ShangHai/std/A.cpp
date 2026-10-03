#include <bits/stdc++.h>
using namespace std;
int ask(int u, int radius) {
    cout << "? " << u << ' ' << radius << endl;
    int response;
    if (!(cin >> response) || response < 0) exit(0);
    return response;
}
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T; cin >> T;
    while (T--) {
        int n; cin >> n;
        vector<vector<int>> children(n + 1);
        vector<int> depth(n + 1), parent(n + 1);
        int max_depth = 0;
        for (int v = 2; v <= n; ++v) {
            cin >> parent[v];
            children[parent[v]].push_back(v);
            depth[v] = depth[parent[v]] + 1;
            max_depth = max(max_depth, depth[v]);
        }
        int lo = 0, hi = max_depth;
        while (lo < hi) {
            int mid = (lo + hi) / 2;
            if (ask(1, mid)) hi = mid;
            else lo = mid + 1;
        }
        int target_depth = lo;
        vector<char> possible(n + 1);
        int count = 0;
        for (int v = 1; v <= n; ++v)
            count += possible[v] = (depth[v] == target_depth);
        vector<int> weight(n + 1);
        while (count > 1) {
            for (int v = 1; v <= n; ++v) weight[v] = possible[v];
            for (int v = n; v >= 2; --v) weight[parent[v]] += weight[v];
            int v = 1;
            while (true) {
                int next = -1;
                for (int u : children[v]) if (3 * weight[u] > 2 * count) next = u;
                if (next == -1) break;
                v = next;
            }
            int u = children[v][0];
            for (int child : children[v]) if (weight[child] > weight[u]) u = child;
            int response = ask(u, target_depth - depth[u]);
            vector<char> in_subtree(n + 1);
            in_subtree[u] = true;
            for (int x = u + 1; x <= n; ++x) in_subtree[x] = in_subtree[parent[x]];
            count = 0;
            for (int x = 1; x <= n; ++x) {
                possible[x] = possible[x] && (in_subtree[x] == response);
                count += possible[x];
            }
        }
        int answer = 1;
        while (!possible[answer]) ++answer;
        cout << "! " << answer << endl;
    }
}
