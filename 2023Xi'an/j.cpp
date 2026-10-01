#include<bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128;

const double eps = 1e-4;
void go() {
	double a, b;
	cin >> a >> b;
	vector<int>v;
	double c = b;
	for (int i = 1; i < 100; i++) {
		double g = pow(0.5, i);
		if (b >= g) {
			b -= g;
			v.push_back(i);
		}
	}
	for (int i = 1; i <= 14; i++) {
		a = a * 0.5;
		cout << 1;
	}
	if (abs(a - c) <= eps) {
		return;
	}
	int m = 0;
	cout << 2;
	for (int k = 0; k < v.size(); k++) {
		v[k] -= m;
		for (int i = 1; i <= v[k] - 1; i++) {
			cout << 1;
		}
		cout << 2;
		m += v[k];
	}
}

int main() {
	ios::sync_with_stdio(0);
	cin.tie(0);
	go();
}
