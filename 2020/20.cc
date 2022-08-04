#include "lib.h"
#include <bitset>
#include <sstream>
#include <chrono>

const int N = 10;
const int T = 12;

struct Side : public bitset<N> {
    Side reverse() const {
        Side rev{};
        for (int i = 0; i < N; i++) {
            rev[N - 1 - i] = (*this)[i];
        }
        return rev;
    }

    bool operator<(const Side &other) const {
        return to_ulong() < other.to_ulong();
    }

    bool operator>(const Side &other) const {
        return to_ulong() > other.to_ulong();
    }

    bool operator<=(const Side &other) const {
        return to_ulong() <= other.to_ulong();
    }

    bool operator>=(const Side &other) const {
        return to_ulong() >= other.to_ulong();
    }

    bool operator==(const Side &other) const {
        return to_ulong() == other.to_ulong();
    }
};

struct Tile {
    int id = 0;

    Side north{0};
    Side east{0};
    Side south{0};
    Side west{0};

    array<bitset<N - 2>, N - 2> grid{};

    friend istream &operator>>(istream &is, Tile &tile) {
        tile.id = 0;
        string ignored;
        is >> ignored;
        is >> tile.id;
        is.ignore(1);

        if (tile.id == 0) {
            return is;
        }

        char c;
        for (int l = 0; l < N; l++) {
            for (int i = 0; i < N; i++) {
                is >> c;
                if (l != 0 && l != N - 1 && i != 0 && i != N - 1) {
                    tile.grid[l - 1][i - 1] = (c == '#');
                }
                if (l == 0)
                    tile.north[i] = (c == '#');
                if (i == 0)
                    tile.west[l] = (c == '#');
                if (l == N - 1)
                    tile.south[i] = (c == '#');
                if (i == N - 1)
                    tile.east[l] = (c == '#');
            }
        }
        return is;
    }

    friend ostream &operator<<(ostream &os, const Tile &tile) {
        os << tile.id << '\n';
        for (int l = 0; l < N; l++) {
            for (int i = 0; i < N; i++) {
                if (l != 0 && l != N - 1 && i != 0 && i != N - 1) {
                    os << tile.grid[l - 1][i - 1];
                }
//                if (l == 0)
//                    os << tile.north[i];
//                else if (i == 0)
//                    os << tile.west[l];
//                else if (l == N - 1)
//                    os << tile.south[i];
//                else if (i == N - 1)
//                    os << tile.east[l];
//                else
//                    os << ' ';
            }
            os << '\n';
        }
        return os;
    }

    bool operator<(const Tile &other) const {return id < other.id;}

    Tile *rotate() const {
        Tile *rot = new Tile();
        rot->id = id;
        rot->north = west.reverse();
        rot->west = south;
        rot->south = east.reverse();
        rot->east = north;
        for (int l = 0; l < N - 2; l++) {
            for (int i = 0; i < N - 2; i++) {
                rot->grid[i][N - 3 - l] = grid[l][i];
            }
        }

        return rot;
    }

    Tile *flipV() const {
        Tile *flip = new Tile{};
        flip->id = id;
        flip->north = south;
        flip->south = north;
        flip->west = west.reverse();
        flip->east = east.reverse();
        for (int l = 0; l < N - 2; l++) {
            for (int i = 0; i < N - 2; i++) {
                flip->grid[N - 3 - l][i] = grid[l][i];
            }
        }
        return flip;
    }

    void addSides(array<multimap<Side, Tile *>, 2> &m) {
        m[0].insert({north, this});
        m[1].insert({west, this});
    }
};

struct Grid {
    array<array<Tile *, T>, T> grid{};

    Grid() {
        for (int y = 0; y < T; y++) {
            for (int x = 0; y < T; y++) {
                grid[y][x] = nullptr;
            }
        }
    }

    friend ostream &operator<<(ostream &os, const Grid &g) {
        for (int y = 0; y < N * T; y++) {
            for (int x = 0; x < N * T; x++) {
                if (g.grid[y / N][x / N] == nullptr) {
                    os << ' ';
                } else if (x % N == 0) {
                    os << (g.grid[y / N][x / N]->west[y % N] ? '#' : '.');
                } else if (x % N == N - 1) {
                    os << (g.grid[y / N][x / N]->east[y % N] ? '#' : '.');
                } else if (y % N == 0) {
                    os << (g.grid[y / N][x / N]->north[x % N] ? '#' : '.');
                } else if (y % N == N - 1) {
                    os << (g.grid[y / N][x / N]->south[x % N] ? '#' : '.');
                } else if (x % N >= N / 2 - 2 && x % N < N / 2 + 2 && y % N == N / 2) {
                    os << to_string(g.grid[y / N][x / N]->id).substr(x % N - N / 2 + 2, 1);
                } else {
                    os << (g.grid[y / N][x / N]->grid[y % N - 1][x % N - 1] ? '@' : ' ');
                }
            }
            os << '\n';
        }
        return os;
    }

    bool get(u_int x, u_int y) const {
        return grid[y / (N - 2)][x / (N - 2)]->grid[y % (N - 2)][x % (N - 2)];
    }

};

