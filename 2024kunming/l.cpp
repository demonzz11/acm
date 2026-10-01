#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

using llsi = long long signed int;

void work() {
	int n;
	llsi p, q;
	std::cin >> n >> p >> q;
	std::vector<std::pair<int, int>> hkr;
	for(int i = 0, x, y; i < n; ++i) {
		std::cin >> x >> y;
		if(x >= p || y >= q) continue;
		hkr.emplace_back(x, y);
	}
	auto cmp=[&](const std::pair<int, int>& A,const std::pair<int, int>& B)
	{
		return A.first!=B.first?A.first<B.first:A.second>B.second;
	};
	std::sort(hkr.begin(), hkr.end(), cmp);
	llsi ue = 0, ans = (q + 1) * q / 2 * (p + 1) + (p + 1) * p / 2 * (q + 1);
	std::vector <int> f;
	f.push_back(0);
	int l = 0;
	for(auto [x, y]: hkr) {
		ans -= (x - l) * ue;
		l = x;
		if (y>=f.back())
		{
			f.push_back(y+1);
			ue+=q-y;
		} else
		{
			int p=upper_bound(f.begin(),f.end(),y)-f.begin();
			if (p>=0&&p<f.size())
			{
				ue+=f[p]-(y+1);
				f[p]=y+1;
			}
		}
	}
	ans -= (p - l) * ue;
	std::cout << ans << char(10);
}

int main() {
	std::ios::sync_with_stdio(false);
	int T; std::cin >> T; while(T--) work();
	return 0;
}
