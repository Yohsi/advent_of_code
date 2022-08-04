#include "lib.h"


unsigned long long countArrangementWithDiff(list<int> &diff, list<int>::iterator begin) {
	if (diff.size() == 1 || diff.empty()) {
		return 1;
	}

	auto it = begin;
	auto last = --diff.end();

	unsigned long long sum = 1;
	list<int> savedElement{};
	while (it != last) {
		int val1 = *it;
		auto tmp = it;
		++it;
		int val2 = *it;
		(*it) += val1;
		savedElement.splice(savedElement.end(), diff, tmp);
		if ((*it) <= 3) {
			sum += countArrangementWithDiff(diff, it);
		}
		*it = val2;
		diff.splice(it, savedElement, savedElement.begin());
	}
	return sum;
}

unsigned long long splitAndCount(list<int> &diff) {
	unsigned long long ret = 1;
	auto it = diff.begin();
	auto end = diff.end();
	while (it != end) {
		auto it2 = find(it, end, 3);
		list<int> sublist{};
		sublist.splice(sublist.end(), diff, it, it2);
		ret *= countArrangementWithDiff(sublist, sublist.begin());
		it = it2;
		if (it != end)
			++it;
	}
	return ret;
}


int main() {
	// temps courant, avant l'execution
	auto time1 = chrono::high_resolution_clock::now();

	ifstream file;
	file.open("../input10.txt");

	if (file.fail()) {
		throw runtime_error("Impossible d'ouvrir le fichier");
	}

	vector<int> list{};
	int current;

	file >> current;
	while (!file.eof()) {
		list.push_back(current);
		file >> current;
	}

	// Add the built-in Joltage adapter
	list.push_back(0);

	sort(list.begin(), list.end());
	// Add the charging outlet
	list.push_back(list[list.size() - 1] + 3);


	// Part 1

	vector<int> diff{};
	diff.reserve(list.size());
	adjacent_difference(list.begin(), list.end(), back_inserter(diff));

//	int diff1 = count(diff.begin(), diff.end(), 1);
//	int diff3 = count(diff.begin(), diff.end(), 3);
//
//	cout << "1 jolt  diff  : " << diff1 << '\n';
//	cout << "3 jolts diff  : " << diff3 << '\n';
//	cout << "multiplication: " << diff1 * diff3 << '\n';

	// Part 2
	std::list<int> diffList{diff.begin() + 1, diff.end()};
	unsigned long long nbArr = splitAndCount(diffList);
	cout << "\nNumber of arrangements: " << nbArr << "\n";


	// temps courant, apres l'execution
	auto time2 = chrono::high_resolution_clock::now();

	// mesurer la difference, et l'exprimer en microsecondes
	unsigned int time = chrono::duration_cast<chrono::microseconds>(time2 - time1).count();

	cout << "\nTemps d'exécution = " << float(time) / 1000.0f << "ms" << endl;

	return 0;
}
