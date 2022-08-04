#include "aoclib.hpp"
#include <ranges>

struct Vec2 {
	long x, y;

	stream_io(Vec2, x, y);
	friend auto operator<=>(const Vec2 &, const Vec2 &) = default;
};


struct Line {
	Vec2 start;
	Vec2 end;

	[[nodiscard]] bool isVertical() const {
		return start.x == end.x;
	}

	[[nodiscard]] bool isHorizontal() const {
		return start.y == end.y;
	}

	[[nodiscard]] set<Vec2> includedPoints() const {
		set<Vec2> ret;
		auto dirX = min(max(end.x - start.x, -1l), 1l);
		auto dirY = min(max(end.y - start.y, -1l), 1l);
		for (long x = start.x, y = start.y;
		     x != end.x + dirX || y != end.y + dirY;
		     x += dirX, y += dirY) {
			ret.insert(Vec2{x, y});
		}
		return ret;
	}

	stream_io(Line, start, end);
};

struct Day5 : public Aoc {
	vector<Line> lines;
	map<Vec2, ulong> nb_lines_crossing1;
	map<Vec2, ulong> nb_lines_crossing2;

	explicit Day5(ifstream &f) : Aoc{f} {
		auto delimiters = {"\n", " -> ", ","};
		parse(f, lines, delimiters);

//		cout << lines << endl;
	}

	ulong part1() override {
		auto hvlines = lines | views::filter([](Line line) {return line.isVertical() || line.isHorizontal();});
		for (auto &&line: hvlines) {
			for (auto &&point: line.includedPoints()) {
				nb_lines_crossing1[point] += 1;
			}
		}
		return count_if(nb_lines_crossing1.begin(), nb_lines_crossing1.end(), [](auto &&p) {
			return p.second >= 2;
		});
	}

	ulong part2() override {
		for (auto &&line: lines) {
			for (auto &&point: line.includedPoints()) {
				nb_lines_crossing2[point] += 1;
			}
		}
		return count_if(nb_lines_crossing2.begin(), nb_lines_crossing2.end(), [](auto &&p) {
			return p.second >= 2;
		});
	}
};

int main() {
	Aoc::run<Day5>(5, 12);
}
