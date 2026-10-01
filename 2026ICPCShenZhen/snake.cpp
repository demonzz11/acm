#include <bits/stdc++.h>
using namespace std;

struct DirectionMap {
	int n, m;
	vector<vector<char>> a;

	void build(int _n, int _m) {
		n = _n;
		m = _m;
		a.assign(n + 1, vector<char>(m + 1));

		if (n % 2 == 0) {
			a[1][1] = 'R';
			for (int i = 2; i <= n; i++) {
				a[i][1] = 'U';
			}
			for (int i = 1; i <= n; i++) {
				if (i & 1) {
					a[i][2] = 'R';
				} else {
					a[i][2] = 'D';
				}
			}
			for (int i = 1; i <= n; i++) {
				for (int j = 3; j <= m - 1; j++) {
					if (i & 1) {
						a[i][j] = 'R';
					} else {
						a[i][j] = 'L';
					}
				}
			}
			for (int i = 1; i <= n; i++) {
				if (i & 1) {
					a[i][m] = 'D';
				} else {
					a[i][m] = 'L';
				}
			}
			for (int i = 2; i <= m; i++) {
				a[n][i] = 'L';
			}
			a[n][1] = 'U';
		} else {
			a[1][1] = 'R';
			for (int i = 2; i <= n; i++) {
				a[i][1] = 'U';
			}
			for (int i = 1; i <= n - 2; i++) {
				if (i & 1) {
					a[i][2] = 'R';
				} else {
					a[i][2] = 'D';
				}
			}
			for (int i = 1; i <= n - 2; i++) {
				for (int j = 3; j <= m - 1; j++) {
					if (i & 1) {
						a[i][j] = 'R';
					} else {
						a[i][j] = 'L';
					}
				}
			}
			for (int i = 1; i <= n - 2; i++) {
				if (i & 1) {
					a[i][m] = 'D';
				} else {
					a[i][m] = 'L';
				}
			}
			for (int i = m; i >= 2; i--) {
				if ((m - i + 1) & 1) {
					a[n][i] = 'L';
				} else {
					a[n][i] = 'U';
				}
			}
			for (int i = m; i >= 2; i--) {
				if ((m - i + 1) & 1) {
					a[n - 1][i] = 'D';
				} else {
					a[n - 1][i] = 'L';
				}
			}
			a[n][1] = 'U';
		}
		if (m == 2) {
			for (int i = 1; i <= n - 1; i++) {
				a[i][m] = 'D';
			}
		}
	}

	string get_path(int &x, int &y, int target_x, int target_y) {
		string path;
		int safety = 0;
		char original_n2 = a[n][2];
		if (n % 2 == 1) {
			if (target_x == n && target_y == 1) a[n][2] = 'L';
			if (target_x == n - 1 && target_y == 2) a[n][2] = 'U';
		}
		while (x != target_x || y != target_y) {
			if (++safety > n * m + 10) {
				path.clear();
				break;
			}
			char d = a[x][y];
			path += d;
			if (d == 'U') x--;
			else if (d == 'D') x++;
			else if (d == 'L') y--;
			else if (d == 'R') y++;
		}
		if (n % 2 == 1) a[n][2] = original_n2;
		return path;
	}
};

struct Snake {
	int n, m;
	deque<pair<int, int>> body;
	set<pair<int, int>> occupied;

	Snake(int _n, int _m, int sr, int sc) : n(_n), m(_m) {
		body.emplace_back(sr, sc);
		occupied.insert({sr, sc});
	}

	bool is_free(int r, int c, bool ignore_tail) {
		if (ignore_tail && body.size() > 1 && make_pair(r, c) == body.back())
			return true;
		return !occupied.count({r, c});
	}

	pair<bool, string> apply_move(char move, bool is_eat) {
		auto [r, c] = body.front();
		int nr = r, nc = c;
		switch (move) {
			case 'U':
				nr--;
				break;
			case 'D':
				nr++;
				break;
			case 'L':
				nc--;
				break;
			case 'R':
				nc++;
				break;
			default:
				return {false, "非法指令"};
		}
		if (nr < 1 || nr > n || nc < 1 || nc > m)
			return {false, "越界"};
		if (!is_free(nr, nc, !is_eat))
			return {false, "撞到自己"};

		body.push_front({nr, nc});
		if (!is_eat) {
			auto tail = body.back();
			body.pop_back();
			occupied.erase(tail);
		}
		occupied.insert({nr, nc});
		return {true, ""};
	}

