#include<bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128;
#define int long long
const int N = 1e6+10;
int f[N][4];

const i64 inf = 1e18;

void go() {
	int l, d;
	cin >> l >> d;
	int t0, t1, t2;
	cin >> t0 >> t1 >> t2;
	string s;
	cin >> s;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < 4; j++) {
			f[i][j] = inf;
		}
	}
	vector<int>p(l);
	for (int i = 0; i < l; i++) {
		if (s[i] == '0') {
			p[i] = t0;
		} else if (s[i] == '1') {
			p[i] = t1;
		} else if (s[i] == '2') {
			p[i] = t2;
		}
	}
	for (int j = 0; j < 4; j++)
		f[0][j] = 0;
	for (int i = 0; i < l; i++) {
		for (int j = 0; j < 3; j++) {
			int nd = min(l, i + d);
			f[nd][j + 1] = min(f[nd][j + 1], f[i][j]);
		}
		for (int j = 0; j < 4; j++) {
			f[i + 1][j] = min(f[i][j] + p[i], f[i + 1][j]);
		}
	}
	cout << *min_element(f[l], f[l] + 4);
}

signed main() {
	ios::sync_with_stdio(0);
	cin.tie(0);
//	cin >> t;
	go();
}
