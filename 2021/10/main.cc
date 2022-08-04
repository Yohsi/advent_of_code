#include "aoclib.hpp"
#include <ranges>
#include <cassert>

struct ParseResult {
	ulong length;
	bool success;

	stream_io(ParseResult, length, success);
};

struct CompleteResult {
	ulong length;
	string to_add;

	stream_io(CompleteResult, length, to_add);
};

struct Day10 : public Aoc {

	map<char, ulong> syntax_score{{')', 3},
	                              {']', 57},
	                              {'}', 1197},
	                              {'>', 25137}};
	map<char, ulong> autocomplete_score{{')', 1},
	                                    {']', 2},
	                                    {'}', 3},
	                                    {'>', 4}};

	vector<string> lines;

	explicit Day10(ifstream &f) : Aoc{f} {
		lines = readLines(f);
	}

	ParseResult read_chunk(const string_view str) {
		if (str.empty()) {
			return {0, true};
		}
		ulong ret = 0;
		char c;
		if (str[0] == '{') {
			c = '}';
		} else if (str[0] == '[') {
			c = ']';
		} else if (str[0] == '<') {
			c = '>';
		} else if (str[0] == '(') {
			c = ')';
		} else {
			return {0, true};
		}
		ret++;
		auto parse_result = read_chunk(str.substr(ret));
		ret += parse_result.length;
		if (!parse_result.success) {
			return {ret, false};
		}
		while (parse_result.length) {
			parse_result = read_chunk(str.substr(ret));
			ret += parse_result.length;
			if (!parse_result.success) {
				return {ret, false};
			}
		}
		if (str.size() > ret) {
			if (str[ret] == c) {
				ret++;
			} else {
//				cout << light_gray << "Expected " << c << " but got " << str[ret] << " instead" << endl;
				return {ret, false};
			}
		}
		return {ret, true};
	}

	ulong part1() override {
		ulong score = 0;
//		vector<string> lines = {"[)", "{()()()>", "(((()))}"};
		for (auto &&line: lines) {

			auto parse_result = read_chunk(line);
			auto parsed_length = parse_result.length;
			while (parse_result.success && parse_result.length) {
				parse_result = read_chunk(line.substr(parsed_length));
				parsed_length += parse_result.length;
			}

			if (!parse_result.success) {
				char incorrect = line[parsed_length];
				score += syntax_score[incorrect];
				print(syntax_score[incorrect]);
			}
			for (int i = 0; i < line.size(); i++) {
				if (i == parsed_length) {
					cout << red;
				} else if (i > parsed_length) {
					cout << dark_gray;
				} else {
					cout << green;
				}
				cout << line[i];
			}
			cout << endl;
		}
		return score;
	}

	CompleteResult autocomplete(string_view str) {
		if (str.empty()) {
			return {0, ""};
		}
		ulong parsed = 0;
		string ret;
		char c;
		if (str[0] == '{') {
			c = '}';
		} else if (str[0] == '[') {
			c = ']';
		} else if (str[0] == '<') {
			c = '>';
		} else if (str[0] == '(') {
			c = ')';
		} else {
			return {0, ret};
		}
		parsed++;
		auto result = autocomplete(str.substr(parsed));
		parsed += result.length;
		ret += result.to_add;
		while (result.length) {
			result = autocomplete(str.substr(parsed));
			parsed += result.length;
			ret += result.to_add;
		}
		if (parsed < str.size()) {
			if (str[parsed] == c) {
				parsed++;
				return {parsed, ret};
			} else {
				assert(false);
			}
		} else {
			return {parsed, ret + c};
		}
	}

	ulong part2() override {
		vector<ulong> scores{};
//		vector<string> lines = {"[)", "{()()()>", "(((()))}"};
		for (auto &&line: lines) {
			auto parse_result = read_chunk(line);
			auto parsed_length = parse_result.length;
			while (parse_result.success && parse_result.length) {
				parse_result = read_chunk(line.substr(parsed_length));
				parsed_length += parse_result.length;
			}

			if (!parse_result.success) {
				continue;
			}

			string to_add;
			auto result = autocomplete(line);
			parsed_length = result.length;
			to_add += result.to_add;
			while (result.length) {
				result = autocomplete(line.substr(parsed_length));
				parsed_length += result.length;
				to_add += result.to_add;
			}
			cout << magenta << line << green << to_add << endl;

			ulong score = 0;
			for (char c: to_add) {
				score *= 5;
				score += autocomplete_score[c];
			}
			scores.push_back(score);
		}
		ranges::sort(scores);
		print(scores);
		return scores[scores.size() / 2];
	}

};

int main() {
	Aoc::run<Day10>(26397, 288957);
}
