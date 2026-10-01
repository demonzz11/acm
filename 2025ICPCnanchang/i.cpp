#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
const int N = 2e5+10;
const int mod = 998244353;
i64 len[N];
i64 inv[N];
i64 fac[N];
i64 qpow(i64 a, i64 b) {
	i64 ans = 1;
	while (b) {
		if (b & 1) {
			ans = ans * a % mod;
		}
		a = a * a % mod;
		b >>= 1;
	}
	return ans;
}
void init() {
	fac[0] = 1;
	for (int i = 1; i < N; i++) {
		fac[i] = fac[i - 1] * i % mod;
	}
	inv[N - 1] = qpow(fac[N - 1], mod - 2);
	for (int i = N - 2; i >= 0; i--) {
		inv[i] = inv[i + 1] * (i + 1) % mod;
	}
}
i64 C(i64 a, i64 b) {
	return fac[a] * inv[b] % mod * inv[a - b] % mod;
}
void go() {
	int n, k;
	cin >> n >> k;
	string s;
	cin >> s;
	s = '#' + s;
	int cnt = 0;
	for (int i = 1; i <= n; i++) {
		if (s[i] == '1')len[++cnt] = i;
	}
	len[++cnt] = n + 1;
	i64 ans = 0;
	for (int i = k; i < cnt; i++) {
		int l = len[i - k] + 1, r = len[i + 1] - 1;
		ans = (ans + C(r - l + 1, k)) % mod;
		if (i > k) {
			l = len[i - k] + 1;
			r = len[i] - 1;
			ans = (mod + ans - C(r - l + 1, k - 1)) % mod;
		}
	}
	cout << ans << '\n';
}

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	init();
	int t;
	std::cin >> t;
	while (t--) {
		go();
	}
	return 0;
}
