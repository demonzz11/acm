#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
const int N = 1e6+10;
i64 t[N], c[N];
const i64 inf = 1e18;

struct itv {
	i64 l, r;
	itv (i64 L, i64 R) {
		l = L, r = R;
	}
	bool is_in(i64 X) {
		return l <= X && X <= r;
	}
	friend itv operator + (const itv& A, const itv& B) {
		return itv(min(A.l, B.l), max(A.r, B.r));
	}
};


void go() {
	int n;
	cin >> n;

	for (int i = 1; i <= n; i++) {
		cin >> t[i] >> c[i];
	}
	auto ck = [&](i64 v)->bool {
		itv cur = {-inf, inf };
		for (int i = 2; i <= n; i++) {
			i64 d = (t[i] - t[i - 1]) * v;
			itv np = itv(cur.l - d, cur.r + d), nq = itv(c[i - 1] - d, c[i - 1] + d);
			bool p = nq.is_in(c[i]), q = np.is_in(c[i]);
			if (p && q) {
				cur = np + nq;
				continue;
			}
			if (p) {
				cur = np;
				continue;
			}
			if (q) {
				cur = nq;
				continue;
			}
			return 0;
		}
		return 1;

	};
	i64 l = 0, r = 1e9, ret = -1;
	while (l <= r) {
		i64 mid = l + r >> 1;
		if (ck(mid)) ret = mid, r = mid - 1;
		else l = mid + 1;
	}
	cout << ret << '\n';
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
