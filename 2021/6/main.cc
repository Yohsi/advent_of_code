#include "aoclib.hpp"
#include <ranges>


struct Day6 : public Aoc {
	vector<int16_t> ages_input;

	explicit Day6(ifstream &f) : Aoc{f} {
		parse(f, ages_input, {","});
//		cout << ages_input << endl;
	}

	ulong part1() override {
		vector<int16_t> ages{ages_input};
		const int nb_days = 80;
		for (int day = 0; day < nb_days; ++day) {
			vector<ulong> to_append{};
			for (auto &age: ages) {
				age -= 1;
				if (age == -1) {
					age = 6;
					to_append.push_back(8);
				}
			}
			move(to_append.begin(), to_append.end(), back_inserter(ages));
			to_append.clear();
//			cout << "Day " << day + 1 << ": " << ages.size() << endl;
		}
		return ages.size();
	}

	ulong part2() override {
		const int nb_days = 256;
		map<uint16_t, ulong> ages_count{};
		map<uint16_t, ulong> to_add{};
		auto values = ages_count | views::values;
		auto to_add_values = to_add | views::values;
		for (auto &&age: ages_input) {
			ages_count[age] += 1;
		}

		for (int day = 0; day < nb_days; ++day) {
			auto age_added_fish = (day + 2) % 7;
			to_add[age_added_fish] = ages_count[day % 7];
			ages_count[day % 7] += to_add[day % 7];
			to_add[day % 7] = 0;
//			cout << "Day " << day + 1 << ": "/* << ages_count << endl*/;
//			cout << accumulate(values.begin(), values.end(), 0ull) +
//			        accumulate(to_add_values.begin(), to_add_values.end(), 0ull) << endl;
		}
		return accumulate(values.begin(), values.end(), 0ull) +
		       accumulate(to_add_values.begin(), to_add_values.end(), 0ull);
	}
};

int main() {
	Aoc::run<Day6>(5934, 26984457539);
}
