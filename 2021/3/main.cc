#include "aoclib.hpp"

struct Day3 : public Aoc {
	vector<string> report;
	unordered_set<ulong> co2;
	unordered_set<ulong> oxygen;
	size_t nbBits;
	size_t nbReports;

	explicit Day3(ifstream &f) : Aoc{f} {
		parse(file, report);
		nbReports = report.size();
		nbBits = report[0].size();
	}

	ulong part1() override {
		vector<ulong> sum(nbBits, 0);
		for (auto &&str: report) {
			for (int i = 0; i < str.size(); ++i) {
				sum[i] += (str[i] == '1');
			}
		}
		vector<bool> mostCommonValues;
		mostCommonValues.reserve(sum.size());
		auto half = report.size() / 2;
		transform(sum.begin(), sum.end(), back_inserter(mostCommonValues), [half](ulong s) {
			return s >= half;
		});
		ulong gamma = 0;
		ulong epsilon = 0;
		for (int i = 0; i < nbBits; ++i) {
			gamma += mostCommonValues[i] << (nbBits - i - 1);
			epsilon += !mostCommonValues[i] << (nbBits - i - 1);
		}
		print(gamma);
		print(epsilon);
		return gamma * epsilon;
	}

	ulong mask(uint i) const {
		return 1 << (nbBits - i - 1);
	}

	ulong sumColumn(const unordered_set<ulong> &s, size_t i) const {
		ulong sum = 0;
		for (auto &&val: s) {
			sum += (val & mask(i)) != 0;
		}
		return sum;
	}


	ulong part2() override {
		auto bin2long = [](const std::string &s) {
			return stoul(s, nullptr, 2);
		};
		transform(report.begin(), report.end(), inserter(co2, co2.begin()), bin2long);
		transform(report.begin(), report.end(), inserter(oxygen, oxygen.begin()), bin2long);

		size_t i = 0;
		while (co2.size() > 1 || oxygen.size() > 1) {
			if (co2.size() > 1) {
				auto sum = sumColumn(co2, i);
				bool bitCriteria = sum * 2 < co2.size();
				std::erase_if(co2, [bitCriteria, i, this](ulong val) {
					return (val & mask(i)) >> (nbBits - i - 1) != bitCriteria;
				});
			}
			if (oxygen.size() > 1) {
				auto sum = sumColumn(oxygen, i);
				bool bitCriteria = sum * 2 >= oxygen.size();
				std::erase_if(oxygen, [bitCriteria, i, this](ulong val) {
					return ((val & mask(i)) >> (nbBits - i - 1)) != bitCriteria;
				});
			}
			i = (i + 1) % nbBits;
		}

		auto co2Rate = *co2.begin();
		print(co2Rate);

		auto oxyRate = *oxygen.begin();
		print(oxyRate);

		return oxyRate * co2Rate;
	}
};


int main() {
	Aoc::run<Day3>(198, 230);
}
