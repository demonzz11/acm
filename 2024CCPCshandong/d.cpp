#include <bits/stdc++.h>
using namespace std;
using i128 = __int128;
#define int long long

void go() {
	int p, a, b, q, c, d, m, t;
	cin >> p >> a >> b >> q >> c >> d >> m >> t;

	while (1) {
		if (m < p || t < a + b + c + d) {
			break;
		}

		int x = min(m / p, (t - b - d) / (a + c));
		if (x <= 0) break;

		int get = (q - p) * x; // 每一轮赚的总钱数
		int tt = (a + c) * x + b + d; // 每一轮花的总时间

		//计算x到达x+1需要多少轮
		int rd = 1;
		i128 next_m = (i128)(x + 1) * p;
		rd = ((next_m - m + get - 1) / get);

		rd = min(rd, t/tt);

		if (rd <= 0) rd = 1;

		t -= rd * tt;
		m += (int)((i128)rd * get);

	}
	cout << m << '\n';
}

signed main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	int T;
	std::cin >> T;
	while (T--) {
		go();
	}
	return 0;
}
