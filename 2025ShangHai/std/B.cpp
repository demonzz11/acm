#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T; cin >> T;
    while (T--) {
        int n, m, s; cin >> n >> m >> s;
        vector<int> parity(n + 1);
        for (int i = 1, x; i <= n; ++i) { cin >> x; parity[i] = x & 1; }
        vector<vector<int>> graph(n + 1), children(n + 1);
        for (int i = 0, u, v; i < m; ++i) {
            cin >> u >> v;
            graph[u].push_back(v); graph[v].push_back(u);
        }
        vector<int> parent(n + 1, -1), depth(n + 1), ptr(n + 1), stack{s};
        parent[s] = 0;
        int cycle_u = -1, cycle_v = -1;
        while (!stack.empty()) {
            int u = stack.back();
            if (ptr[u] == int(graph[u].size())) { stack.pop_back(); continue; }
            int v = graph[u][ptr[u]++];
            if (parent[v] == -1) {
                parent[v] = u; depth[v] = depth[u] + 1;
                children[u].push_back(v); stack.push_back(v);
            } else if ((depth[u] & 1) == (depth[v] & 1) && depth[v] <= depth[u]) {
                cycle_u = u; cycle_v = v; // DFS back edge (possibly a loop)
            }
        }
        bool ok = true;
        int total = 0;
        for (int v = 1; v <= n; ++v) {
            if (parent[v] == -1 && parity[v]) ok = false;
            if (parent[v] != -1) total ^= parity[v];
        }
        if (total && cycle_u == -1) ok = false;
        if (!ok) { cout << "No\n"; continue; }
        vector<int> route;
        route.reserve(5 * n);
        auto visit = [&](int v) { route.push_back(v); parity[v] ^= 1; };
        fill(ptr.begin(), ptr.end(), 0);
        stack = {s};
        auto enter = [&](int u) {
            if (!total || u != cycle_u) return;
            visit(cycle_v);
            vector<int> path;
            for (int v = cycle_u; v != cycle_v; v = parent[v]) path.push_back(v);
            reverse(path.begin(), path.end());
            for (int v : path) visit(v);
        };
        enter(s);
        while (!stack.empty()) {
            int u = stack.back();
            if (ptr[u] < int(children[u].size())) {
                int v = children[u][ptr[u]++];
                visit(v); stack.push_back(v); enter(v);
            } else {
                stack.pop_back();
                if (u == s) break;
                int p = parent[u];
                visit(p);
                if (parity[u]) { visit(u); visit(p); }
            }
        }
        cout << "Yes\n" << route.size() << '\n';
        for (int v : route) cout << v << ' ';
        cout << '\n';
    }
}
