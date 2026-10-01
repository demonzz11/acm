#include <bits/stdc++.h>
using namespace std;
#define int long long
const int N = 3e5+10;
int f[N][20], lg[N], pre[N], suf[N], a[N];
int qr(int l, int r) {
	int s = lg[r - l + 1];
	return gcd(f[l][s], f[r - (1 << s) + 1][s]);
}

void go() {
	int n, k;
	cin >> n >> k;
	
	for (int i = 1; i <= n; i++)cin >> a[i];
	
	lg[0] = -1;
	for (int i = 1; i <= n; i++)lg[i] = lg[i >> 1] + 1;
	for (int i = 1; i <= n; i++)f[i][0] = a[i] + k;
	
	for (int j = 1; j < 20; j++) {
		for (int i = 1; i + (1 << j) -1 <= n; i++) {
			f[i][j] = gcd(f[i][j - 1], f[i + (1 << (j - 1))][j - 1]);
		}
	}
	
	pre[0] = suf[n + 1] = 0;
	for (int i = 1; i <= n; i++) {
		pre[i] = gcd(pre[i - 1], a[i]);
	}
	for (int i = n; i >= 1; i--) {
		suf[i] = gcd(suf[i + 1], a[i]);
	}
	
	vector<int>p;
	for (int i = 1; i <= n; i++) {
		if (pre[i] != pre[i - 1]) {
			p.push_back(i);
			if (i - 1 >= 1)
				p.push_back(i - 1);
		}
		if (suf[i] != suf[i + 1]) {
			p.push_back(i);
			if (i + 1 <= n)
				p.push_back(i + 1);
		}
	}
	sort(p.begin(), p.end());
	p.erase(unique(p.begin(), p.end()), p.end());
	int ans = pre[n];
	
	for (int i = 0; i < p.size(); i++) {
		for (int j = i; j < p.size(); j++) {
			int l = p[i], r = p[j];
			ans = max(ans, gcd(gcd(qr(l, r), pre[l - 1]), suf[r + 1]));
		}
	}
	cout << ans << '\n';
}

signed main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	int t;
	std::cin >> t;
	while (t--) {
		go();
	}
	return 0;
}
