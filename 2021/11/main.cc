#include "aoclib.hpp"
#include <ranges>


struct Day11 : public Aoc {
	vector<string> input;
	vector<string> input1;

	explicit Day11(ifstream &f) : Aoc{f} {
		parse2(readBuf(f), input, "\n");
		input1 = input;
	}

	void print_input() {
		for (auto &&line: input1) {
			for (auto &&c: line) {
				if (c == 'X') {
					cout << red << c;
				} else if (c == ':') {
					cout << light_yellow << '@';
				} else {
					cout << dark_gray << c;
				}
			}
			cout << '\n';
		}
		cout << '\n';
	}

	ulong part1() override {
		ulong nb_flashes = 0;
		ulong prev_nb_flashes = 0;
//		print_input();

		for (int iteration = 0; iteration < 100; ++iteration) {
//			print(iteration, magenta);
			loop2d(input1, [](char &octopus) {octopus++;});
//			print_input();
			do {
				prev_nb_flashes = nb_flashes;
				loop2d(input1, [&](ulong l, ulong c, char &octopus) {
					if (octopus > '9' && octopus != 'X') {
						nb_flashes++;
						octopus = 'X';
						auto adjs = adjacents(input1, l, c);
						for (char &adj: adjs) {
							if (adj >= '0' && adj <= '9') {
								adj++;
							}
						}
					}
				});
//				print_input();
			} while (nb_flashes != prev_nb_flashes);
			loop2d(input1, [](char &octopus) {
				if (octopus == 'X') {
					octopus = '0';
				}
			});
//			print_input();
		}
		return nb_flashes;
	}

	ulong part2() override {
		ulong nb_flashes = 0;
		ulong prev_nb_flashes = 0;
		ulong iter = 0;
//		print_input();
		auto vsize = input.size();
		auto hsize = vsize == 0 ? 0 : input[0].size();

		while (nb_flashes < hsize * vsize) {
			nb_flashes = 0;
			iter++;
//			print(iteration, magenta);
			loop2d(input, [](char &octopus) {octopus++;});
//			print_input();
			do {
				prev_nb_flashes = nb_flashes;
				loop2d(input, [&](ulong l, ulong c, char &octopus) {
					if (octopus > '9' && octopus != 'X') {
						nb_flashes++;
						octopus = 'X';
						auto adjs = adjacents(input, l, c);
						for (char &adj: adjs) {
							if (adj >= '0' && adj <= '9') {
								adj++;
							}
						}
					}
				});
//				print_input();
			} while (nb_flashes != prev_nb_flashes);
			loop2d(input, [](char &octopus) {
				if (octopus == 'X') {
					octopus = '0';
				}
			});
//			print_input();
		}
		return iter;
	}
};

int main() {
	Aoc::run<Day11>(1656, 195);
}
