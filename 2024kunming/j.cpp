#include <iostream>
#include <queue>

using llsi = long long signed int;
using pii = std::pair<int, int>;
using tuple = std::tuple<int, int, int>;

template<typename T>
using less_pq = std::priority_queue<T, std::vector<T>, std::greater<T>>;

void work() {
	int n, m, k;
	std::cin >> n >> m >> k;
	std::vector<std::vector<tuple>> out(n);

	for (int i = 0, u, v, c, l; i < m; ++i) {
		std::cin >> u >> v >> c >> l;
		u--, v--, c--;
		out[u].emplace_back(v, c, l);
		out[v].emplace_back(u, c, l);
	}

	std::vector<pii> dis(n, {1e9, 0});
	std::vector<bool> vis(n, false);
	std::vector<less_pq<std::pair<int, int>>> hang(m);
	less_pq<std::pair<pii, int>> cur;
	for (int i = 0; i < m; ++i) hang[i].push({0, 0});
	for (int i = 1, a, b; i <= k; ++i) {
		std::cin >> a >> b;
		a--;
		while (hang[a].size() && hang[a].top().first <= b) {
			cur.push({{i, hang[a].top().first}, hang[a].top().second});
			hang[a].pop();
		}
		while (cur.size()) {
			auto [cur_dis, current] = cur.top();
			cur.pop();
			if (vis[current]) continue;
			vis[current] = 1;
			for (auto [out, color, length] : out[current]) {
				if (vis[out]) continue;
				if (color != a) {
					hang[color].push({length, out});
					continue;
				}
				if (cur_dis.second + length <= b && pii(i, cur_dis.second + length) < dis[out]) {
					dis[out] = pii(i, cur_dis.second + length);
					cur.push({dis[out], out});
				} else {
					hang[a].push({length, out});
					continue;
				}
			}
		}
	}
	for (int i = 0; i < n; ++i) std::cout << int(vis[i]);
	std::cout << char(10);
}

int main() {
	std::ios::sync_with_stdio(false);
	int T;
	std::cin >> T;
	while (T--) work();
	return 0;
}


