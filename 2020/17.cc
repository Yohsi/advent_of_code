#include "lib.h"

int countNeighbours(const deque<deque<deque<deque<bool>>>> &space, int px, int py, int pz, int pw) {
    int nb = 0;
    for (int w = max(pw - 1, 0); w < min(pw + 2, (int) space.size()); ++w) {
        for (int z = max(pz - 1, 0); z < min(pz + 2, (int) space[0].size()); ++z) {
            for (int y = max(py - 1, 0); y < min(py + 2, (int) space[0][0].size()); ++y) {
                for (int x = max(px - 1, 0); x < min(px + 2, (int) space[0][0][0].size()); ++x) {
                    if (x != px || y != py || z != pz || w != pw) {
                        nb += space[w][z][y][x];
                    }
                }
            }
        }
    }
    return nb;
}

void print(const deque<deque<deque<bool>>> &space) {
    for (int z = 0; z < space.size(); ++z) {
        for (int y = 0; y < space[0].size(); ++y) {
            for (int x = 0; x < space[0][0].size(); ++x) {
                cout << (space[z][y][x] ? '#' : '.');
            }
            cout << '\n';
        }
        cout << '\n';
    }
}

int main() {
    // temps courant, avant l'execution
    auto time1 = chrono::high_resolution_clock::now();

    const int N = 6;

    vector<string> lines = readFileLines("../input17.txt");

    deque<deque<deque<deque<bool>>>> spaceA{1 + 2 * N,
                                            {1 + 2 * N, {lines.size() + 2 * N, deque<bool>(lines[0].size() + 2 * N, false)}}};
    deque<deque<deque<deque<bool>>>> spaceB{1 + 2 * N,
                                            {1 + 2 * N, {lines.size() + 2 * N, deque<bool>(lines[0].size() + 2 * N, false)}}};

    for (int y = 0; y < lines.size(); ++y) {
        for (int x = 0; x < lines[0].size(); ++x) {
            spaceA[N][N][y + N][x + N] = lines[y][x] == '#';
        }
    }

    spaceB = spaceA;

//    print(spaceA);
//    cout << "-----\n";

    for (int i = 0; i < N; ++i) {
        for (int w = 0; w < spaceA.size(); ++w) {
            for (int z = 0; z < spaceA[0].size(); ++z) {
                for (int y = 0; y < spaceA[0][0].size(); ++y) {
                    for (int x = 0; x < spaceA[0][0][0].size(); ++x) {
                        int neighbours = countNeighbours(spaceA, x, y, z, w);
                        if (spaceA[w][z][y][x]) {
                            spaceB[w][z][y][x] = neighbours == 2 || neighbours == 3;
                        } else {
                            spaceB[w][z][y][x] = neighbours == 3;
                        }
                    }
                }
            }
        }
        spaceA = spaceB;
//        print(spaceA);
//        cout << "-----\n";
    }

    int total = 0;
    for (int w = 0; w < spaceA.size(); ++w) {
        for (int z = 0; z < spaceA[0].size(); ++z) {
            for (int y = 0; y < spaceA[0][0].size(); ++y) {
                for (int x = 0; x < spaceA[0][0][0].size(); ++x) {
                    total += spaceA[w][z][y][x];
                }
            }
        }
    }

    cout << total << endl;

    // temps courant, apres l'execution
    auto time2 = chrono::high_resolution_clock::now();

    // mesurer la difference, et l'exprimer en microsecondes
    unsigned int time = chrono::duration_cast<chrono::microseconds>(time2 - time1).count();

    cout << "\nTemps d'exécution = " << float(time) / 1000.0f << "ms" << endl;

}
