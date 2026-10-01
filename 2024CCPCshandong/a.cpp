#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128;
#define int long long
void go() {
	int n, k;
	cin >> n >> k;

	vector<std::tuple<int, int, int>>a(n);
	for (auto&[x, y, z] : a) {
		cin >> x >> y >> z;
	}
	auto ck = [&](i64 ss) ->bool{
		i64 cnt = 0;
		for (int i = 0; i < n; i++) {
			auto [x, y, z] = a[i];
			i64 cur = ss / (x * y + z) * y;
			i64 md = ss % (x * y + z);
			cnt = cnt + cur;
			cnt += min(md / x, y);
			if (cnt >= k)return 1;
		}
		return 0;
	};
	i64 l = 0, r = LONG_LONG_MAX, ans = 0;
	while (l <= r) {
		i64 mid = l + r >> 1;
		if (ck(mid)) {
			ans = mid;
			r = mid - 1;
		} else {
			l = mid + 1;
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
