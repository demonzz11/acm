#include<bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128;

const int N = 5e5+10;
//const int N = 10;

i64 a[N], d[N];
void go() {
	i64 n;
	cin >> n;
	bool ok = false;

	// 偏移出现的位置
	map<i64, pair<i64, i64>>mp;

	for (int i = 1; i <= n; i++) {
		cin >> a[i];
		if (a[i] < i) {
			ok = true;
		}
		d[i] = a[i] - i;
	}
	if (ok) {
		cout << -1 << '\n';
		return;
	}
	i64 ans = 0;
	for (int i = 1; i <= n; i++) {
		if (mp[d[i]].first == 0) {
			mp[d[i]].first = i;
		}
		mp[d[i]].second = i;
	}
	i64 ls_r = -1;
	i64 ls_x = 0;
	int now = 0;
	for (auto&[x, w] : mp) {
		auto [l, r] = w;
		l = ls_r + 1;
		if (l > r)
			continue;
		ans += x - now;
		d[l] = max(d[l] - x, 0LL);
		ans -= x * (r - l + 1);
		ls_r = r;
		now = x;
		ls_x = x;
	}
	i64 sum = accumulate(d + 1, d + n + 1, 0LL);
	cout << sum + ans + n << '\n';
}

int main() {
	ios::sync_with_stdio(0);
	cin.tie(0);
	int t;
	cin >> t;
	while (t--) {
		go();
	}
}
