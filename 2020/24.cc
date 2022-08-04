#include <ostream>
#include "lib.h"

enum Direction {
    NW, NE, E, SE, SW, W
};

struct HexTile {
    long x;
    long y;

    HexTile() : x{0}, y{0} {}

    HexTile(long x, long y) : x{x}, y{y} {}

    bool operator==(const HexTile &rhs) const {
        return x == rhs.x &&
               y == rhs.y;
    }

    bool operator!=(const HexTile &rhs) const {
        return !(rhs == *this);
    }

    bool operator<(const HexTile &rhs) const {
        if (x < rhs.x)
            return true;
        if (rhs.x < x)
            return false;
        return y < rhs.y;
    }

    bool operator>(const HexTile &rhs) const {
        return rhs < *this;
    }

    bool operator<=(const HexTile &rhs) const {
        return !(rhs < *this);
    }

    bool operator>=(const HexTile &rhs) const {
        return !(*this < rhs);
    }

    friend ostream &operator<<(ostream &os, const HexTile &tile) {
        os << "(" << tile.x << ", " << tile.y << ")";
        return os;
    }

    void move(Direction dir) {
        switch (dir) {
            case NW:
                x--;
                y++;
                break;
            case NE:
                y++;
                break;
            case E:
                x++;
                break;
            case SE:
                x++;
                y--;
                break;
            case SW:
                y--;
                break;
            case W:
                x--;
                break;
        }
    }

    [[nodiscard]] array<HexTile, 6> getAdjacent() const {
        return {
                HexTile{x - 1, y + 1},  // NW
                HexTile{x, y + 1},         // NE
                HexTile{x + 1, y},         // E
                HexTile{x + 1, y - 1},  // SE
                HexTile{x, y - 1},         // SW
                HexTile{x - 1, y},          // W
        };
    }
};


class Floor {
public:
    void flipTile(const HexTile &tile) {
        if (tile.x + 1 > maxX)
            setMaxX(tile.x + 1);
        if (tile.x - 1 < minX)
            setMinX(tile.x - 1);
        if (tile.y + 1 > maxY)
            setMaxY(tile.y + 1);
        if (tile.y - 1 < minY)
            setMinY(tile.y - 1);

        floor[tile] = !floor[tile];
    }

    u_int getNbBlackAdjacent(const HexTile &tile) {
        u_int nb = 0;
        for (auto &&adj: tile.getAdjacent()) {
            if (floor.find(adj) != floor.end())
                nb += floor[adj];
        }
        return nb;
    }

    [[nodiscard]] map<HexTile, bool>::iterator begin() {
        return floor.begin();
    }

    [[nodiscard]] map<HexTile, bool>::iterator end() {
        return floor.end();
    }

    [[nodiscard]] size_t countBlack() const {
        return count_if(floor.begin(), floor.end(), [](const pair<HexTile, bool> p) {return p.second;});
    }

    friend ostream &operator<<(ostream &os, const Floor &f) {
        for (long y = f.maxY; y >= f.minY; --y) {
            os << genSpaces(y - f.minY);
            for (long x = f.minX; x <= f.maxX; ++x) {
                if (f.floor.at(HexTile{x, y}))
                    cout << "# ";
                else if (x == 0)
                    cout << "/ ";
                else if (y == 0)
                    cout << "- ";
                else
                    cout << ". ";

            }
            os << '\n';
        }
        return os;
    }

private:
    map<HexTile, bool> floor{{{0,0}, false}};
    long maxX = 0;
    long minX = 0;
    long maxY = 0;
    long minY = 0;

    void setMaxX(long newX) {
        for (long x = maxX + 1; x <= newX; ++x) {
            for (long y = minY; y <= maxY; ++y) {
                floor.insert({HexTile{x, y}, false});
            }
        }
        maxX = newX;
    }

    void setMinX(long newX) {
        for (long x = newX; x < minX; ++x) {
            for (long y = minY; y <= maxY; ++y) {
                floor.insert({HexTile{x, y}, false});
            }
        }
        minX = newX;
    }

    void setMaxY(long newY) {
        for (long x = minX; x <= maxX; ++x) {
            for (long y = minY + 1; y <= newY; ++y) {
                floor.insert({HexTile{x, y}, false});
            }
        }
        maxY = newY;
    }

    void setMinY(long newY) {
        for (long x = minX; x <= maxX; ++x) {
            for (long y = newY; y <= minY; ++y) {
                floor.insert({HexTile{x, y}, false});
            }
        }
        minY = newY;
    }
};

int main() {
    // temps courant, avant l'execution
    auto time1 = chrono::high_resolution_clock::now();

    auto lines = readFileLines("../input24.txt");

    Floor floor{};

    for (auto &&line:lines) {
        HexTile tile{};

        Direction dir;
        char prevChar = '0';
        for (char c:line) {
            if (c == 's' || c == 'n') {
                prevChar = c;
            } else {
                if (prevChar == 's') {
                    dir = c == 'e' ? SE : SW;
                    prevChar = '0';
                } else if (prevChar == 'n') {
                    dir = c == 'e' ? NE : NW;
                    prevChar = '0';
                } else {
                    dir = c == 'e' ? E : W;
                }
                tile.move(dir);
            }
        }
        cout << "Flipping tile " << tile << ":\n";
        floor.flipTile(tile);
        cout << floor << '\n';
    }

    for (int i = 0; i < 100; ++i) {
        cout << "\n--- Day " << i << " ---\n";
        cout << floor << '\n';

        Floor tempFloor{floor};
        for (auto &&[tile, black] : floor) {
            u_int nbBlackAdj = floor.getNbBlackAdjacent(tile);
            if (black) {
                if (nbBlackAdj == 0 || nbBlackAdj > 2) {
                    tempFloor.flipTile(tile);
                }
            } else {
                if (nbBlackAdj == 2) {
                    tempFloor.flipTile(tile);
                }
            }
        }
        floor = tempFloor;
        cout << floor.countBlack() << '\n';
    }


    // temps courant, apres l'execution
    auto time2 = chrono::high_resolution_clock::now();

    // mesurer la difference, et l'exprimer en microsecondes
    unsigned int time = chrono::duration_cast<chrono::microseconds>(time2 - time1).count();

    cout << "\nTemps d'exécution = " << float(time) / 1000.0f << "ms" << endl;

}
