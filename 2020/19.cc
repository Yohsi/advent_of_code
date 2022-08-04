#include <sstream>
#include "lib.h"

set<int>
checkRule(int rule, const string &word, const map<int, set<vector<int>>> &nonTermRules, const map<int, char> &termRules,
          int i, int spaces) {
	if (i >= word.size()) {
//		cout << genSpaces(spaces) << "reached end of the word\n";
		return {};
	}

	char c = word[i];
	if (termRules.find(rule) != termRules.end()) {
//		cout << genSpaces(spaces) << "rule " << rule << " -> " << termRules.at(rule) << "\n";
		if (c == termRules.at(rule)) {
//			cout << genSpaces(spaces) << "terminal = " << c << " at index " << i << " -> OK\n";
			return {1};
		} else {
//			cout << genSpaces(spaces) << "terminal = " << c << " at index " << i << " -> not OK\n";
			return {};
		}
	} else {
//		cout << genSpaces(spaces) << "rule " << rule << " -> " << nonTermRules.at(rule) << "\n";
		set<int> ret = {};

		// decompose OR
		for (auto &&subruleSequence : nonTermRules.at(rule)) {
//			cout << genSpaces(spaces) << "testing sequence " << subruleSequence << "\n";
			set<int> nextOffsets = {0};

			// Decompose sequence of rules
			for (auto &&subrule : subruleSequence) {

				// Test for each possible offsets
				set<int> temp{};
				for (auto &&nextOffset : nextOffsets) {
					auto ruleOffsets = checkRule(subrule, word, nonTermRules, termRules, i + nextOffset, spaces + 2);
					transform(ruleOffsets.begin(), ruleOffsets.end(), inserter(temp, temp.end()),
					          [nextOffset](int o) {return nextOffset + o;});
				}
				nextOffsets = std::move(temp);
			}

//			if (!nextOffsets.empty()) {
//				cout << genSpaces(spaces) << "sequence matched\n";
//			} else {
//				cout << genSpaces(spaces) << "sequence not matched\n";
//			}
			ret.merge(nextOffsets);
		}
		return ret;
	}
}

int main() {
	// temps courant, avant l'execution
	auto time1 = chrono::high_resolution_clock::now();


	auto lines = readFileLines("../input19.txt");
	set<string> words{};
	map<int, set<vector<int>>> nonTermRules{};
	map<int, char> termRules{};

	// Part 2
	nonTermRules[8].insert({42, 8});
	nonTermRules[11].insert({42, 11, 31});


	bool wordsPart = false;
	for (auto &&line : lines) {
		if (wordsPart) {
			words.insert(line);
		} else {
			if (line.empty()) {
				wordsPart = true;
			} else {
				auto lineSplit = splitLine(line, ": ");
				int ruleNb = stoi(lineSplit[0]);
				auto ruleBody = lineSplit[1];
				if (ruleBody[0] == '"') {
					// Case terminal
					termRules[ruleNb] = ruleBody[1];
				} else {
					// Case non-terminal
					auto ruleBodySplit = splitLine(ruleBody, " | ");
					for (auto &&term : ruleBodySplit) {
						vector<int> subrules{};
						auto termSplit = splitLine(term, " ");
						subrules.reserve(termSplit.size());
						for (auto &&subrule : termSplit) {
							subrules.push_back(stoi(subrule));
						}
						nonTermRules[ruleNb].insert(subrules);
					}
				}
			}
		}
	}

	cout << termRules << endl;
	cout << endl;
	cout << nonTermRules << endl;

	long sum = 0;
	for (auto &&word : words) {
		set<int> charMatched = checkRule(0, word, nonTermRules, termRules, 0, 0);
		bool wordRecognized = find(charMatched.begin(), charMatched.end(), word.size()) != charMatched.end();
		sum += wordRecognized;
//		cout << word << ": " << to_string(wordRecognized) << "\n";
	}

	cout << "\nTotal: " << sum << " words recognized\n";

	// temps courant, apres l'execution
	auto time2 = chrono::high_resolution_clock::now();

	// mesurer la difference, et l'exprimer en microsecondes
	unsigned int time = chrono::duration_cast<chrono::microseconds>(time2 - time1).count();

	cout << "\nTemps d'exécution = " << float(time) / 1000.0f << "ms" << endl;

}
