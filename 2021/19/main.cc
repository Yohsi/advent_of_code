#include "aoclib.hpp"
#include <ranges>
#include <cmath>

Vec3 rotate_helper(const Vec3 &orig, int rotation) {
	switch (rotation) {
		case 0:
			return {orig.x, orig.y, orig.z};
		case 1:
			return {orig.x, orig.z, -orig.y};
		case 2:
			return {orig.x, -orig.y, -orig.z};
		case 3:
			return {orig.x, -orig.z, orig.y};
		default:
			assert(false);
	}
}
struct beacon {
	int x,y,z;
};


Vec3 rotate(const Vec3 &orig, int rotation) {
	assert(rotation >= 0);
	if (rotation < 4)
		return rotate_helper(orig, rotation % 4);
	if (rotation < 8)
		return rotate_helper({-orig.x, -orig.y, orig.z}, rotation % 4);
	if (rotation < 12)
		return rotate_helper({orig.y, -orig.x, orig.z}, rotation % 4);
	if (rotation < 16)
		return rotate_helper({-orig.y, orig.x, orig.z}, rotation % 4);
	if (rotation < 20)
		return rotate_helper({orig.z, orig.y, -orig.x}, rotation % 4);
	if (rotation < 24)
		return rotate_helper({-orig.z, orig.y, orig.x}, rotation % 4);
	assert(false);
}

ulong distance_squared(const Vec3 &a, const Vec3 &b) {
	long x_diff = a.x - b.x;
	long y_diff = a.y - b.y;
	long z_diff = a.z - b.z;
	return x_diff * x_diff + y_diff * y_diff + z_diff * z_diff;
}

ulong manhattan_distance(const Vec3 &a, const Vec3 &b) {
	long x_diff = a.x - b.x;
	long y_diff = a.y - b.y;
	long z_diff = a.z - b.z;
	return abs(x_diff) + abs(y_diff) + abs(z_diff);
}

struct Day19 : public Aoc {
	vector<uset<Vec3>> scanner_reports{};
	map<pair<uint, uint>, vector<Vec3>> translations{};
	map<pair<uint, uint>, vector<ushort>> rotations{};

	explicit Day19(ifstream &f) : Aoc{f} {
		for (auto &&line: readLines(f)) {
			if (line[1] == '-') {
				scanner_reports.emplace_back();
			} else if (!line.empty()) {
				auto s = splitLine(line, ",");
				long x = atol(s[0].c_str());
				long y = atol(s[1].c_str());
				long z = atol(s[2].c_str());
				scanner_reports.back().emplace(x, y, z);
			}
		}
	}

