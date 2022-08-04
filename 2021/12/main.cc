#include "aoclib.hpp"
#include <ranges>


struct Day12 : public Aoc {
	map<string, set<string>> caves;

	explicit Day12(ifstream &f) : Aoc{f} {
		vector<vector<string>> connections;
		parse2(readBuf(f), connections, "\n", "-");
		for (auto &&connection: connections) {
			caves[connection[0]].insert(connection[1]);
			caves[connection[1]].insert(connection[0]);
		}
		print(caves);
	}

	static bool is_big(const string_view cave) {
		return cave[0] < 91;
	}

	ulong find_path(vector<string> &path) {
		if (path.back() == "end") {
			print(path);
			return 1;
		}
		ulong res = 0;
		for (auto &&cave: caves[path.back()]) {
			if (is_big(cave) || ranges::find(path, cave) == path.end()) {
				path.push_back(cave);
				res += find_path(path);
				path.pop_back();
			}
		}
		return res;
	}

	ulong part1() override {
		vector<string> path{"start"};
		return find_path(path);
	}

	ulong find_path2(vector<string> &path, bool double_visit_done) {
		if (path.back() == "end") {
			print(path);
			return 1;
		}
		ulong res = 0;
		for (auto &&cave: caves[path.back()]) {
			if (is_big(cave) || ranges::count(path, cave) == 0) {
				path.push_back(cave);
				res += find_path2(path, double_visit_done);
				path.pop_back();
			} else if (!double_visit_done && cave != "start" && cave != "end" && ranges::count(path, cave) == 1) {
				path.push_back(cave);
				res += find_path2(path, true);
				path.pop_back();
			}
		}
		return res;
	}

	ulong part2() override {
		vector<string> path{"start"};
		return find_path2(path, false);
	}
};

int main() {
	Aoc::run<Day12>(10, 36);
}
