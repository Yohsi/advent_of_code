#include "aoclib.hpp"
#include <ranges>


struct Day17 : public Aoc {
	long xmin;
	long xmax;
	long ymin;
	long ymax;
	uset<long> allowed_vx;
	long minvx = 10e9;

	explicit Day17(ifstream &f) : Aoc{f} {
		f >> xmin >> xmax >> ymin >> ymax;
		for (long vx = 0; vx <= xmax; ++vx) {
			long xend = vx * (vx + 1) / 2;
			if (xmin <= xend && xend <= xmax) {
				allowed_vx.emplace(vx);
				minvx = min(minvx, vx);
			}
		}
	}

	bool traj_valid(long vx, long vy) const {
		long x = 0;
		long y = 0;
		while (x <= xmax && y >= ymin) {
			if (xmin <= x && x <= xmax &&
			    ymin <= y && y <= ymax) {
				return true;
			}
			x += vx;
			y += vy;
			if (vx < 0)
				vx += 1;
			else if (vx > 0)
				vx -= 1;
			vy -= 1;
		}
		return false;
	}

	long max_vy = 0;

	ulong part1() override {
		long max_height = 0;
		for (long vy = 0; vy < 10000; ++vy) {
			long step = minvx;
			long y = 0;
			while (y >= ymin) {
				long height = vy * (vy + 1) / 2;
				y = height - (step - vy) * (step - vy + 1) / 2;
				if (ymin <= y && y <= ymax) {
					max_vy = max(max_vy, vy);
					max_height = max(max_height, height);
				}
				step++;
			}
		}
		return max_height;
	}

	ulong part2() override {
		ulong count = 0;
		for (long vx = 1; vx <= xmax; ++vx) {
			for (long vy = ymin; vy <= max_vy; ++vy) {
				bool valid = traj_valid(vx, vy);
				count += valid;
			}
		}
		return count;
	}
};

int main() {
	Aoc::run<Day17>(45, 112);
}



// x = vx + vx-1 + vx-2 + ... + vx
// x = vx*(vx+1)/2
// x <= xmax <=>
// vx*(vx+1)/2 <= xmax
// x >= xmin <=>
// vx*(vx+1)/2 >= xmin


// step <= vx

// y = vy + vy-1 + vy-2 + ... + 0 + -1 + -2
// y = vy*(vy+1)/2 - (step - vy) * (step - vy + 1) / 2
//   = vy*(vy+1)/2 - (step² - 2*vy*step + step + vy² - vy)
//   = -step² + (2*vy - 1)*step - vy²/2 + 3*vy/2
//