	ulong part1() override {
		vector<umap<ulong, set<pair<Vec3, Vec3>>>> distances{};
		distances.reserve(scanner_reports.size());
		for (int i = 0; i < scanner_reports.size(); ++i) {
			distances.emplace_back();
			for (auto &&b1: scanner_reports[i]) {
				for (auto &&b2: scanner_reports[i]) {
					if (b1 != b2) {
						distances[i][distance_squared(b1, b2)].insert(make_pair(b1, b2));
					}
				}
			}
		}
//		print(distances);

		for (int ref_id = 0; ref_id < scanner_reports.size(); ref_id++) {
			auto &ref_dist = distances[ref_id];

			for (int i = 0; i < scanner_reports.size(); ++i) {
				if (i == ref_id) continue;

//				cout << light_yellow << "report " << i << '\n';
				array<map<Vec3, ulong>, 24> possible_translations;
				map<ushort, ulong> possible_rotations;
				for (auto &&[dist, coord_pairs]: distances[i]) {
//				cout << light_gray << "distance " << dist << " (" << coord_pairs.size() << " pairs: " << coord_pairs << ")" << '\n';
					if (ref_dist.contains(dist)) {
						for (auto &&[v1, v2]: coord_pairs) {
							for (auto &&[v1_ref, v2_ref]: ref_dist[dist]) {
								for (int rot_id = 0; rot_id < 24; ++rot_id) {
//								print(rot_id, red);
									auto v1_rot = rotate(v1, rot_id);
									auto v2_rot = rotate(v2, rot_id);
//								print(v1_ref, magenta);
//								print(v2_ref, magenta);
//								print(v1_rot);
//								print(v2_rot);
									auto diff11 = v1_ref - v1_rot;
									auto diff22 = v2_ref - v2_rot;
									auto diff12 = v2_ref - v1_rot;
									auto diff21 = v1_ref - v2_rot;
									if (diff11 == diff22) {
//									cout << light_magenta << "diff11 = diff22 = " << diff11 << '\n';
										possible_translations[rot_id][diff11]++;
										possible_rotations[rot_id]++;
									}
									if (diff12 == diff21) {
//									cout << light_magenta << "diff12 = diff21 = " << diff12 << '\n';
										possible_translations[rot_id][diff12]++;
										possible_rotations[rot_id]++;
									}
								}
							}
						}
					}
				}
				auto p = make_pair(ref_id, i);

				ulong current_max = 12;
				for (auto &&[rot, nb]: possible_rotations) {
					if (nb >= current_max) {
						current_max = nb;
						rotations[p] = {rot};
					}
				}
				if (!rotations[p].empty()) {
					current_max = 12;
					for (auto &&[tran, nb]: possible_translations[rotations[p][0]]) {
						if (nb >= current_max) {
							current_max = nb;
							translations[p] = {tran};
						}
					}
				}
			}
		}

		set<Vec3> all_coords{};
		for (auto &&coord: scanner_reports[0]) {
			all_coords.insert(coord);
		}


		set<uint> to_process{};
		generate_n(inserter(to_process, to_process.end()), scanner_reports.size() - 1, [i = 0]() mutable {return ++i;});
		deque<uint> processed{0};

		while (!to_process.empty()) {
			for (auto &&id: to_process) {
				bool found = false;
				for (auto &&ref_id: processed) {
					auto p_ref = make_pair(ref_id, id);
					auto p_0 = make_pair(0, id);
					if (rotations[p_ref].size() == 1) {
						if (translations[p_0].empty()) {
							translations[p_0] = translations[{0, ref_id}];
							translations[p_0].insert(translations[p_0].begin(), translations[p_ref][0]);
						}
						if (rotations[p_0].empty()) {
							rotations[p_0] = rotations[{0, ref_id}];
							rotations[p_0].insert(rotations[p_0].begin(), rotations[p_ref][0]);
						}

//						print(ref_id);
//						print(id);
						auto transform = [&](Vec3 v) {
							for (int i = 0; i < translations[p_0].size(); ++i) {
								v = translations[p_0][i] + rotate(v, rotations[p_0][i]);
							}
							return v;
						};
						for (auto &&coord: scanner_reports[id]) {
							auto &&transformed = transform(coord);
//							if (all_coords.contains(transformed)) {
//								print(coord, green);
//								print(transformed, yellow);
//							}
//							nb_duplicates += all_coords.contains(transformed);
							all_coords.insert(transformed);
						}
						found = true;
						to_process.erase(id);
						processed.push_front(id);
						break;
					}
				}
				if (found) {
					break;
				}
			}
		}
		return all_coords.size();
	}

	ulong part2() override {
		ulong max_dist = 0;
		for (int i = 0; i < scanner_reports.size(); ++i) {
			for (int j = 0; j < scanner_reports.size(); ++j) {
				Vec3 trans1{};
				Vec3 trans2{};
				for (int k = 0; k < translations[{0, i}].size(); ++k) {
					trans1 = translations[{0, i}][k] + rotate(trans1, rotations[{0, i}][k]);
				}
				for (int k = 0; k < translations[{0, j}].size(); ++k) {
					trans2 = translations[{0, j}][k] + rotate(trans2, rotations[{0, j}][k]);
				}
				max_dist = max(max_dist, manhattan_distance(trans1, trans2));
			}
		}
		return max_dist;
	}
};

int main() {
	Aoc::run<Day19>(79, 3621, true);
}
