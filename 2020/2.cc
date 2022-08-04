#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

int main() {
  ifstream file;
  file.open("../input2.txt");

  int validNb = 0;

  while (!file.eof()) {
    string pwd;
    int pos1;
    int pos2;
    char letter;

    file >> pos1;
    if (file.eof())
      break;

    file.ignore(1);
    file >> pos2;
    file >> letter;
    file.ignore(1);
    file >> pwd;

    pos1--;
    pos2--;

    int nb = ((pwd[pos1] == letter) + (pwd[pos2] == letter));

    if (nb == 1) {
      validNb++;
    }
    cout << pos1 << '-' << pos2 << ": " << letter << " | " << pwd << " -> "
         << nb << " (" << pwd[pos1] << " and " << pwd[pos2] << ")" << '\n';
  }
  cout << validNb << endl;
}
