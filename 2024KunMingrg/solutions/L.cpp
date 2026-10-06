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
        vector<long long> own(n), enemy(m);
        long long attacks = 0;
        bool has_one = false;
        for (auto &h : own) {
            cin >> h;
            if (h > 1) ++attacks;
            else has_one = true;
        }
        if (has_one) ++attacks;
        for (auto &h : enemy) cin >> h;
        sort(own.begin(), own.end());
        sort(enemy.begin(), enemy.end());

        long long damage = 0;
        int next_own = 0;
        bool possible = true;
        for (long long health : enemy) {
            // A friendly attack lowers its own initial Health by one.
            while (next_own < n && own[next_own] <= damage + 1) {
                ++next_own;
                ++damage;
            }
            long long needed = max(0LL, health - damage);
            if (needed > attacks) {
                possible = false;
                break;
            }
            attacks -= needed;
            ++damage; // This enemy dies and explodes once.
        }
        cout << (possible ? "Yes" : "No") << '\n';
    }
}
