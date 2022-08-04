#include "aoclib.hpp"

struct Day1 : public Aoc {
	vector<ulong> depths;

	explicit Day1(ifstream &f) : Aoc{f} {
		parse(f, depths);
	}

	ulong part1() override {
		vector<long> diff{};
		diff.reserve(depths.size());
		adjacent_difference(depths.begin(), depths.end(), back_inserter(diff));
		return count_if(diff.begin() + 1, diff.end(), [](long val) {return val > 0;});
	}

	ulong part2() override {
		vector<ulong> slidingMean{};
		for (int i = 1; i < depths.size() - 1; ++i) {
			slidingMean.emplace_back(depths.at(i - 1) +
			                         depths.at(i) +
			                         depths.at(i + 1));
		}
		vector<long> diff{};
		diff.reserve(slidingMean.size());
		adjacent_difference(slidingMean.begin(), slidingMean.end(), back_inserter(diff));
		return count_if(diff.begin() + 1, diff.end(), [](long val) {return val > 0;});
	}
};

int main() {
	Aoc::run<Day1>(7, 5);
}
