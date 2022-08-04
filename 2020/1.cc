#include <fstream>
#include <iostream>
#include <vector>

int main() {
  std::ifstream file;
  file.open("../input1.txt");

  std::vector<int> list;

  while (!file.eof()) {
    int temp;
    file >> temp;
    list.push_back(temp);
  }

  for (auto &&i : list) {
    for (auto &&j : list) {
      for (auto &&k : list) {
        if (i + j + k == 2020)
          std::cout << i * j * k << std::endl;
      }
    }
  }
}
