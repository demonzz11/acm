#include<bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128;

void go() {
	i64 n, k;
	cin >> n >> k;
	vector<i64>a(n);
	i64 sum = 0;
	for (int i = 0; i < n; i++) {
		cin >> a[i];
		sum += a[i];
	}

	sort(a.begin(), a.end(), greater());
	if (n == 1) {
		cout << 1 << '\n';
		return;
	}
	if (n == 2) {
		cout << min(1 + k, a[0] + 1) << '\n';
		return;
	}
	auto ck = [&](i64 mid) ->bool{
		i64 res = 0;
		int i;
		int cnt = 0;
		for (i = 0; i < n; i++) {
			if (res + a[i] >= mid) {
				cnt = 1;
				break;
			}
			res += a[i];
		}
		return (n - i - 1) * k <= (mid - i - cnt);
	};
	i64 l = n;
	i64 r = sum;
	i64 ans = 0;
	while (l <= r) {
		i64 mid = l + (r - l) / 2;
		if (ck(mid)) {
			ans = mid;
			r = mid - 1;
		} else {
			l = mid + 1;
		}
	}
	cout << ans << '\n';
}

int main() {
	ios::sync_with_stdio(0);
	cin.tie(0);
	int t;
	cin >> t;
	while (t--)
		go();
}
