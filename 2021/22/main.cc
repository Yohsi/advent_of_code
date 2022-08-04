#include "aoclib.hpp"
#include <ranges>


struct Range {
	long min, max;

	[[nodiscard]] ulong interval() const {
		return valid() ? max - min : 0;
	}

	[[nodiscard]] bool valid() const {
		return max >= min;
	}

	[[nodiscard]] bool empty() const {
		return min == max;
	}

	auto operator<=>(const Range &o) const = default;

	stream_io(Range, min, max);
};


struct Cuboid {
	Range x, y, z;

	[[nodiscard]] ulong count_cubes() const {
		return x.interval() * y.interval() * z.interval();
	}

	[[nodiscard]] bool valid() const {
		return x.valid() && y.valid() && z.valid();
	}

	[[nodiscard]] bool empty() const {
		return x.empty() || y.empty() || z.empty();
	}

	auto operator<=>(const Cuboid &o) const = default;

	friend ostream &operator<<(ostream &os, const Cuboid &c) {
		os << "Cuboid(" << c.x.min << ".." << c.x.max << "," <<
		   c.y.min << ".." << c.y.max << "," <<
		   c.z.min << ".." << c.z.max << ")";
		return os;
	}
};

struct Instruction {
	bool on;
	Cuboid cuboid;
};


Cuboid intersection(const Cuboid &a, const Cuboid &b) {
	Cuboid inter{};
	inter.x.min = max(a.x.min, b.x.min);
	inter.x.max = min(a.x.max, b.x.max);
	inter.y.min = max(a.y.min, b.y.min);
	inter.y.max = min(a.y.max, b.y.max);
	inter.z.min = max(a.z.min, b.z.min);
	inter.z.max = min(a.z.max, b.z.max);
	return inter.valid() ? inter : Cuboid{};
}

set<Cuboid> difference(const Cuboid &a, const Cuboid &b) {
	set<Cuboid> diff{};

	auto bxmin = max(a.x.min, b.x.min);
	auto bymin = max(a.y.min, b.y.min);
	auto bzmin = max(a.z.min, b.z.min);
	auto bxmax = min(a.x.max, b.x.max);
	auto bymax = min(a.y.max, b.y.max);
	auto bzmax = min(a.z.max, b.z.max);

	Cuboid c{};
	c.x.min = a.x.min;
	c.x.max = bxmin;
	c.y.min = a.y.min;
	c.y.max = a.y.max;
	c.z.min = a.z.min;
	c.z.max = a.z.max;
	if (c.valid() && !c.empty())
		diff.insert(c);

	c.x.min = bxmax;
	c.x.max = a.x.max;
	c.y.min = a.y.min;
	c.y.max = a.y.max;
	c.z.min = a.z.min;
	c.z.max = a.z.max;
	if (c.valid() && !c.empty())
		diff.insert(c);

	c.x.min = bxmin;
	c.x.max = bxmax;
	c.y.min = a.y.min;
	c.y.max = bymin;
	c.z.min = a.z.min;
	c.z.max = a.z.max;
	if (c.valid() && !c.empty())
		diff.insert(c);

	c.x.min = bxmin;
	c.x.max = bxmax;
	c.y.min = bymax;
	c.y.max = a.y.max;
	c.z.min = a.z.min;
	c.z.max = a.z.max;
	if (c.valid() && !c.empty())
		diff.insert(c);

	c.x.min = bxmin;
	c.x.max = bxmax;
	c.y.min = bymin;
	c.y.max = bymax;
	c.z.min = a.z.min;
	c.z.max = bzmin;
	if (c.valid() && !c.empty())
		diff.insert(c);

	c.x.min = bxmin;
	c.x.max = bxmax;
	c.y.min = bymin;
	c.y.max = bymax;
	c.z.min = bzmax;
	c.z.max = a.z.max;
	if (c.valid() && !c.empty())
		diff.insert(c);

	return diff;
}

struct Day22 : public Aoc {
	vector<Instruction> instructions;

	explicit Day22(ifstream &f) : Aoc{f} {
		for (auto &&l: readLines(f)) {
			if (l.empty()) continue;
			auto split = splitLine(l, " ");
			Instruction i{};
			i.on = split[0] == "on";
			istringstream ss{split[1]};
			ss >> "x=" >> i.cuboid.x.min >> ".." >> i.cuboid.x.max >> ",";
			ss >> "y=" >> i.cuboid.y.min >> ".." >> i.cuboid.y.max >> ",";
			ss >> "z=" >> i.cuboid.z.min >> ".." >> i.cuboid.z.max;
			i.cuboid.x.max++;
			i.cuboid.y.max++;
			i.cuboid.z.max++;
			instructions.push_back(i);
		}
	}

	ulong part1() override {
		deque<Cuboid> cubes_on{};
		Cuboid init_cube{};
		init_cube.x.min = -50;
		init_cube.x.max = 50;
		init_cube.y.min = -50;
		init_cube.y.max = 50;
		init_cube.z.min = -50;
		init_cube.z.max = 50;

		for (auto &&ins: instructions) {
			if (intersection(ins.cuboid, init_cube).empty()) continue;
			deque<Cuboid> cubes_on2{};
			for (auto &&cube_on: cubes_on) {
				auto inter = intersection(cube_on, ins.cuboid);
				if (inter.empty()) {
					cubes_on2.emplace_back(cube_on);
				} else {
					auto diff_cube_ins = difference(cube_on, ins.cuboid);
					move(diff_cube_ins.begin(), diff_cube_ins.end(), inserter(cubes_on2, cubes_on2.end()));
				}
			}
			cubes_on = move(cubes_on2);
			if (ins.on) {
				cubes_on.emplace_back(ins.cuboid);
			}
		}

		return std::accumulate(cubes_on.begin(), cubes_on.end(), 0ul,
		                       [](ulong a, const Cuboid &c) {return a + c.count_cubes();});



//		for (auto &ins: instructions) {
//			if (!intersection(ins.cuboid, init_cube).empty()) {
//				for (long x = ins.cuboid.x.min; x <= ins.cuboid.x.max; ++x) {
//					for (long y = ins.cuboid.y.min; y <= ins.cuboid.y.max; ++y) {
//						for (long z = ins.cuboid.z.min; z <= ins.cuboid.z.max; ++z) {
//							cubes_on[{x, y, z}] = ins.on;
//						}
//					}
//				}
//			}
//		}
//
//		ulong nb_on = 0;
//		for (auto &&[vec, on]: cubes_on) {
//			nb_on += on;
//		}
//
//		return nb_on;
	}

	ulong part2() override {
		deque<Cuboid> cubes_on{};

		for (auto &&ins: instructions) {
			deque<Cuboid> cubes_on2{};
			for (auto &&cube_on: cubes_on) {
				auto inter = intersection(cube_on, ins.cuboid);
				if (inter.empty()) {
					cubes_on2.emplace_back(cube_on);
				} else {
					auto diff_cube_ins = difference(cube_on, ins.cuboid);
					move(diff_cube_ins.begin(), diff_cube_ins.end(), inserter(cubes_on2, cubes_on2.end()));
				}
			}
			cubes_on = move(cubes_on2);
			if (ins.on) {
				cubes_on.emplace_back(ins.cuboid);
			}
		}

		return std::accumulate(cubes_on.begin(), cubes_on.end(), 0ul,
		                       [](ulong a, const Cuboid &c) {return a + c.count_cubes();});

	}
};

int main() {
	Aoc::run<Day22>(474140, 2758514936282235ul, true);
}
