#include "aoclib.hpp"
#include <ranges>


struct Day7 : public Aoc {
	vector<ulong> hpos;

	explicit Day7(ifstream &f) : Aoc{f} {
		parse(f, hpos, vector<string>{","});
		cout << hpos << endl;
	}

	ulong cost(long align) {
		ulong ret = 0;
		for (ulong h: hpos) {
			ret += labs(align - long(h));
		}
		return ret;
	}

	ulong part1() override {
		auto best_cost = UINT64_MAX;
		auto align = 0;
		bool stop = false;
		while (!stop) {
			ulong c = cost(align);
			if (c < best_cost) {
				best_cost = c;
			} else if (c > best_cost) {
				stop = true;
			}
			align++;
		}
		return best_cost;
	}

	ulong cost2(long align) {
		ulong ret = 0;
		for (ulong h: hpos) {
			auto n = labs(align - long(h));
			ret += n * (n + 1) / 2;
		}
		return ret;
	}

	ulong part2() override {
		auto best_cost = UINT64_MAX;
		auto align = 0;
		bool stop = false;
		while (!stop) {
			ulong c = cost2(align);
			if (c < best_cost) {
				best_cost = c;
			} else if (c > best_cost) {
				stop = true;
			}
			align++;
		}
		return best_cost;
	}
};

int main() {
	Aoc::run<Day7>(37, 168);
}
