#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128;
constexpr int N = 1010;

int lb(int x) { return x & -x; }
void go() {
  int n;
  cin >> n;
  if (n == 4) {
    cout << -1 << '\n';
    return;
  }
  if (n & 1) {
    for (int i = 2; i <= n; i++) {
      if (i & 1) {
        cout << i - 2 << " " << i << "\n";
      } else {
        cout << i - 1 << " " << i << "\n";
      }
    }
  } else {
    for (int i = 2; i <= n - 2; i++) {
      if (i & 1) {
        cout << i - 2 << " " << i << "\n";
      } else {
        cout << i - 1 << " " << i << "\n";
      }
    }
    cout << 2 << " " << n - 1 << '\n';
    cout << 2 << " " << n - 2 << '\n';
  }
}

int main() {
  ios::sync_with_stdio(0);
  cin.tie(0);
  int t;
  cin >> t;
  while (t--)
    go();
}
