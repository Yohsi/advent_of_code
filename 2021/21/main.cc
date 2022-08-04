#include "aoclib.hpp"
#include <ranges>

constexpr array<ushort, 10> dices{0, 0, 0, 1, 3, 6, 7, 6, 3, 1};

struct State {
	State() = default;

	State(ushort pos1, ushort pos2) : positions{pos1, pos2} {}

	array<ushort, 2> scores{0, 0};
	array<ushort, 2> positions{0, 0};

	[[nodiscard]] map<State, ulong> turn(int player) const {
		map<State, ulong> states;
		for (ushort res = 3; res <= 9; ++res) {
			State new_state = *this;
			new_state.positions[player] = (positions[player] + res - 1) % 10 + 1;
			new_state.scores[player] += new_state.positions[player];
			states[new_state] += dices[res];
		}
		return states;
	}

	stream_io(State, scores, positions);
	auto operator<=>(const State &) const = default;
};

struct Day21 : public Aoc {
	ushort p1_initial_position;
	ushort p2_initial_position;

	array<ulong, 2> scores{0, 0};
	array<ushort, 2> positions;
	ulong nb_rolls = 0;
	std::function<ulong()> roll_dice = [i = 0, this]() mutable {
		nb_rolls++;
		i = (i % 100) + 1;
		return i;
	};

	explicit Day21(ifstream &f) : Aoc{f} {
		f >> p1_initial_position >> p2_initial_position;
	}

	void turn(int player) {
		auto res = roll_dice() + roll_dice() + roll_dice();
		positions[player] = (positions[player] + res - 1) % 10 + 1;
		scores[player] += positions[player];
	}

	ulong part1() override {
		positions = {p1_initial_position, p2_initial_position};
		int p = 1;
		while (scores[p] < 1000) {
			p = !p;
			turn(p);
		}
		return scores[!p] * nb_rolls;
	}

	ulong part2() override {
		map<State, ulong> states{{State{p1_initial_position, p2_initial_position}, 1}};
		map<State, ulong> final_states;
		int player = 1;
		while (!states.empty()) {
			player = !player;
			map<State, ulong> states2;
			for (auto &&[state, nb]: states) {
				if (state.scores[0] < 21 && state.scores[1] < 21) {
					auto next_states = state.turn(player);
//					print(state);
//					print(next_states);
					for (auto &&[next_state, nb_next]: next_states) {
						states2[next_state] += nb * nb_next;
					}
				} else {
					final_states[state] += nb;
				}
			}
			states = move(states2);
			print(states);
		}

		array<ulong, 2> nb_wins{0, 0};
		for (auto &&[state, nb]: final_states) {
			nb_wins[state.scores[0] >= 21] += nb;
		}
		print(nb_wins);

		return ranges::max(nb_wins);
	}
};

int main() {
	Aoc::run<Day21>(739785, 444356092776315ul, true);
}
