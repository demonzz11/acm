#include <bits/stdc++.h>
using namespace std;

struct Query { int left, right; };
struct Record { int node, length, side; };

int bracket_type(char c) {
    if (c == '(' || c == ')') return 1;
    if (c == '[' || c == ']') return 2;
    if (c == '{' || c == '}') return 3;
    return 4;
}

bool opening(char c) {
    return c == '(' || c == '[' || c == '{' || c == '<';
}

char opposite(char c) {
    const string alphabet = "()[]{}<>";
    return alphabet[alphabet.find(c) ^ 1];
}

class Solver {
    vector<int> parent{0}, depth{0}, type{0}, position{0};
    vector<Record> records;
    int empty_count = 0;

    void collect(const string &s, const vector<Query> &queries, int side) {
        const int n = int(s.size());
        const int m = int(queries.size());
        vector<int> head(n + 1, -1), next(m), prefix(n + 1);
        for (int i = 0; i < m; ++i) {
            const int end = side == 0 ? queries[i].right : n - queries[i].left + 1;
            next[i] = head[end];
            head[end] = i;
        }

        // Erasing a start position means it can never form a valid prefix
        // ending at the current position or at any later position.
        vector<int> successor(n + 2);
        iota(successor.begin(), successor.end(), 0);
        vector<unsigned char> erased(n + 2, 0);
        auto find = [&](int x) {
            int root = x;
            while (successor[root] != root) root = successor[root];
            while (successor[x] != x) {
                const int next_x = successor[x];
                successor[x] = root;
                x = next_x;
            }
            return root;
        };

        int top = 0;
        for (int i = 1; i <= n; ++i) {
            const char c = s[i - 1];
            prefix[i] = prefix[i - 1] + (opening(c) ? 1 : -1);
            if (opening(c)) {
                parent.push_back(top);
                depth.push_back(depth[top] + 1);
                type.push_back(bracket_type(c));
                position.push_back(i);
                top = int(parent.size()) - 1;
            } else {
                int matched = 0;
                if (top != 0 && type[top] == bracket_type(c)) {
                    matched = position[top];
                    top = parent[top];
                } else {
                    top = 0;
                }
                for (int start = find(matched + 1); start <= i; start = find(start)) {
                    erased[start] = 1;
                    successor[start] = find(start + 1);
                }
            }

            for (int id = head[i]; id != -1; id = next[id]) {
                const int left = side == 0 ? queries[id].left : n - queries[id].right + 1;
                if (erased[left]) continue;
                const int length = prefix[i] - prefix[left - 1];
                if (length == 0) {
                    if (side == 0) ++empty_count;
                } else {
                    records.push_back({top, length, side});
                }
            }
        }
    }

public:
    int solve(const string &s, const vector<Query> &queries) {
        const int n = int(s.size());
        parent.reserve(n + 1);
        depth.reserve(n + 1);
        type.reserve(n + 1);
        position.reserve(n + 1);
        records.reserve(queries.size());
        collect(s, queries, 0);
        string reflected(s.rbegin(), s.rend());
        for (char &c : reflected) c = opposite(c);
        collect(reflected, queries, 1);
        if (records.empty()) return empty_count / 2;

        // Every opening bracket creates one persistent-stack node. Across
        // the original and reflected strings there are exactly n nodes.
        const int nodes = int(parent.size());
        const int maximum_depth = *max_element(depth.begin(), depth.end());
        int levels = 1;
        while ((1 << (levels - 1)) < maximum_depth) ++levels;
        vector<vector<int>> up(levels, vector<int>(nodes));
        vector<vector<int>> rank(levels, vector<int>(nodes));
        up[0] = parent;
        rank[0] = type;
        vector<int> order(nodes), temporary(nodes), count(nodes + 5);
        iota(order.begin(), order.end(), 0);
        int classes = 4;

        // Deterministic doubling ranks for words read from a node upwards.
        // Stable counting sorts compare the second half, then the first.
        for (int h = 1; h < levels; ++h) {
            for (int v = 0; v < nodes; ++v) {
                up[h][v] = up[h - 1][up[h - 1][v]];
            }
            fill(count.begin(), count.begin() + classes + 1, 0);
            for (int v : order) ++count[rank[h - 1][up[h - 1][v]]];
            int total = 0;
            for (int c = 0; c <= classes; ++c) {
                const int frequency = count[c];
                count[c] = total;
                total += frequency;
            }
            for (int v : order) temporary[count[rank[h - 1][up[h - 1][v]]]++] = v;

            fill(count.begin(), count.begin() + classes + 1, 0);
            for (int v : temporary) ++count[rank[h - 1][v]];
            total = 0;
            for (int c = 0; c <= classes; ++c) {
                const int frequency = count[c];
                count[c] = total;
                total += frequency;
            }
            for (int v : temporary) order[count[rank[h - 1][v]]++] = v;

            int new_classes = 0;
            rank[h][order[0]] = 0;
            for (int j = 1; j < nodes; ++j) {
                const int x = order[j - 1], y = order[j];
                if (rank[h - 1][x] != rank[h - 1][y] ||
                    rank[h - 1][up[h - 1][x]] != rank[h - 1][up[h - 1][y]]) {
                    ++new_classes;
                }
                rank[h][y] = new_classes;
            }
            classes = new_classes;
        }

        const auto &full_rank = rank.back();
        sort(records.begin(), records.end(), [&](const Record &a, const Record &b) {
            if (a.length != b.length) return a.length < b.length;
            return full_rank[a.node] < full_rank[b.node];
        });
        auto equal_prefix = [&](int x, int y, int length) {
            if (full_rank[x] == full_rank[y]) return true;
            for (int h = levels - 1; h >= 0; --h) {
                if ((length >> h) & 1) {
                    if (rank[h][x] != rank[h][y]) return false;
                    x = up[h][x];
                    y = up[h][y];
                }
            }
            return true;
        };

        int answer = empty_count / 2;
        int left_count = 0, right_count = 0;
        for (int i = 0; i < int(records.size()); ++i) {
            if (i != 0 && (records[i].length != records[i - 1].length ||
                !equal_prefix(records[i].node, records[i - 1].node, records[i].length))) {
                answer += min(left_count, right_count);
                left_count = right_count = 0;
            }
            if (records[i].side == 0) ++left_count;
            else ++right_count;
        }
        return answer + min(left_count, right_count);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    while (t--) {
        int n, m;
        string s;
        cin >> n >> m >> s;
        vector<Query> queries(m);
        for (auto &[left, right] : queries) cin >> left >> right;
        cout << Solver().solve(s, queries) << '\n';
    }
}
