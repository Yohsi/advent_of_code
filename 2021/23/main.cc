#include "aoclib.hpp"
#include <ranges>
#include <cmath>


struct Day23 : public Aoc {
	string hallway = "...........";
	array<vector<char>, 4> rooms{};
	array<array<char, 2>, 4> input_rooms{};
	const array<int, 7> possible_h{0, 1, 3, 5, 7, 9, 10};

	void print_rooms() const {
		cout << "#############\n"
		     << "#" << hallway << "#\n"
		     << "###";
		for (int j = 0; j < rooms[0].size(); ++j) {
			for (int i = 0; i < 4; ++i) {
				cout << rooms[i][j] << "#";
			}
			if (j == 0) {
				cout << "##";
			}
			cout << "\n  #";
		}
		cout << "########\n";
	}

	static int room_location_in_hallway(int r) {
		return 2 + 2 * r;
	}

	[[nodiscard]] bool hallway_free_between(int h1, int h2) const {
		for (int i = min(h1, h2); i <= max(h1, h2); ++i) {
			if (hallway[i] != '.') return false;
		}
		return true;
	}

	[[nodiscard]] bool check_termination() const {
		for (int room = 0; room < 4; ++room) {
			for (int i = 0; i < 2; ++i) {
				if (rooms[room][i] != ('A' + room)) return false;
			}
		}
		return true;
	}

	static int amphipod_energy(char amphipod) {
		switch (amphipod) {
			case 'A':
				return 1;
			case 'B':
				return 10;
			case 'C':
				return 100;
			case 'D':
				return 1000;
			default:
				assert(false);
				return 0;
		}
	}

	optional<long> backtrack(int max_depth) {
		if (check_termination()) {
			return 0;
		}

		if (max_depth == 0) return nullopt;

//		print_rooms();
//		cout << '\n';

		optional<long> min_energy = nullopt;

		// Move an amphipod out of its room
		for (int room_nb = 0; room_nb < 4; ++room_nb) {
			vector<char> &room = rooms[room_nb];

			bool skip = true;
			for (auto a: room) {
				skip &= a == '.' || a == ('A' + room_nb);
			}
			if (skip) continue;

			// To the hallway
			for (auto h_next: possible_h) {
				int h_prev = room_location_in_hallway(room_nb);
				if (hallway_free_between(h_prev, h_next)) {
					for (int i = 0; i < room.size(); ++i) {
						auto amphipod = room[i];
						if (amphipod != '.') {
							// move the amphipod
							room[i] = '.';
							hallway[h_next] = amphipod;

							// recurse
							auto res = backtrack(max_depth - 1);
							if (res) {
								long path_length = (abs(h_next - h_prev) + i + 1);
								long energy = path_length * amphipod_energy(amphipod) + *res;
								min_energy = min_energy.has_value() ? min(*min_energy, energy) : energy;
							}

							// reset
							room[i] = amphipod;
							hallway[h_next] = '.';

							break;
						}
					}
				}
			}

			// To its destination
			int place_orig = -1;
			for (int i = 0; i < room.size(); i++) {
				if (room[i] != '.') {
					place_orig = i;
					break;
				}
			}
			if (place_orig == -1) continue;

			char amphipod = room[place_orig];
			int dest_room_nb = amphipod - 'A';
			auto &dest_room = rooms[dest_room_nb];
			assert(0 <= dest_room_nb && dest_room_nb < 4);
			int h_next = room_location_in_hallway(dest_room_nb);
			int h_prev = room_location_in_hallway(room_nb);
			if (!hallway_free_between(h_prev, h_next)) continue;
			// check that the destination doesn't contain other amphipods
			int place_dest = -1;
			for (auto a: dest_room) {
				if (a == '.')
					place_dest++;
				else if (a != amphipod)
					place_dest = -1;
			}
			if (place_dest == -1) continue;

			// move the amphipod
			room[place_orig] = '.';
			dest_room[place_dest] = amphipod;

			// recurse
			auto res = backtrack(max_depth - 1);
			if (res) {
				long path_length = (abs(h_next - h_prev) + place_orig + place_dest + 2);
				long energy = path_length * amphipod_energy(amphipod) + *res;
				min_energy = min_energy.has_value() ? min(*min_energy, energy) : energy;
			}

			// reset
			room[place_orig] = amphipod;
			dest_room[place_dest] = '.';
		}

		// Move an amphipod from the hallway to its room
		for (int h_prev = 0; h_prev < 11; ++h_prev) {
			char amphipod = hallway[h_prev];
			if (amphipod == '.') continue;
			int dest_room_nb = amphipod - 'A';
			auto &dest_room = rooms[dest_room_nb];
			assert(0 <= dest_room_nb && dest_room_nb < 4);
			int h_next = room_location_in_hallway(dest_room_nb);
			int dir = h_prev < h_next ? 1 : -1;
			if (!hallway_free_between(h_prev + dir, h_next)) continue;
			// check that the destination doesn't contain other amphipods
			int place_dest = -1;
			for (auto a: dest_room) {
				if (a == '.')
					place_dest++;
				else if (a != amphipod)
					place_dest = -1;
			}
			if (place_dest == -1) continue;

			// move the amphipod
			hallway[h_prev] = '.';
			dest_room[place_dest] = amphipod;

			// recurse
			auto res = backtrack(max_depth - 1);
			if (res) {
				long path_length = (abs(h_next - h_prev) + place_dest + 1);
				long energy = path_length * amphipod_energy(amphipod) + *res;
				min_energy = min_energy.has_value() ? min(*min_energy, energy) : energy;
			}

			// reset
			hallway[h_prev] = amphipod;
			dest_room[place_dest] = '.';
		}
		return min_energy;
	}

	explicit Day23(ifstream &f) : Aoc{f} {
		auto input = readLines(f);
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 2; ++j) {
				input_rooms[i][j] = input[2 + j][3 + 2 * i];
			}
		}
	}

	ulong part1() override {
		for (int i = 0; i < rooms.size(); i++) {
			copy(input_rooms[i].begin(), input_rooms[i].end(), back_inserter(rooms[i]));
		}
		print_rooms();
		auto res = backtrack(20);
		if (!res.has_value()) {
			cout << red << "No solution\n";
		}
		return res.value_or(0);
	}

	ulong part2() override {
		array<string, 2> to_add = {"DCBA", "DBAC"};
		hallway = "...........";
		for (int i = 0; i < rooms.size(); i++) {
			rooms[i].clear();
			rooms[i].push_back(input_rooms[i][0]);
			rooms[i].push_back(to_add[0][i]);
			rooms[i].push_back(to_add[1][i]);
			rooms[i].push_back(input_rooms[i][1]);
		}
		print_rooms();
		auto res = backtrack(30);
		if (!res.has_value()) {
			cout << red << "No solution\n";
		}
		return res.value_or(0);
	}
};

int main() {
	Aoc::run<Day23>(12521, 44169);
}
