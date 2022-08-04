#include "aoclib.hpp"
#include <ranges>

struct Board {
	array<array<ulong, 5>, 5> board;

	[[nodiscard]] bool hasAlignment(const unordered_set<ulong> &drawnNbs) const {
		return hasVertAlign(drawnNbs) || hasHorizAlign(drawnNbs);
	}

	[[nodiscard]] bool hasVertAlign(const unordered_set<ulong> &drawnNbs) const {
		auto cols = {0, 1, 2, 3, 4};
		return any_of(cols.begin(), cols.end(), [&drawnNbs, this](auto &&col) {
			auto colNbMarked = count_if(board.begin(), board.end(), [col, &drawnNbs](auto &&line) {
				return drawnNbs.contains(line[col]);
			});
//			print(colNbMarked);
			return colNbMarked == 5;
		});
	}

	[[nodiscard]] bool hasHorizAlign(const unordered_set<ulong> &drawnNbs) const {
		return any_of(board.begin(), board.end(), [&drawnNbs](auto &&line) {
			auto linNbMarked = count_if(line.begin(), line.end(), [&drawnNbs](ulong val) {
				return drawnNbs.contains(val);
			});
//			print(line);
//			print(linNbMarked);
			return linNbMarked == 5;
		});
	}

	[[nodiscard]] ulong sumUnmarked(const unordered_set<ulong> &drawnNbs) const {
		return accumulate(board.begin(), board.end(), 0, [&drawnNbs](ulong a, auto &&line) {
			return accumulate(line.begin(), line.end(), a, [&drawnNbs](ulong a, ulong val) {
				return drawnNbs.contains(val) ? a : a + val;
			});
		});
	}

	stream_io(Board, board);
};


struct Day4 : public Aoc {
	vector<Board> boards;
	vector<ulong> drawOrder;

	explicit Day4(ifstream &f) : Aoc{f} {
		drawOrder = splitLine<vector<ulong>>(readLine(f), ",");
		Board b;
		while (f >> b) {
			boards.push_back(b);
		}
//		print(drawOrder);
//		print(boards);
	}

	ulong part1() override {
		int i = 4;
		unordered_set<ulong> drawnNbs(drawOrder.begin(), drawOrder.begin() + i + 1);
		auto it = boards.end();
		while (true) {
//			print(drawnNbs);
			it = find_if(boards.begin(), boards.end(), [&drawnNbs](const Board &b) {
				return b.hasAlignment(drawnNbs);
			});
			if (it != boards.end()) break;
			i++;
			drawnNbs.insert(drawOrder[i]);
		}
		auto lastDrawn = drawOrder[i];
		auto sum = it->sumUnmarked(drawnNbs);
//		print(sum);
//		print(lastDrawn);
//		print(drawnNbs);
//		print(*it);
		return sum * lastDrawn;
	}

	ulong part2() override {
		int i = 4;
		unordered_set<ulong> drawnNbs(drawOrder.begin(), drawOrder.begin() + i + 1);
		long losingBoardsNb = 0;
		while (true) {
//			print(drawnNbs);
			losingBoardsNb = count_if(boards.begin(), boards.end(), [&drawnNbs](const Board &b) {
				return !b.hasAlignment(drawnNbs);
			});
			if (losingBoardsNb == 0) break;
			i++;
			drawnNbs.insert(drawOrder[i]);
		}
		auto lastDrawn = drawOrder[i];
		drawnNbs.erase(lastDrawn);
		auto it = find_if(boards.begin(), boards.end(), [&drawnNbs](const Board &b) {
			return !b.hasAlignment(drawnNbs);
		});
		drawnNbs.insert(lastDrawn);
		auto sum = it->sumUnmarked(drawnNbs);
		print(sum);
		print(lastDrawn);
//		print(drawnNbs);
//		print(*it);
		return sum * lastDrawn;
	}
};

int main() {
	Aoc::run<Day4>(4512, 1924);
}
