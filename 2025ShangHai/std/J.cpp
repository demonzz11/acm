#include <bits/stdc++.h>
using namespace std;
struct Edge { int to, weight, next; };
struct Item { int edge, next; };
struct Frame {
    array<int, 8> head;
    int next_weight = 0;
    Frame() { head.fill(-1); }
};
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m, k; cin >> n >> m >> k;
    vector<vector<pair<int, int>>> out(n);
    for (int i = 0, u, v, w; i < m; ++i) {
        cin >> u >> v >> w; out[u - 1].push_back({w - 1, v - 1});
    }
    vector<Edge> edges;
    edges.reserve(m);
    vector<int> first(n, -1);
    for (int v = 0; v < n; ++v) {
        sort(out[v].begin(), out[v].end());
        if (!out[v].empty()) first[v] = int(edges.size());
        for (auto [w, u] : out[v]) {
            int next = int(edges.size()) + 1;
            edges.push_back({u, w, next});
        }
        if (!out[v].empty()) edges.back().next = -1;
    }
    vector<vector<pair<int, int>>>().swap(out);
    vector<Item> items;
    items.reserve(n + k);
    int free_head = -1;
    auto add = [&](Frame& frame, int e) {
        if (e == -1) return;
        int id;
        if (free_head == -1) { id = int(items.size()); items.push_back({}); }
        else { id = free_head; free_head = items[id].next; }
        int w = edges[e].weight;
        items[id] = {e, frame.head[w]}; frame.head[w] = id;
    };
    vector<Frame> stack(1);
    stack.reserve(k + 1);
    for (int v = 0; v < n; ++v) add(stack[0], first[v]);
    int emitted = 0;
    while (!stack.empty() && emitted < k) {
        Frame& parent = stack.back();
        while (parent.next_weight < 8 && parent.head[parent.next_weight] == -1)
            ++parent.next_weight;
        if (parent.next_weight == 8) { stack.pop_back(); continue; }
        int w = parent.next_weight;
        Frame child;
        // Emit all paths with this same weight sequence before extending them.
        while (parent.head[w] != -1 && emitted < k) {
            int id = parent.head[w], e = items[id].edge;
            parent.head[w] = items[id].next;
            items[id].next = free_head; free_head = id;
            cout << stack.size() << '\n'; ++emitted;
            add(parent, edges[e].next); // next sibling, generated lazily
            add(child, first[edges[e].to]); // smallest continuation
        }
        ++parent.next_weight;
        if (emitted < k) stack.push_back(child);
    }
    while (emitted++ < k) cout << -1 << '\n';
}
