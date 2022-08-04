#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

int main() {
  ifstream file;
  file.open("../input3.txt");

  if (file.fail()) {
    cerr << "Impossible d'ouvrir le fichier" << endl;
    return 1;
  }

  vector<string> list;

  string line;
  file >> line;
  while (file.good()) {
    list.push_back(line);
    file >> line;
  }

  pair<int, int> slopes[5] = {{1, 1}, {3, 1}, {5, 1}, {7, 1}, {1, 2}};

  long ret = 1;
  
  for (auto &slope : slopes) {

    int slopeX = slope.first;
    int slopeY = slope.second;

    int nb = 0;

    size_t x = 0;
    size_t y = 0;
    while (y < list.size()) {
      nb += (list[y][x] == '#');

      x = (x + slopeX) % list[y].size();
      y += slopeY;
    }
    
    cout << nb << endl;
    ret *= nb;
  }

  cout << ret << endl;
}
