#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    cin >> T;
    while (T--) {
        int n, m;
        cin >> n >> m;
        vector<vector<int>> a(n, vector<int>(m));
        int value = 0;
        for (int diagonal = 0; diagonal < n + m - 1; ++diagonal) {
            int first_row = max(0, diagonal - (m - 1));
            int last_row = min(n - 1, diagonal);
            for (int row = first_row; row <= last_row; ++row) {
                a[row][diagonal - row] = ++value;
            }
        }
        cout << "Yes\n";
        for (const auto &row : a) {
            for (int j = 0; j < m; ++j) {
                if (j) cout << ' ';
                cout << row[j];
            }
            cout << '\n';
        }
    }
}
