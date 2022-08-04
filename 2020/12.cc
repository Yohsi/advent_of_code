#include "lib.h"

enum Direction { EAST = 0, SOUTH = 1, WEST = 2, NORTH = 3 };

struct Position {
  int x, y;
  Position(int x, int y) : x{x}, y{y} {}

  Position operator+=(const Position &o) {
    x += o.x;
    y += o.y;
    return *this;
  }

  friend Position operator+(Position lhs, const Position &rhs) {
    lhs += rhs;
    return lhs;
  }
  Position operator*=(int o) {
    x *= o;
    y *= o;
    return *this;
  }

  friend Position operator*(Position lhs, int rhs) {
    lhs *= rhs;
    return lhs;
  }
  friend Position operator*(int lhs, Position rhs) {
    rhs *= lhs;
    return rhs;
  }

  void rotate(int angle) {
    bool clockwise = angle < 0;
    angle = abs(angle);

    while (angle != 0) {
      int xOld = x;
      x = clockwise ? y : -y;
      y = clockwise ? -xOld : xOld;
      angle -= 90;
    }
  }

  void moveBy(Direction dir, int dist) {
    switch (dir) {
    case EAST:
      x += dist;
      break;
    case SOUTH:
      y += dist;
      break;
    case WEST:
      x -= dist;
      break;
    case NORTH:
      y -= dist;
      break;
    }
  }
};

ostream &operator<<(ostream &os, const Position &p) {
  os << "(" << p.x << "," << p.y << ")";
  return os;
}

int main() {
  ifstream file;
  file.open("input12.txt");

  if (file.fail()) {
    throw runtime_error("Impossible d'ouvrir le fichier");
  }

  vector<pair<char, int>> path{};

  char dir;
  int dist;
  while (!file.eof()) {
    file >> dir;
    file >> dist;
    if (!file.eof())
      path.emplace_back(dir, dist);
  }

  Position pos{0, 0};

  Position waypoint{10, -1};

  for (auto &&action : path) {
    switch (action.first) {
    case 'E':
      waypoint.moveBy(EAST, action.second);
      break;
    case 'S':
      waypoint.moveBy(SOUTH, action.second);
      break;
    case 'W':
      waypoint.moveBy(WEST, action.second);
      break;
    case 'N':
      waypoint.moveBy(NORTH, action.second);
      break;
    case 'L':
      waypoint.rotate(-action.second);
      //       facing = (Direction)(((int)facing + 4 - action.second / 90) % 4);
      break;
    case 'R':
      waypoint.rotate(action.second);
      //       facing = (Direction)(((int)facing + action.second / 90) % 4);
      break;
    case 'F':
      pos += action.second * waypoint;
      break;
    }
    cout << "action = " << action << ", pos = " << pos
         << ", waypoint = " << waypoint << endl;
  }

  cout << abs(pos.x) + abs(pos.y) << endl;
  return 0;
}
