#include <bits/stdc++.h>
using namespace std;
int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  vector<string> s(8);
  vector<int> a(8);
  for (int i = 0; i < 8; ++i)
    cin >> s[i] >> a[i];
  int x = int(max_element(a.begin(), a.begin() + 4) - a.begin());
  int y = int(max_element(a.begin() + 4, a.end()) - a.begin());
  if (a[x] < a[y])
    swap(x, y);
  cout << s[x] << " beats " << s[y] << '\n';
}
