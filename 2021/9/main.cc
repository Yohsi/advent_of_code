#include "aoclib.hpp"
#include <ranges>

struct Day9 : public Aoc {
	vector<string> input;
	set<Vec2> low_points;

	explicit Day9(ifstream &f) : Aoc{f} {
		parse2(readBuf(f), input, "\n");
	}

	ulong part1() override {
		ulong ret = 0;
		const auto vsize = input.size();
		const auto hsize = input[0].size();
		for (ulong l = 0; l < vsize; ++l) {
			for (ulong c = 0; c < hsize; ++c) {
				char val = input[l][c];
				if ((l == 0 || input[l - 1][c] > val) &&
				    (l == (vsize - 1) || input[l + 1][c] > val) &&
				    (c == 0 || input[l][c - 1] > val) &&
				    (c == (hsize - 1) || input[l][c + 1] > val)) {
					ret += val + 1 - 48;
					low_points.insert({c, l});
//					print(l);
//					print(c);
//					print(val);
				}
			}
		}
		return ret;
	}

	void find_adjacent(Vec2 pt, set<Vec2> &s) {
		auto l = pt.y;
		auto c = pt.x;
		if (s.contains(pt) || input[l][c] == '9') return;

		s.insert(pt);

		const auto vsize = input.size();
		const auto hsize = input[0].size();

		if (l > 0) {
			find_adjacent({c, l - 1}, s);
		}
		if (l < vsize - 1) {
			find_adjacent({c, l + 1}, s);
		}
		if (c > 0) {
			find_adjacent({c - 1, l}, s);
		}
		if (c < hsize - 1) {
			find_adjacent({c + 1, l}, s);
		}
	}

	void print_basin(Vec2 low_point, const set<Vec2> &basin) {
		const auto vsize = input.size();
		const auto hsize = input[0].size();
		for (ulong l = 0; l < vsize; ++l) {
			for (ulong c = 0; c < hsize; ++c) {
				char val = input[l][c];
				if (low_point == Vec2{c, l}) {
					cout << light_magenta;
				} else if (basin.contains({c, l})) {
					cout << green;
				} else {
					cout << light_gray;
				}
				cout << val;
			}
			cout << endl;
		}
	}

	ulong part2() override {
		vector<ulong> basin_size{};
		for (auto &&low_point: low_points) {
			set<Vec2> basin;
			find_adjacent(low_point, basin);
//			print(low_point, green);
//			print(basin);
//			print_basin(low_point, basin);
//			print(basin.size(), cyan);
			basin_size.push_back(basin.size());
		}
		ranges::sort(basin_size, std::greater<>());
		return basin_size[0] * basin_size[1] * basin_size[2];
	}
};

int main() {
	Aoc::run<Day9>(15, 1134);
}
