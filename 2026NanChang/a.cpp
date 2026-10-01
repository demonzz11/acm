#include<bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128;

void go() {
	i64 n, m;
	cin >> n >> m;
	if (n % m == 0) {
		cout << "1" << '\n';
	} else {
		cout << "0" << '\n';
	}
}

int main() {
	ios::sync_with_stdio(0);
	cin.tie(0);
	int t;
	cin >> t;
	while (t--) {
		go();
	}
}
