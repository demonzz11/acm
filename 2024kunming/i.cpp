#include <bits/stdc++.h>
using namespace std;
using i64 = long long;

void go() {
	string s;
	cin >> s;

	vector<int>len;
	int cnt = 1;
	for (int i = 1; i < s.size(); i++) {
		if (s[i] == s[i - 1]) {
			cnt++;
		} else {
			len.push_back(cnt);
			cnt = 1;
		}
	}
	len.push_back(cnt);
	if (len.size() > 1 && s[0] == s.back()) {
		len[0] += len.back();
		len.pop_back();
	}
	if(len.size()==1){
		cout<<len[0]/2<<'\n';
		return;
	}
	i64 ans = 0;
	int fg = 0;
	for (int i = 0; i < len.size(); i++) {
		fg += (len[i] % 2 == 0);
		ans += len[i] / 2;
	}
	if (fg)
		ans--;
	cout << ans<<'\n';
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
