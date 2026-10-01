#include <bits/stdc++.h>
using namespace std;
using i64 = long long;

void go() {
	int n;
	cin >> n;
	if (n == 1 || n % 4 == 0) {
		cout << "impossible" << '\n';
		return ;
	}
	for (int i = 0; i < n; i++) {
		if (i == 0 || (i + 1) % 4 == 0) {
			cout << i + 1 << " " << i << " ";
			i++;
		} else {
			cout << i << " ";
		}
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