typedef multimap<Side, Tile *>::iterator multimap_iter;


vector<Grid> solve(Grid &current, int currentIndex, set<int> &availableTiles, array<multimap<Side, Tile *>, 2> &sides) {
    if (currentIndex == T * T) {
//        cout << "FOUND!" << '\n';
        return {current};
    }
    int x = currentIndex % T;
    int y = currentIndex / T;

    multimap_iter it;
    multimap_iter end;

    if (x > 0) {
        tie(it, end) = sides[1].equal_range(current.grid[y][x - 1]->east);
    } else if (y > 0) {
        tie(it, end) = sides[0].equal_range(current.grid[y - 1][x]->south);
    } else {
        it = sides[0].begin();
        end = sides[0].end();
    }

    vector<Grid> ret;

    for (; it != end; it++) {
        Tile *tile = it->second;

        if (availableTiles.find(tile->id) == availableTiles.end()) {
            continue;
        }
        if (x > 0 && y > 0 && current.grid[y - 1][x]->south != tile->north) {
            continue;
        }
//        if (x == 0 && y == 0 && tile->id != 1951) {
//            continue;
//        }

        availableTiles.erase(tile->id);
//        cout << genSpaces(currentIndex) << "testing " << currentIndex << " -> " << tile->id << ", remaining ids = "
//             << availableTiles << '\n';
        current.grid[y][x] = tile;
        auto toAppend = solve(current, currentIndex + 1, availableTiles, sides);
        move(toAppend.begin(), toAppend.end(), back_inserter(ret));
        availableTiles.insert(tile->id);
        current.grid[y][x] = nullptr;
    }

//    cout << genSpaces(currentIndex) << "failed at index " << currentIndex << ", backtracking\n";
    return ret;
}

int countMonsters(const Grid &grid) {
    const array<bitset<20>, 3> monster = {
            bitset<20>{"                  # ", 20, ' ', '#'},
            bitset<20>{"#    ##    ##    ###", 20, ' ', '#'},
            bitset<20>{" #  #  #  #  #  #   ", 20, ' ', '#'},
    };

    int ret = 0;
    for (int y = 0; y < T * (N - 2) - 2; ++y) {
        for (int x = 0; x < T * (N - 2) - 19; ++x) {
            bool isMonster = true;
            for (int l = 0; l < 3; ++l) {
                for (int c = 0; c < 20; ++c) {
                    if (monster[l][c] && !grid.get(x + c, y + l))
                        isMonster = false;
                }
            }
            ret += isMonster;
        }
    }
    return ret;
}

int main() {
    // temps courant, avant l'execution
    auto a = chrono::high_resolution_clock::now();

    ifstream file;
    file.open("../input20.txt");

    if (file.fail()) {
        throw runtime_error("Impossible d'ouvrir le fichier");
    }

    set<int> tiles{};

    array<multimap<Side, Tile *>, 2> sides = {};

    while (file.good()) {
        Tile *tile = new Tile();
        file >> *tile;
        if (tile->id != 0) {
            tiles.insert(tile->id);

            Tile *rot0 = tile;
//            cout << *rot0 << "\n\n";
            Tile *rot1 = rot0->rotate();
//            cout << *rot1 << "\n\n";
            Tile *rot2 = rot1->rotate();
//            cout << *rot2 << "\n\n";
            Tile *rot3 = rot2->rotate();
//            cout << *rot3 << "\n\n";
            Tile *flippedRot0 = rot0->flipV();
//            cout << *flippedRot0 << "\n\n";
            Tile *flippedRot1 = flippedRot0->rotate();
//            cout << *flippedRot1 << "\n\n";
            Tile *flippedRot2 = flippedRot1->rotate();
//            cout << *flippedRot2 << "\n\n";
            Tile *flippedRot3 = flippedRot2->rotate();
//            cout << *flippedRot3 << "\n\n";


            rot0->addSides(sides);
            rot1->addSides(sides);
            rot2->addSides(sides);
            rot3->addSides(sides);

            flippedRot0->addSides(sides);
            flippedRot1->addSides(sides);
            flippedRot2->addSides(sides);
            flippedRot3->addSides(sides);
        }
    }

    Grid grid{};
    auto solutions = solve(grid, 0, tiles, sides);


    int nbMonsters = 0;
    for (auto &&sol : solutions) {
        int m = countMonsters(sol);
        if (m != 0) {
            nbMonsters = m;
            break;
        }
    }

    // count the total number of #
    long totalHashNb = 0;
    for (int y = 0; y < T * (N - 2); ++y) {
        for (int x = 0; x < T * (N - 2); ++x) {
            totalHashNb += solutions[0].get(x, y);
        }
    }

    int monsterHashNb = 15 * nbMonsters;

    cout << "nb of # that are not part of sea monster : " << totalHashNb - monsterHashNb << '\n';


    // temps courant, apres l'execution
    auto b = chrono::high_resolution_clock::now();

    // mesurer la difference, et l'exprimer en microsecondes
    unsigned int time = chrono::duration_cast<chrono::microseconds>(b - a).count();

    cout << "temps d'exécution = " << float(time) / 1000.0f << "ms" << endl;

}
