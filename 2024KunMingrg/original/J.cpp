#include <bits/stdc++.h>
using namespace std;
using i64 = long long;

void solve() {
  int cnt = 0;
  int x, n;
  string s;
  cin >> n;
  cin >> s;
  for (int i = 1; i <= n; i++) {
    cin >> x;
    cnt += (x != i);
  }

  if (n == 2) {
    cout << "Alice" << "\n";
    return;
  }

  if (s == "Alice") {
    if (cnt == 0 || cnt == 2) {
      cout << "Alice" << '\n';
    } else {
      cout << "Bob" << "\n";
    }
  } else {
    if (n == 3) {
      if (cnt == 2) {
        cout << "Bob" << '\n';
      } else {
        cout << "Alice" << "\n";
      }
    } else {
      if (cnt == 0) {
        cout << "Alice" << '\n';
      } else {
        cout << "Bob" << "\n";
      }
    }
  }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--)
    solve();
  return 0;
}
