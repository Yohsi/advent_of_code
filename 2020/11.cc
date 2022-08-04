#include "lib.h"

void printGrid(const vector<string> &grid) {
  for (auto &&s : grid) {
    cout << s << '\n';
  }
}

bool isOccupied(const vector<string> &grid, int x, int y) {
  bool ret;
  if ((size_t)y >= grid.size() || (size_t)x >= grid[0].size() || x < 0 ||
      y < 0) {
    ret = false;
  } else {
    ret = grid[y][x] == '#';
  }
  //   cout << "(" << x << "," << y << ") is occupied = " << ret << '\n';
  return ret;
}

bool isVisibleSeatOccupied(const vector<string> &grid, int x, int y, int dirX,
                           int dirY) {
  int curX = x + dirX;
  int curY = y + dirY;

  while (curX >= 0 && curY >= 0 && (size_t)curY < grid.size() &&
         (size_t)curX < grid[0].size()) {
    if (grid[curY][curX] == '#') {
      return true;
    }
    if (grid[curY][curX] == 'L') {
      return false;
    }
    curX += dirX;
    curY += dirY;
  }
  return false;
}

int nbAdjSeatsOccupied(const vector<string> &grid, int x, int y) {
  return (isOccupied(grid, x - 1, y - 1) + isOccupied(grid, x, y - 1) +
          isOccupied(grid, x + 1, y - 1) + isOccupied(grid, x + 1, y) +
          isOccupied(grid, x + 1, y + 1) + isOccupied(grid, x, y + 1) +
          isOccupied(grid, x - 1, y + 1) + isOccupied(grid, x - 1, y));
}

int nbVisibleSeatsOccupied(const vector<string> &grid, int x, int y) {
  return (isVisibleSeatOccupied(grid, x, y, -1, -1) +
          isVisibleSeatOccupied(grid, x, y, 0, -1) +
          isVisibleSeatOccupied(grid, x, y, 1, -1) +
          isVisibleSeatOccupied(grid, x, y, 1, 0) +
          isVisibleSeatOccupied(grid, x, y, 1, 1) +
          isVisibleSeatOccupied(grid, x, y, 0, 1) +
          isVisibleSeatOccupied(grid, x, y, -1, 1) +
          isVisibleSeatOccupied(grid, x, y, -1, 0));
}

int main() {
  vector<string> gridA = readFileLines("input11.txt");

  // Part 1
  bool changed = true;
  vector<string> gridB{gridA.size(), string(gridA[0].size(), 'x')};

  vector<string> &grid = gridA;
  vector<string> &newGrid = gridB;

  //   printGrid(grid);
  //   cout << endl;

  while (changed) {
    changed = false;
    for (size_t y = 0; y < grid.size(); y++) {
      for (size_t x = 0; x < grid[0].size(); x++) {
        int nbAdjOcc = nbVisibleSeatsOccupied(grid, x, y);
        //         cout << "(" << x << "," << y << ") = " << grid[y][x]
        //              << " | adj = " << nbAdjOcc << '\n';
        if (grid[y][x] == 'L' && nbAdjOcc == 0) {
          newGrid[y][x] = '#';
          changed = true;
        } else if (grid[y][x] == '#' && nbAdjOcc >= 5) {
          newGrid[y][x] = 'L';
          changed = true;
        } else {
          newGrid[y][x] = grid[y][x];
        }
      }
    }
    if (grid == gridA) {
      grid = gridB;
      newGrid = gridA;
    } else {
      grid = gridA;
      newGrid = gridB;
    }
    //     cout << '\n';
    //     printGrid(grid);
    //     cout << endl;
  }

  int nbOccupied =
      accumulate(grid.begin(), grid.end(), 0, [](int c, const string &s) {
        return c + count(s.begin(), s.end(), '#');
      });

  cout << nbOccupied << endl;

  // Part 2

  return 0;
}
