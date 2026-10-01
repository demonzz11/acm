#include<bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128;

void go() {
	int n;
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cout << i << " \n"[i == n];
	}
}

int main() {
	ios::sync_with_stdio(0);
	cin.tie(0);
	go();
}
