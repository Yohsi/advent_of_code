#include "aoclib.hpp"
#include <ranges>


struct Day25 : public Aoc {
	vector<string> g{};
	vector<string> g2{};
	size_t h, w;

	explicit Day25(ifstream &f) : Aoc{f} {
		string s;
		while (f >> s)
			g.emplace_back(s);

		h = g.size();
		w = g[0].size();
	}

	bool sim() {
		bool ret = true;
		g2 = g;
		for (size_t l = 0; l < h; ++l) {
			for (size_t c = 0; c < w; ++c) {
				if (g2[l][c] == '>') {
					auto new_l = l;
					auto new_c = (c + 1) % w;
					if (g2[new_l][new_c] == '.') {
						g[new_l][new_c] = g2[l][c];
						g[l][c] = '.';
						ret = false;
					}
				}
			}
		}
		g2 = g;
		for (size_t l = 0; l < h; ++l) {
			for (size_t c = 0; c < w; ++c) {
				if (g2[l][c] == 'v') {
					auto new_l = (l + 1) % h;
					auto new_c = c;
					if (g2[new_l][new_c] == '.') {
						g[new_l][new_c] = g2[l][c];
						g[l][c] = '.';
						ret = false;
					}
				}
			}
		}
		return ret;
	}

	ulong part1() override {
		long res = 1;
		print(g);
		while (!sim()) {
			res++;
			print(res);
			print(g);
		}
		return res;
	}

	ulong part2() override {

		return 0;
	}
};

int main() {
	Aoc::run<Day25>(58, 0);
}
