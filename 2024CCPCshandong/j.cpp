#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
#define int long long
const int N = 1e3+5;
const int INF = 0x3f3f3f3f;
int n, m, g[N][N];
int d[N], v[N], a[N];

int prim() {
	memset(d, 0x3f, sizeof(d));
	memset(v,0,sizeof(v));
	int ans = 0;
	for (int i = 0; i < n; i++) {
		int t = -1;
		for (int j = 1; j <= n; j++)
			if (v[j] == 0 && (t == -1 || d[j] < d[t]))t = j;
		v[t] = 1;
		if (i && d[t] == INF)return INF;
		if (i)ans += d[t];
		for (int j = 1; j <= n; j++)d[j] = min(d[j], g[t][j]);
	}
	return ans;
}
void go() {
	memset(g, 0x3f, sizeof(g));
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> a[i];
	}
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			int w;
			cin >> w;
			g[i][j] = w;
		}
	}
	int t = prim();
	for (int i = 1; i <= n; i++) {
		int w = INF;
		for (int j = 1; j <= n; j++) {
			w = min(w, g[i][j]);
		}
		t += (a[i] - 1) * w;
	}
	cout << t << '\n';
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
