#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    while (T--) {
        int n;
        string first;
        cin >> n >> first;

        int wrong = 0;
        for (int i = 1; i <= n; ++i) {
            int p;
            cin >> p;
            wrong += (p != i);
        }

        bool alice_wins;
        if (n == 2) {
            alice_wins = true;
        } else if (first == "Alice") {
            alice_wins = (wrong == 2);
        } else {
            alice_wins = (n == 3 && wrong == 3);
        }
        cout << (alice_wins ? "Alice" : "Bob") << '\n';
    }
    return 0;
}
