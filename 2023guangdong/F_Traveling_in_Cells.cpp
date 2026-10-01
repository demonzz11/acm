#include <bits/stdc++.h>
using namespace std;

constexpr int N = 1e5 + 10, MAX = 1e5;

int T[N], tot;
struct node {
  int ls, rs, sum;
} tr[N * 200];

void update(int &p, int x, int k, int l = 1, int r = MAX) {
  if (!p)
    p = tot++;
  tr[p].sum += k;
  if (l == r)
    return;
  int mid = (l + r) >> 1;
  if (x <= mid)
    update(tr[p].ls, x, k, l, mid);
  else
    update(tr[p].rs, x, k, mid + 1, r);
}

void go() {}

int main() {
  ios::sync_with_stdio(0);
  cin.tie(0);
  int t;
  cin >> t;
  while (t--) {
    go();
  }
}