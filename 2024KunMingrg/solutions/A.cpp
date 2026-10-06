#include <bits/stdc++.h>
using namespace std;

using int64 = long long;
constexpr int64 INF = (1LL << 60);

// Lengauer-Tarjan with iterative DFS and iterative path compression.
vector<int> dominator_parents(const vector<vector<int>> &graph) {
    int n = int(graph.size()) - 1;
    vector<int> number(n + 1), vertex(n + 1), parent(n + 1);
    vector<size_t> next_edge(n + 1);
    vector<int> stack{1};
    int count = 1;
    number[1] = 1;
    vertex[1] = 1;
    while (!stack.empty()) {
        int u = stack.back();
        if (next_edge[u] == graph[u].size()) {
            stack.pop_back();
            continue;
        }
        int v = graph[u][next_edge[u]++];
        if (number[v]) continue;
        number[v] = ++count;
        vertex[count] = v;
        parent[count] = number[u];
        stack.push_back(v);
    }
    assert(count == n);

    vector<vector<int>> predecessors(n + 1), bucket(n + 1);
    for (int u = 1; u <= n; ++u) {
        for (int v : graph[u]) {
            predecessors[number[v]].push_back(number[u]);
        }
    }
    vector<int> semi(n + 1), label(n + 1), ancestor(n + 1), idom(n + 1);
    iota(semi.begin(), semi.end(), 0);
    iota(label.begin(), label.end(), 0);
    vector<int> path;
    path.reserve(n);
    auto evaluate = [&](int v) {
        if (ancestor[v] == 0) return label[v];
        path.clear();
        int x = v;
        while (ancestor[ancestor[x]] != 0) {
            path.push_back(x);
            x = ancestor[x];
        }
        while (!path.empty()) {
            x = path.back();
            path.pop_back();
            int a = ancestor[x];
            if (semi[label[a]] < semi[label[x]]) label[x] = label[a];
            ancestor[x] = ancestor[a];
        }
        return label[v];
    };
    for (int u = n; u >= 2; --u) {
        for (int v : predecessors[u]) {
            semi[u] = min(semi[u], semi[evaluate(v)]);
        }
        bucket[semi[u]].push_back(u);
        ancestor[u] = parent[u];
        for (int v : bucket[parent[u]]) {
            int x = evaluate(v);
            idom[v] = semi[x] < semi[v] ? x : parent[u];
        }
        bucket[parent[u]].clear();
    }
    for (int u = 2; u <= n; ++u) {
        if (idom[u] != semi[u]) idom[u] = idom[idom[u]];
    }
    vector<int> answer(n + 1);
    for (int u = 2; u <= n; ++u) answer[vertex[u]] = vertex[idom[u]];
    return answer;
}

class SegmentTree {
    struct Node {
        int64 minimum = 0, cheapest = 0, add = 0, cap = INF;
    };
    int n;
    vector<Node> tree;

    void build(int p, int l, int r, const vector<int64> &cost) {
        if (l == r) {
            tree[p].minimum = tree[p].cheapest = cost[l];
            return;
        }
        int mid = (l + r) / 2;
        build(p * 2, l, mid, cost);
        build(p * 2 + 1, mid + 1, r, cost);
        tree[p].cheapest = min(tree[p * 2].cheapest, tree[p * 2 + 1].cheapest);
        pull(p);
    }
    void pull(int p) {
        tree[p].minimum = min(tree[p * 2].minimum, tree[p * 2 + 1].minimum);
    }
    void apply_add(int p, int64 value) {
        tree[p].minimum += value;
        tree[p].add += value;
        if (tree[p].cap != INF) tree[p].cap += value;
    }
    void apply_cap(int p, int64 base) {
        tree[p].minimum = min(tree[p].minimum, base + tree[p].cheapest);
        tree[p].cap = min(tree[p].cap, base);
    }
    void push(int p) {
        if (tree[p].add != 0) {
            apply_add(p * 2, tree[p].add);
            apply_add(p * 2 + 1, tree[p].add);
            tree[p].add = 0;
        }
        if (tree[p].cap != INF) {
            apply_cap(p * 2, tree[p].cap);
            apply_cap(p * 2 + 1, tree[p].cap);
            tree[p].cap = INF;
        }
    }
    void range_add(int p, int l, int r, int ql, int qr, int64 value) {
        if (ql <= l && r <= qr) {
            apply_add(p, value);
            return;
        }
        push(p);
        int mid = (l + r) / 2;
        if (ql <= mid) range_add(p * 2, l, mid, ql, qr, value);
        if (qr > mid) range_add(p * 2 + 1, mid + 1, r, ql, qr, value);
        pull(p);
    }

public:
    explicit SegmentTree(const vector<int64> &cost)
        : n(int(cost.size()) - 1), tree(4 * n + 4) {
        build(1, 1, n, cost);
    }
    int64 minimum() const { return tree[1].minimum; }
    void switch_filter(int64 base) { apply_cap(1, base); }
    void add_everywhere(int64 value) { apply_add(1, value); }
    void add(int l, int r, int64 value) { range_add(1, 1, n, l, r, value); }
};

void solve() {
    int n, m, q;
    cin >> n >> m >> q;
    vector<vector<int>> reversed(n + 1);
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        reversed[v].push_back(u);
    }
    vector<int64> cost(n + 1);
    for (int u = 1; u <= n; ++u) cin >> cost[u];
    vector<int> parent = dominator_parents(reversed);
    vector<vector<int>> children(n + 1);
    for (int u = 2; u <= n; ++u) children[parent[u]].push_back(u);

    vector<int> order{1}, size(n + 1, 1), heavy(n + 1), head(n + 1), position(n + 1);
    for (size_t i = 0; i < order.size(); ++i) {
        for (int v : children[order[i]]) order.push_back(v);
    }
    for (int i = n - 1; i >= 1; --i) {
        int u = order[i], p = parent[u];
        size[p] += size[u];
        if (heavy[p] == 0 || size[u] > size[heavy[p]]) heavy[p] = u;
    }
    vector<pair<int, int>> pending{{1, 1}};
    vector<int64> linear_cost(n + 1);
    int timer = 0;
    while (!pending.empty()) {
        auto [start, chain_head] = pending.back();
        pending.pop_back();
        for (int u = start; u != 0; u = heavy[u]) {
            head[u] = chain_head;
            position[u] = ++timer;
            linear_cost[timer] = cost[u];
            for (int v : children[u]) {
                if (v != heavy[u]) pending.emplace_back(v, v);
            }
        }
    }

    SegmentTree segment(linear_cost);
    int64 no_filter = 0;
    for (int day = 0; day < q; ++day) {
        int a;
        int64 loss;
        cin >> a >> loss;
        int64 previous_best = min(no_filter, segment.minimum());
        segment.switch_filter(previous_best);
        segment.add_everywhere(loss);
        // Exactly the dominators of a prevent this day's loss.
        while (a != 0) {
            segment.add(position[head[a]], position[a], -loss);
            a = parent[head[a]];
        }
        no_filter += loss;
        if (day) cout << ' ';
        cout << min(no_filter, segment.minimum());
    }
    cout << '\n';
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    while (t--) solve();
}
