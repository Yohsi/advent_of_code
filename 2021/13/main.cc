#include "aoclib.hpp"
#include <ranges>

struct Fold {
	char direction;
	ulong line;

	stream_io(Fold, direction, line);
};

uset<Vec2> apply_fold(Fold f, const uset<Vec2> &dots) {
	uset<Vec2> res;
	if (f.direction == 'y') {
		for (auto &dot: dots) {
			Vec2 new_dot{dot};
			new_dot.y = dot.y >= f.line ? 2 * f.line - dot.y : dot.y;
			assert(2 * f.line >= dot.y);
			res.insert(new_dot);
		}
	} else {
		for (auto &dot: dots) {
			Vec2 new_dot{dot};
			new_dot.x = dot.x >= f.line ? 2 * f.line - dot.x : dot.x;
			assert(2 * f.line >= dot.x);
			res.insert(new_dot);
		}
	}
	return res;
}


struct Day13 : public Aoc {
	uset<Vec2> dots;
	vector<Fold> folds;

	explicit Day13(ifstream &f) : Aoc{f} {
		auto lines = readLines(f);
		bool dot_section = true;
		for (auto &&line: lines) {
			if (line.empty()) {
				dot_section = false;
			} else {
				stringstream ss{line};
				if (dot_section) {
					Vec2 coord;
					ss >> coord.x;
					ss.ignore(1);
					ss >> coord.y;
					dots.insert(coord);
				} else {
					Fold fold;
					ss.ignore(11);
					ss >> fold.direction;
					ss.ignore(1);
					ss >> fold.line;
					folds.emplace_back(fold);
				}
			}
		}
//		print(dots);
//		print(folds);
	}


	ulong part1() override {
		auto dot1 = apply_fold(folds[0], dots);
		return dot1.size();
	}

	ulong part2() override {
		uset<Vec2> dots2 = dots;
		for (auto &&fold: folds) {
			dots2 = apply_fold(fold, dots2);
		}

		ulong xmax = 0;
		ulong ymax = 0;
		for (auto &&coord: dots2) {
			xmax = max(coord.x, xmax);
			ymax = max(coord.y, ymax);
		}
		print(xmax);
		print(ymax);

		for (ulong l = 0; l <= ymax; ++l) {
			for (ulong c = 0; c <= xmax; ++c) {
				if (dots2.contains({c, l})) {
					cout << white << '#';
				} else {
					cout << ' ';
				}
			}
			cout << '\n';
		}
		return 0;
	}
};

int main() {
	Aoc::run<Day13>(17);
}
