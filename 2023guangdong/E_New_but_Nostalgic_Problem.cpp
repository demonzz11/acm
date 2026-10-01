#include <bits/stdc++.h>

using LL = long long;
using namespace std;
const int N = 1e6 + 5;
int n, k, nCnt;
int tr[N][26], tot[N], sz[N];
char s[N];
int newnode() {
  nCnt++;
  tot[nCnt] = sz[nCnt] = 0;
  memset(tr[nCnt], 0, sizeof(tr[nCnt]));
  return nCnt;
}

void add() {
  int u = 1;
  tot[u]++;
  for (int i = 1; s[i]; i++) {
    int &c = tr[u][s[i] - 'a'];
    if (!c)
      c = newnode();
    tot[u = c]++;
  }
  sz[u]++;
}

void go() {
  nCnt = 0, newnode();
  cin >> n >> k;
  for (int i = 0; i < n; i++) {
    scanf("%s", s + 1);
    add();
  }
  int now = 1;
  while (true) {
    int t = sz[now];
    for (int i = 0; i < 26; i++) {
      if (tot[tr[now][i]]) {
        t++;
      }
    }
    if (t >= k) {
      if (now == 1)
        cout << "EMPTY";
      cout << '\n';
      return;
    }
    for (int i = 0; i < 26; i++) {
      if (tot[tr[now][i]]) {
        t = t - 1 + tot[tr[now][i]];
        if (t >= k) {
          k -= t - tot[tr[now][i]];
          now = tr[now][i];
          cout << (char)(i + 'a');
          break;
        }
      }
    }
  }
}

int main() {

  int t;
  cin >> t;
  while (t--)
    go();
  return 0;
}
