#include<bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128;

void go() {
	int a, b, c;
	cin >> a >> b >> c;
	if (a > b) {
		cout << "Win" << '\n';
	} else if (c > b) {
		cout << "WIN" << '\n';
	} else {
		cout << "nowin" << '\n';
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
