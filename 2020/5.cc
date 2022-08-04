#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

const int MAX_LINE_SIZE = 100;
using namespace std;

int main() {
    cout << "processing..." << endl;

    ifstream file;
    file.open("../input5.txt");

    if (file.fail()) {
        cerr << "Impossible d'ouvrir le fichier" << endl;
        return 1;
    }

    vector<string> lines;

    char fileLine[MAX_LINE_SIZE];
    while (file.getline(fileLine, MAX_LINE_SIZE)) {
        lines.emplace_back(fileLine);
    }

    int maxSeatId = 0;
    int minSeatId = 1000;
    vector<int> ids{};

    for (auto &&line : lines) {
        int row = 0;
        int col = 0;
        int rHalf = 64;
        int cHalf = 4;

        for (char c : line) {
            if (c == 'F') {
                rHalf /= 2;
            } else if (c == 'B') {
                row += rHalf;
                rHalf /= 2;
            } else if (c == 'L') {
                cHalf /= 2;
            } else if (c == 'R') {
                col += cHalf;
                cHalf /= 2;
            }
        }

        int seatId = row * 8 + col;
        ids.push_back(seatId);

        if (seatId > maxSeatId) {
            maxSeatId = seatId;
        }
        if (seatId < minSeatId) {
            minSeatId = seatId;
        }
    }
    cout << "min = " << minSeatId << endl;
    cout << "max = " << maxSeatId << endl;

    sort(ids.begin(), ids.end());

    vector<int> increasingIds = vector<int>(maxSeatId - minSeatId);
    generate(increasingIds.begin(), increasingIds.end(), [current = minSeatId]() mutable {
        return current++;
    });

    auto p = mismatch(ids.begin(), ids.end(), increasingIds.begin());
    int id = *p.first - 1;

    cout << "missing = " << id << endl;

}