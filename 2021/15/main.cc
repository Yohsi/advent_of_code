#undef _GLIBCXX_DEBUG

#include "aoclib.hpp"
#include <ranges>

struct Day11 : public Aoc {
	vector<string> input;

	explicit Day11(ifstream &f) : Aoc{f} {
		parse2(readBuf(f), input, "\n");
//		print(input);
	}


	ulong part1() override {
		auto vsize = input.size();
		auto hsize = vsize == 0 ? 0 : input[0].size();

		auto cost = [&](const Vec2 &c1, const Vec2 &c2) {return input[c2.y][c2.x] - '0';};
		auto heuristic = [&](const Vec2 &c1, const Vec2 &c2) {
			return labs(long(c2.x) - long(c1.x)) + labs(long(c1.y) - long(c2.y));
		};
		auto neighbors = [&](const Vec2 &c) {
			vector<Vec2> result;
			result.reserve(4);
			if (c.x > 0)
				result.emplace_back(Vec2{c.x - 1, c.y});
			if (c.x + 1 < hsize)
				result.emplace_back(Vec2{c.x + 1, c.y});
			if (c.y > 0)
				result.emplace_back(Vec2{c.x, c.y - 1});
			if (c.y + 1 < vsize)
				result.emplace_back(Vec2{c.x, c.y + 1});
			return result;
		};

		auto path = a_star(neighbors, cost, heuristic, Vec2{0, 0}, Vec2{vsize - 1, hsize - 1});
		print(path);
#ifndef NDEBUG
		for (ulong l = 0; l < vsize; ++l) {
			for (ulong c = 0; c < hsize; ++c) {
				if (ranges::count(path.path, Coord{c, l}) > 0) {
					cout << light_magenta;
				} else {
					cout << light_gray;
				}
				cout << input[l][c];
			}
			cout << '\n';
		}
#endif
		return path.cost;
	}

	ulong part2() override {
		auto vsize = input.size();
		auto hsize = vsize == 0 ? 0 : input[0].size();
		auto vsize2 = input.size() * 5;
		auto hsize2 = vsize == 0 ? 0 : input[0].size() * 5;
		vector<string> input2{vsize * 5, string(hsize * 5, '_')};
		for (int block_y = 0; block_y < 5; ++block_y) {
			for (int block_x = 0; block_x < 5; ++block_x) {
				for (int y = 0; y < vsize; ++y) {
					for (int x = 0; x < hsize; ++x) {
						auto x2 = block_x * hsize + x;
						auto y2 = block_y * vsize + y;
						input2[y2][x2] = (input[y][x] - '1' + block_x + block_y) % 9 + '1';
					}
				}
			}
		}
		auto cost = [&](const Vec2 &c1, const Vec2 &c2) {return input2[c2.y][c2.x] - '0';};
		auto heuristic = [&](const Vec2 &c1, const Vec2 &c2) {
			return labs(long(c2.x) - long(c1.x)) + labs(long(c1.y) - long(c2.y));
		};
		auto neighbors = [&](const Vec2 &c) {
			vector<Vec2> result;
			result.reserve(4);
			if (c.x > 0)
				result.emplace_back(Vec2{c.x - 1, c.y});
			if (c.x + 1 < hsize2)
				result.emplace_back(Vec2{c.x + 1, c.y});
			if (c.y > 0)
				result.emplace_back(Vec2{c.x, c.y - 1});
			if (c.y + 1 < vsize2)
				result.emplace_back(Vec2{c.x, c.y + 1});
			return result;
		};

		auto path = a_star(neighbors, cost, heuristic, Vec2{0, 0}, Vec2{vsize2 - 1, hsize2 - 1});
		print(path);
#ifndef NDEBUG
		for (ulong l = 0; l < vsize2; ++l) {
			for (ulong c = 0; c < hsize2; ++c) {
				if (ranges::count(path.path, Coord{c, l}) > 0) {
					cout << light_magenta;
				} else {
					cout << light_gray;
				}
				cout << input2[l][c];
			}
			cout << '\n';
		}
#endif
		return path.cost;
	}
};

int main() {
	Aoc::run<Day11>(40, 315);
}
