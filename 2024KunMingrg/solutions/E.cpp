#include <bits/stdc++.h>
using namespace std;

constexpr int MAX_N = 256;
using Row = bitset<MAX_N>;

bool insert_row(Row row, int value, vector<Row> &basis, vector<int> &rhs, int n) {
    for (int pivot = n - 1; pivot >= 0; --pivot) {
        if (!row[pivot]) continue;
        if (basis[pivot].none()) {
            basis[pivot] = row;
            rhs[pivot] = value;
            return true;
        }
        row ^= basis[pivot];
        value ^= rhs[pivot];
    }
    return false;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, k;
    cin >> n >> k;
    vector<vector<int>> tree(n);
    for (int i = 1; i < n; ++i) {
        int u, v;
        cin >> u >> v;
        --u; --v;
        tree[u].push_back(v);
        tree[v].push_back(u);
    }

    vector<Row> basis(n);
    vector<int> rhs(n);
    Row root;
    root[0] = 1;
    insert_row(root, 0, basis, rhs, n);
    int rank = 1;
    vector<pair<int, int>> queries;
    vector<Row> selected_rows;

    for (int source = 0; source < n && rank < n; ++source) {
        vector<int> parent(n, -1), distance(n, -1);
        vector<Row> path(n);
        vector<int> order{source};
        distance[source] = 0;
        path[source][source] = 1;
        for (int at = 0; at < (int)order.size(); ++at) {
            int u = order[at];
            for (int v : tree[u]) {
                if (v == parent[u]) continue;
                parent[v] = u;
                distance[v] = distance[u] + 1;
                path[v] = path[u];
                path[v][v] = 1;
                order.push_back(v);
            }
        }
        for (int target = source + 1; target < n && rank < n; ++target) {
            if (distance[target] != k) continue;
            if (insert_row(path[target], 0, basis, rhs, n)) {
                ++rank;
                queries.emplace_back(source, target);
                selected_rows.push_back(path[target]);
            }
        }
    }
    if (rank < n) {
        cout << "No" << endl;
        return 0;
    }

    cout << "Yes\n? " << queries.size();
    for (auto [u, v] : queries) cout << ' ' << u + 1 << ' ' << v + 1;
    cout << endl; // All queries must be sent together before reading replies.
    vector<int> replies(queries.size());
    for (int &value : replies) {
        if (!(cin >> value) || value == -1) return 0;
    }

    fill(basis.begin(), basis.end(), Row{});
    fill(rhs.begin(), rhs.end(), 0);
    insert_row(root, 0, basis, rhs, n);
    for (int i = 0; i < (int)selected_rows.size(); ++i) {
        insert_row(selected_rows[i], replies[i], basis, rhs, n);
    }
    vector<int> weight(n);
    // Each row's pivot is its highest set bit, so lower variables are known.
    for (int pivot = 0; pivot < n; ++pivot) {
        weight[pivot] = rhs[pivot];
        for (int j = 0; j < pivot; ++j) {
            if (basis[pivot][j]) weight[pivot] ^= weight[j];
        }
    }
    cout << '!';
    for (int i = 1; i < n; ++i) cout << ' ' << weight[i];
    cout << endl;
}
