#include<bits/stdc++.h>
#define int long long
using namespace std;

int cal(int i) {
	double t = pow(2 * i, 0.5);
	return i + (int)t + 1.5;
}


void go() {
	int x, y;
	cin >> x >> y;

	if (y <= x) {
		cout << x - y << '\n';
		return;
	}

	auto  h = [](int v) ->int{
		int l = 1, r = 2e9;
		int ans = 0;
		while (l <= r) {
			int m = l + (r - l) / 2;
			if (m * (1 + m) / 2 >= v) {
				r = m - 1;
				ans = m;
			} else {
				l = m + 1;
			}
		}
		return ans;
	};

	auto f = [&](int x)-> int {
		int rx = h(x);
		int d = rx * (rx + 1) / 2 - x;
		int ry = h(y);

		int res = ry - rx;

		int x2 = ry * (ry + 1) / 2 - d;
		if (x2 < y) {
			x2 = cal(x2);
			res++;
		}
		res += (x2 - y);
		return res;
	};
	int ans = f(x);

	if (h(x) > 1) {
		int ph = h(x) -1;
		int now = ph * (1 + ph) / 2;
		ans = min(ans, f(now) + x - now);
	}

	cout << ans << '\n';
}

signed main() {
	ios::sync_with_stdio(0);
	cin.tie(0);

	int t = 1;
	cin >> t;
	while (t--) go();

	return 0;
}

