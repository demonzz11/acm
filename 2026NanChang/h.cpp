#include<bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128;
#define int long long

i64 lb(i64 x) {
	return x & (-x);
}
void go() {
	i64 n, k;
	cin >> n >> k;
	vector<i64>a(n);
	for (int i = 0; i < n; i++)
		cin >> a[i];
	i64 sum = 0;
	for (int i = 0; i < n; i++) {
		sum ^= a[i];
	}
	int num = lb(sum);
	if (num && num <= k) {
		cout << "Alice" << '\n';
	} else {
		cout << "Bob" << '\n';
	}
}

signed main() {
	ios::sync_with_stdio(0);
	cin.tie(0);
	int t;
	cin >> t;
	while (t--) {
		go();
	}
}
