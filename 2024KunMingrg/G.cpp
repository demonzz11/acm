#include <bits/stdc++.h>
using namespace std;
#define int long long
using Matrix = vector<vector<int>>;
std::vector<int> minp, primes, phi, mu, omega, Omega;

// 重置并线性筛出 [1,n] 的相关函数值。
void sieve(int n) {
  minp.assign(n + 1, 0);
  phi.assign(n + 1, 0);
  mu.assign(n + 1, 0);
  omega.assign(n + 1, 0);
  Omega.assign(n + 1, 0);
  primes.clear();
  if (n >= 1) {
    phi[1] = 1;
    mu[1] = 1;
    omega[1] = 0;
    Omega[1] = 0;
  }
  for (int i = 2; i <= n; i++) {
    if (minp[i] == 0) {
      minp[i] = i;
      phi[i] = i - 1;
      mu[i] = -1;
      omega[i] = 1;
      Omega[i] = 1;
      primes.push_back(i);
    }
    for (auto p : primes) {
      if (i * p > n) {
        break;
      }
      minp[i * p] = p;
      if (p == minp[i]) {
        phi[i * p] = phi[i] * p;
        omega[i * p] = omega[i];
        Omega[i * p] = Omega[i] + 1;
        break;
      }
      phi[i * p] = phi[i] * (p - 1);
      mu[i * p] = -mu[i];
      omega[i * p] = omega[i] + 1;
      Omega[i * p] = Omega[i] + 1;
    }
  }
}

// 返回 x 是否为质数，只允许查询筛内下标。
bool isprime(int n) { return n >= 2 && minp[n] == n; }

// 返回单个正整数的欧拉函数值，不依赖筛。
int phiSingle(int n) {
  int res = n;
  for (int i = 2; i * i <= n; i++) {
    if (n % i == 0) {
      res = res / i * (i - 1);
      while (n % i == 0) {
        n /= i;
      }
    }
  }
  if (n > 1) {
    res = res / n * (n - 1);
  }
  return res;
}

struct dot {
  int x, y, cnt;
  bool operator<(const dot &b) const { return cnt > b.cnt; }
};
Matrix nums(5005);

void solve() {
  int a, b;
  cin >> a >> b;
  int g = gcd(a, b);
  priority_queue<dot> q;
  q.push({a / g, b / g, 0});
  int ans = 0;
  while (!q.empty()) {
    int x = q.top().x, y = q.top().y, cnt = q.top().cnt;
    if (x > y)
      swap(x, y);
    q.pop();

    int cur_g = gcd(x, y);
    cnt++;
    if (x == 0) {
      ans = cnt;
      break;
    }
    x /= cur_g, y /= cur_g;

    //        for(int num:nums[x])
    //        {
    //            min_res=min(y%num,min_res);
    //        }
    q.push({x - 1, y, cnt});
    //        if(min_res!=1e9 && min_res!=0) q.push({x,y-min_res,cnt-1+min_res});
    q.push({x, y - 1, cnt});
  }

  cout << ans << "\n";
}

signed main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  sieve(5005);
  for (int i = 1; i <= 5000; i++) {
    for (int j = 2; j <= i; j++) {
      if (isprime(j) && i % j == 0)
        nums[i].push_back(j);
    }
  }

  bool ff = true;
  if (ff) {
    int t;
    cin >> t;
    while (t--) {
      solve();
    }
  } else
    solve();
  return 0;
}
