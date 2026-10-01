#include <bits/stdc++.h>
using namespace std;
using i64 = long long;

void go() {
	int n, a, b, c;
	i64 ans = INT_MIN;
	cin >> n >> a >> b >> c;
	map<int, int>mp;
	vector<array<int, 6>>v(n);
	for (int i = 0; i < n; i++) {
		for (auto&x : v[i]) {
			cin >> x;
		}
		auto &[x, y, z, X, Y, Z] = v[i];
		mp[x] = mp[y] = mp[z] = mp[X] = mp[Y] = mp[Z] = 0;
		if (x > X)swap(x, X);
		if (y > Y)swap(y, Y);
		if (z > Z)swap(z, Z);
	}
	int sz = 0;
	for (auto&[x, c] : mp) {
		c = ++sz;
	}
	vector<int>dx(sz + 2), dy(sz + 2), dz(sz + 2);
	for (int i = 0; i < n; i++) {
		auto [x, y, z, X, Y, Z] = v[i];
		dx[mp[x]]++, dx[mp[X] + 1]--;
		dy[mp[y]]++, dy[mp[Y] + 1]--;
		dz[mp[z]]++, dz[mp[Z] + 1]--;
	}
	for (int i = 1; i <= sz; i++) {
		dx[i] += dx[i - 1];
		dy[i] += dy[i - 1];
		dz[i] += dz[i - 1];
		ans = max({ans, 1LL * dx[i], 1LL * dz[i], 1LL * dy[i]});
	}
	cout << ans << '\n';
}

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	go();
	return 0;
}
