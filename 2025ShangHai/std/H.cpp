#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T; cin >> T;
    while (T--) {
        int n; cin >> n;
        vector<int> a(2 * n), odd;
        for (int& x : a) cin >> x;
        sort(a.begin(), a.end());
        int paired_xor = 0;
        for (int l = 0, r; l < 2 * n; l = r) {
            r = l + 1;
            while (r < 2 * n && a[r] == a[l]) ++r;
            int count = r - l;
            if ((count / 2) & 1) paired_xor ^= a[l];
            if (count & 1) odd.push_back(a[l]);
        }
        bool win = odd.empty() ? paired_xor == 0 :
            odd.size() == 2 && (odd[0] == paired_xor || odd[1] == paired_xor);
        cout << (win ? "Menji" : "Bot") << '\n';
    }
}
