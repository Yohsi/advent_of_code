#include <ostream>
#include "aoclib.hpp"

struct Cmd {
	string direction;
	long val = 0;

	friend ostream &operator<<(ostream &os, const Cmd &cmd) {
		os << "direction: " << cmd.direction << " val: " << cmd.val;
		return os;
	}

	friend istream &operator>>(istream &is, Cmd &cmd) {
		return is >> cmd.direction >> cmd.val;
	}
};

struct Day2 : public Aoc {
	vector<Cmd> cmds;
	long h = 0;
	long d = 0;
	long a = 0;

	explicit Day2(ifstream &f) : Aoc{f} {
		parse(f, cmds);
	}

	ulong part1() override {
		for (auto &&cmd: cmds) {
			if (cmd.direction == "forward") {
				h += cmd.val;
			} else if (cmd.direction == "up") {
				d -= cmd.val;
			} else if (cmd.direction == "down") {
				d += cmd.val;
			}
		}
		print(h);
		print(d);
		return h * d;
	}

	ulong part2() override {
		h = 0;
		d = 0;
		for (auto &&cmd: cmds) {
			if (cmd.direction == "forward") {
				h += cmd.val;
				d += a * cmd.val;
			} else if (cmd.direction == "up") {
				a -= cmd.val;
			} else if (cmd.direction == "down") {
				a += cmd.val;
			}
		}
		print(h);
		print(d);
		print(a);
		return h * d;
	}
};

int main() {
	Aoc::run<Day2>(150, 900);
}
