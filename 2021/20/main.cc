#include "aoclib.hpp"
#include <ranges>
#include <bitset>

struct Day20 : public Aoc {
	string key;
	deque<deque<char>> grid;

	explicit Day20(ifstream &f) : Aoc{f} {
		key = readLine(f);
		readLine(f);
		int i = 0;
		for (auto &&l: readLines(f)) {
			if (l.empty()) break;
			grid.emplace_back();
			for (auto c: l) {
				grid[i].push_back(c);
			}
			i++;
		}
	}

	void print_grid() const {
		cout << magenta;
		for (auto &l: grid) {
			for (char c: l) {
				cout << c;
			}
			cout << '\n';
		}
	}

	void grow_grid(bool first, char c) {
		for (auto &&line: grid) {
			if (!first) {
				line.front() = c;
				line.back() = c;
			} else {
				line.emplace_front(c);
				line.emplace_back(c);
				line.emplace_front(c);
				line.emplace_back(c);
			}
			line.emplace_front(c);
			line.emplace_back(c);
		}
		if (!first) {
			grid.front() = deque<char>(grid[0].size(), c);
			grid.back() = deque<char>(grid[0].size(), c);
		} else {
			grid.emplace_front(grid[0].size(), c);
			grid.emplace_back(grid[0].size(), c);
			grid.emplace_front(grid[0].size(), c);
			grid.emplace_back(grid[0].size(), c);
		}
		grid.emplace_front(grid[0].size(), c);
		grid.emplace_back(grid[0].size(), c);
	}

	void enhance() {
		string tmp;
		tmp.reserve(9);

//		cout << light_gray << "--------------------------------\n";

		auto grid2 = deque<deque<char>>(grid.size(), deque<char>(grid[0].size(), '.'));

		auto grid_size = grid.size();

		for (int l = 1; l < grid_size - 1; ++l) {
			for (int c = 1; c < grid_size - 1; ++c) {
				for (int dl = -1; dl <= 1; ++dl) {
					for (int dc = -1; dc <= 1; ++dc) {
						tmp.push_back(grid[l + dl][c + dc]);
					}
				}
				bitset<9> adj{tmp, 0, 9, '.', '#'};
				tmp.clear();
				char new_char = key.at(adj.to_ulong());
				grid2[l][c] = new_char;
			}
		}
		grid = move(grid2);

		grow_grid(false, grid[1][1]);
	}

	ulong part1() override {
		grow_grid(true, '.');

		for (int i = 0; i < 2; i++) {
			enhance();
		}

		return accumulate(grid.begin(), grid.end(), 0ul, [](ulong a, auto &&line) {
			return a + ranges::count(line, '#');
		});
	}

	ulong part2() override {
		for (int i = 0; i < 50 - 2; i++) {
			enhance();
		}

		return accumulate(grid.begin(), grid.end(), 0ul, [](ulong a, auto &&line) {
			return a + ranges::count(line, '#');
		});
	}
};

int main() {
	Aoc::run<Day20>(35, 3351);
}
