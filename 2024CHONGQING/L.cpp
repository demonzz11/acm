#include<bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128;
const int mod = 998244353;
void go() {
	vector<int>stk;
	int n;
	cin >> n;
	i64 sum = 0;
	for (int i = 0; i < n; i++) {
		string s;
		int a;
		cin >> s;
		if (s == "Push") {
			cin >> a;
			sum = (sum + a) % mod;
			stk.push_back(a);
		} else if (s == "Pop") {
			sum = (sum - stk.back() + mod) % mod;
			stk.pop_back();
		} else {
			if (stk.size() < 1e6) {
				vector<int>n_s = stk;
				for (int i = 0; i < n_s.size(); i++) {
					stk.push_back(n_s[i]);
				}
			}
			sum = sum * 2 % mod;
		}
		cout << sum << '\n';
	}
}

int main() {
	ios::sync_with_stdio(0);
	cin.tie(0);
	go();
}
