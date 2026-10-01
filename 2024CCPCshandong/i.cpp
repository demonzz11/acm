#include <bits/stdc++.h>
using namespace std;
using i64 = long long;

const int inf = 1e9;
void go() {
	string s;
	cin >> s;
	if (s[0] == s.back()) {
		cout << 0 << '\n';
		return;
	}
	int ans = inf;
	for (int i = 1; i < s.size(); i++) {
		if (s[i] == s[i - 1]) {
			ans = min(i,ans);
		}
	}
	cout << (ans == inf ? -1 : ans) << '\n';
}

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	int t;
	std::cin >> t;
	while (t--) {
		go();
	}
	return 0;
}