	pair<bool, string> apply_sequence(const string& seq, int target_r, int target_c, bool is_last) {
		if (seq.empty()) return {false, "空指令序列"};
		for (size_t i = 0; i < seq.size() - 1; ++i) {
			auto [ok, err] = apply_move(seq[i], false);
			if (!ok) return {false, "第" + to_string(i + 1) + "步 " + err};
		}
		auto [ok, err] = apply_move(seq.back(), true);
		if (!ok) return {false, "最后一步 " + err};
		if (body.front() != make_pair(target_r, target_c))
			return {false, "最终头位置错误"};
		return {true, ""};
	}

	string body_str() {
		string res;
		for (auto &p : body) res += "(" + to_string(p.first) + "," + to_string(p.second) + ") ";
		return res;
	}

	vector<pair<int, int>> get_free_cells() {
		vector<pair<int, int>> free;
		for (int i = 1; i <= n; ++i)
			for (int j = 1; j <= m; ++j)
				if (!occupied.count({i, j}))
					free.emplace_back(i, j);
		return free;
	}
};

void test_one(int n, int m, int rs, int cs) {
	cout << "测试 " << n << "x" << m << " 起点 (" << rs << "," << cs << ")" << endl;
	DirectionMap dm;
	dm.build(n, m);

	Snake snake(n, m, rs, cs);
	int cur_x = rs, cur_y = cs;
	int total_apples = n * m - 1;
	mt19937 rng(42); // 固定种子

	for (int idx = 0; idx < total_apples; ++idx) {
		auto free_cells = snake.get_free_cells();
		if (free_cells.empty()) break; // 所有格子被占满，实际上最后一个苹果吃完后就没有了
		// 随机选择一个空闲格子作为下一个苹果
		uniform_int_distribution<int> dist(0, free_cells.size() - 1);
		auto [ar, ac] = free_cells[dist(rng)];

		string path = dm.get_path(cur_x, cur_y, ar, ac);
		if (path.empty()) {
			cout << "\n【失败】苹果 " << idx + 1 << " (" << ar << "," << ac << ") 导致死循环！\n";
			cout << "当前蛇头: (" << cur_x << "," << cur_y << ")\n";
			cout << "当前蛇身: " << snake.body_str() << "\n";
			cout << "方向图在头位置的指向: " << dm.a[cur_x][cur_y] << "\n";
			cout << "n,m = " << n << " " << m << ", 起点 (" << rs << "," << cs << ")\n";
			cout << "已经吃掉的苹果数: " << idx << "\n";
			exit(1);
		}
		bool is_last = (idx == total_apples - 1);
		auto [ok, err] = snake.apply_sequence(path, ar, ac, is_last);
		if (!ok) {
			cout << "\n【失败】苹果 " << idx + 1 << " (" << ar << "," << ac << ") 错误: " << err << "\n";
			cout << "输出的路径: " << path << "\n";
			cout << "当前蛇头: (" << cur_x << "," << cur_y << ")\n";
			cout << "当前蛇身: " << snake.body_str() << "\n";
			cout << "n,m = " << n << " " << m << ", 起点 (" << rs << "," << cs << ")\n";
			cout << "已经吃掉的苹果数: " << idx << "\n";
			exit(1);
		}
		// 更新当前头位置
		cur_x = ar;
		cur_y = ac;
		if ((idx + 1) % 50 == 0) {
			cout << "  已吃掉 " << (idx + 1) << " 个苹果，蛇长 " << snake.body.size() << endl;
		}
	}
	cout << "  成功！所有苹果已吃完。\n";
}

int main() {
	// 测试所有 n,m 组合，n*m <= 100
	for (int n = 2; n <= 10; ++n) {
		for (int m = 2; m <= 10; ++m) {
			for (int c = 1; c <= n; c++) {
				for (int j = 1; j <= m; j++) {
//			if (n * m > 100) continue;
					test_one(n, m, c, j);
				}
			}
		}
	}
	cout << "所有测试通过！\n";
	return 0;
}
