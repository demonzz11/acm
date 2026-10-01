#include<bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128;
const int N = 2e5+10;
vector<i64>a[N];
i64 f[N][3];
string s;
void dfs(int x) {
	if (a[x].empty()) {
		return;
	}
	for (auto&y : a[x]) {
		dfs(y);
		for (int i = 0; i < 3; i++) {
			if (f[y][i] != s[x]) {
				f[x][i] = f[y][i] + 1;
			} else {
				f[x][i] = f[y][i];
			}
		}
	}
}
void go() {
	int n;
	cin >> n;
	cin >> s;
	for (int i = 0; i < n - 1; i++) {
		int u, v;
		cin >> u >> v;
		u--, v--;
		a[u].push_back(v);
	}
	dfs(0);
	cout << min({f[0][1], f[0][0], f[0][2]}) << '\n';
}

int main() {
	ios::sync_with_stdio(0);
	cin.tie(0);
	go();
}
