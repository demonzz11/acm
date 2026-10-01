#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
void go() {
	int n;
	cin >> n;
	i64 r = n / 97;
	i64 l = n / 122;
	int g = -1;
	for (int i = l; i <= r; i++) {
		if (97 * i <= n && 122 * i >= n) {
			g = i;
			break;
		}
	}
	if (g == -1) {
		cout << "No" << '\n';
		return;
	} else {
		cout << "Yes" << '\n';
	}
	vector<char>ans(g + 1, 'z');
	i64 res = g*'z';
	for (int i = 1; i <= g; i++) {
		if (res - 25 > n) {
			ans[i] = 'a';
			res -= 25;
		} else {
			ans[i] = 'z' - ( res - n);
			res = n;
		}
		cout << ans[i];
	}
	cout << '\n';
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
