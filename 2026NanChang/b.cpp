#include<bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128;


void go() {
	i64 n;
	cin >> n;
	vector<pair<int, char>>a(n * 4 + 1);

	for (int i = 1; i <= n * 4; i++) {
		cin >> a[i].first >> a[i].second;
	}
	map<int, char>mp;
	mp[0] = 'R';
	mp[1] = 'Y';
	mp[2] = 'G';
	mp[3] = 'B';
	int l = 2;
	for (int k = 0; k < 4; k++) {
		for (int i = 1; i <= n * 4; i++) {
			if (a[i].second == mp[k]) {
				for (int j = i; j >= l; j--) {
					if (a[j].first == a[j - 1].first) {
						swap(a[j - 1], a[j]);
					}
				}
				l++;
			}
		}
	}

	for (int i = 2; i <= n * 4; i++) {
		for (int j = i; j >= 2; j--) {
			if (a[j].first <= a[j - 1].first) {
				if (a[j].second == a[j - 1].second)
					swap(a[j], a[j - 1]);
			}
		}
	}
	for (int i = 1; i <= 4 * n; i++) {
		if (a[i].second != mp[(i - 1) / n]) {
			cout << "NO" << '\n';
			return;
		}
		if (a[i].first < a[i - 1].first && a[i].second == a[i - 1].second) {
			cout << "NO" << '\n';
			return;
		}
	}
	cout << "YES" << '\n';
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
