#include "aoclib.hpp"
#include <ranges>


struct Day14 : public Aoc {
	string s_input;
	umap<string, char> rules;

	explicit Day14(ifstream &f) : Aoc{f} {
		s_input = readLine(f);
		readLine(f);
		for (auto &&r: readLines(f)) {
			if (r.empty()) break;
			auto split = splitLine(r, " -> ");
			rules.emplace(split[0], split[1][0]);
		}
	}

	ulong part1() override {
		string s = s_input;
		const int nIter = 10;
		for (int i = 0; i < nIter; ++i) {
			string new_string{};
			for (int j = 0; j < s.size() - 1; ++j) {
				char s1 = s[j];
				char s2 = s[j + 1];
				new_string.push_back(s1);
				string ss{s1, s2};
				if (rules.contains(ss)) {
					new_string.push_back(rules.at(ss));
				}
			}
			new_string.push_back(s.back());
			s = new_string;
			umap<char, ulong> count;
			for (auto &&c: s) {
				count[c] += 1;
			}
			ulong min = 99999;
			ulong max = 0;
			for (auto &&[c, n]: count) {
				if (n < min) {
					min = n;
				}
				if (n > max) {
					max = n;
				}
			}
			print(count, blue);
		}
		umap<char, ulong> count;
		for (auto &&c: s) {
			count[c] += 1;
		}
		ulong min = 99999;
		ulong max = 0;
		for (auto &&[c, n]: count) {
			if (n < min) {
				min = n;
			}
			if (n > max) {
				max = n;
			}
		}
		print(count, blue);
		return max - min;
	}


	ulong part2() override {
		const int nIter = 40;

		umap<string, pair<string, string>> rules_successors{};

		for (auto &&[digram, c]: rules) {
			string digram1{digram[0], c};
			string digram2{c, digram[1]};
			rules_successors[digram].first = digram1;
			rules_successors[digram].second = digram2;
		}
		print(rules_successors);

		umap<string, ulong> digrams1{};
		for (int i = 0; i < s_input.size() - 1; ++i) {
			char s1 = s_input[i];
			char s2 = s_input[i + 1];
			string s1s2{s1, s2};
			digrams1[s1s2]++;
		}

		umap<string, ulong> digrams2{};
		for (int i = 1; i <= nIter; ++i) {
			for (auto &&[digram, n]: digrams1) {
				if (rules_successors.contains(digram)) {
					auto &&[d1, d2] =  rules_successors.at(digram);
					digrams2[d1] += n;
					digrams2[d2] += n;
				} else {
					digrams2[digram] += n;
				}
			}
			swap(digrams1, digrams2);
			digrams2.clear();
		}

		umap<char, ulong> total_count{};
		for (auto &&[digram, n]: digrams1) {
			total_count[digram[0]] += n;
			total_count[digram[1]] += n;
		}

		total_count[s_input[0]]++;
		total_count[s_input[s_input.size() - 1]]++;
		for (auto &&[c, n]: total_count) {
			n /= 2;
		}


		ulong min = 1e19;
		ulong max = 0;
		char cmin, cmax;
		for (auto &&[c, n]: total_count) {
			if (n < min) {
				min = n;
				cmin = c;
			}
			if (n > max) {
				max = n;
				cmax = c;
			}
		}

		print(min);
		print(max);
		print(cmin);
		print(cmax);
		return max - min;
	}
};

int main() {
	Aoc::run<Day14>(1588, 2188189693529);
}
