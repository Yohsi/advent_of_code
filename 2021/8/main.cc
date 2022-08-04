#include "aoclib.hpp"
#include <ranges>
#include <cmath>

struct Day8 : public Aoc {
	vector<vector<vector<string>>> patterns;

	explicit Day8(ifstream &f) : Aoc{f} {
		auto input = readBuf(f);
		parse2(input, patterns, "\n", " | ", " ");
//		cout << patterns << endl;
	}

	ulong part1() override {
		ulong unique_count = 0;
		for (auto &&display: patterns) {
			for (auto &&digit: display[1]) {
				auto size = digit.size();
				unique_count += (size == 2 || //1
				                 size == 3 || //7
				                 size == 4 || //4
				                 size == 7);  //8
			}
		}
		return unique_count;
	}

	static const set<char> segments;
	static const array<set<char>, 10> segments_used;

	static void remove_assigned(map<char, set<char>> &mapping) {
		map<char, set<char>> prev;
		while (prev != mapping) {
			prev = mapping;
			for (char c = 'a'; c <= 'g'; c++) {
				if (mapping[c].size() == 1) {
					for (char o = 'a'; o <= 'g'; o++) {
						if (o != c) {
							mapping[o].erase(c);
						}
					}
				}
			}
		}
	}

	static void update_mappings(const string &pattern, const set<int> &digits, map<char, set<char>> &mapping) {
		set<char> union_segments_used{};
		set<char> not_in_pattern = set_difference(segments, set<char>{pattern.begin(), pattern.end()});
		for (int d: digits) {
			union_segments_used = set_union(union_segments_used, segments_used[d]);
		}

		for (char c: pattern) {
			mapping[c] = set_intersection(mapping[c], union_segments_used);
		}
		if (digits.size() == 1) {
			for (char other: not_in_pattern) {
				mapping[other] = set_intersection(mapping[other], set_difference(segments, union_segments_used));
			}
		} else {
			set<int> possible = digits;
			for (int d: digits) {
				set<char> segments_unused = set_difference(segments, segments_used[d]);
				for (char c: not_in_pattern) {
					if (set_intersection(mapping.at(c), segments_unused).empty()) {
						possible.erase(d);
					}
				}
			}
			if (possible.size() == 1) {
				update_mappings(pattern, possible, mapping);
			}
		}

//		remove_assigned(mapping);
	}

	// cdfbe -> 2,3,5
	// ag missing -> c,f et e,g
	// 2 -> b et f -> non
	// 3 -> b et e -> non
	// 5 -> c et e -> oui
	//
	// new_mapping = {
	//   a : {c, f}
	//   b : {c, f}
	//   c : {e, g}
	//   d : {a}
	//   e : {b, d}
	//   f : {b, d}
	//   g : {e, g}
	// }
	// mapping = {
	//   a : {c, f}
	//   b : {c, f}
	//   c : {e, g}
	//   d : {a}
	//   e : {b, d}
	//   f : {b, d}
	//   g : {e, g}
	// }
	//  aaa
	// b   c
	// b   c
	//  ddd
	// e   f
	// e   f
	//  ggg
	//

	static bool possible_digit(int digit, const string &pattern, const map<char, set<char>> &mapping) {
		auto &digit_segments = segments_used[digit];
		auto local_mapping = mapping;
		map<char, set<char>> prev_mapping{};
		while (prev_mapping != local_mapping) {
			prev_mapping = local_mapping;
			for (char c: pattern) {
				local_mapping[c] = set_intersection(local_mapping.at(c), digit_segments);
			}
		}
		return !ranges::any_of(local_mapping, [](auto &&p) {return p.second.empty();});

//		return ranges::all_of(pattern, [&](char c) {return !set_intersection(mapping.at(c), digit_segments).empty();});
	}


	static set<int> possible_digits(const string &pattern, const map<char, set<char>> &mapping) {
		switch (pattern.size()) {
			case 2 :
				return {1};
			case 3 :
				return {7};
			case 4 :
				return {4};
			case 5 : {
				set<int> s{2, 3, 5};
				erase_if(s, [&](int d) {return !possible_digit(d, pattern, mapping);});
				return s;
			}
			case 6 : {
				set<int> s{0, 6, 9};
				erase_if(s, [&](int d) {return !possible_digit(d, pattern, mapping);});
				return s;
			}
			case 7 : {
				return {8};
			}
		}
		throw runtime_error{"Unexpected value"};
	}

	ulong decode(const vector<string> &encoded, const map<char, char> &encoded_to_actual) {
		ulong ret = 0;
		for (int i = 0; i < encoded.size(); ++i) {
			set<char> actual_segments;
			for (char c: encoded[i]) {
				actual_segments.insert(encoded_to_actual.at(c));
			}
			print(encoded[i]);
			print(actual_segments);
			int digit = 0;
			for (int j = 0; j < segments_used.size(); ++j) {
				if (segments_used[j] == actual_segments) {
					digit = j;
					break;
				}
			}
			print(digit);
			ret += digit * powl(10, encoded.size() - i - 1);
		}
		return ret;
	}

	ulong part2() override {
		ulong ret = 0;
		vector<map<char, set<char>>> possible_mappings{patterns.size(),
		                                               {{'a', segments},
		                                                {'b', segments},
		                                                {'c', segments},
		                                                {'d', segments},
		                                                {'e', segments},
		                                                {'f', segments},
		                                                {'g', segments}}};
		for (int i = 0; i < patterns.size(); ++i) {
			auto &mapping = possible_mappings[i];
			auto unknowns = mapping | views::filter([](auto &&p) {return p.second.size() > 1;});
			map<char, set<char>> prev_mapping{};
			while (mapping != prev_mapping) {
				prev_mapping = mapping;
				for (auto &&pattern: patterns[i][0]) {
					auto digits = possible_digits(pattern, mapping);
					update_mappings(pattern, digits, mapping);
				}
			}

			map<char, char> encoded_to_actual{};
			for (auto&&[encoded, actual]: possible_mappings[i]) {
				encoded_to_actual.insert({encoded, *actual.begin()});
			}
			print(mapping);
			ret += decode(patterns[i][1], encoded_to_actual);
		}

		return ret;
	}
};

const set<char> Day8::segments = {'a', 'b', 'c', 'd', 'e', 'f', 'g'};
const array<set<char>, 10> Day8::segments_used = {{{'a', 'b', 'c', 'e', 'f', 'g'}, // 0
                                                   {'c', 'f'}, // 1
                                                   {'a', 'c', 'd', 'e', 'g'}, // 2
                                                   {'a', 'c', 'd', 'f', 'g'}, // 3
                                                   {'b', 'c', 'd', 'f'}, // 4
                                                   {'a', 'b', 'd', 'f', 'g'}, // 5
                                                   {'a', 'b', 'd', 'e', 'f', 'g'}, // 6
                                                   {'a', 'c', 'f'}, // 7
                                                   {'a', 'b', 'c', 'd', 'e', 'f', 'g'}, // 8
                                                   {'a', 'b', 'c', 'd', 'f', 'g'}}}; //9

int main() {
	Aoc::run<Day8>(26, 61229);
}
