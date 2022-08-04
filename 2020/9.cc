#include "lib.h"

const int N = 25;

bool isValid(long nb, deque<long> q) {
  for (long a : q) {
    for (long b : q) {
      if (a + b == nb) {
        return true;
      }
    }
  }
  return false;
}

int main() {
  ifstream file;
  file.open("input9.txt");

  if (file.fail()) {
    throw runtime_error("Impossible d'ouvrir le fichier");
  }

  vector<long> list{};
  long current;

  file >> current;
  while (!file.eof()) {
    list.push_back(current);
    file >> current;
  }

  deque<long> q{};

  for (int i = 0; i < N; i++) {
    file >> current;
    q.push_back(list[i]);
  }

  bool valid = true;
  size_t index = N - 1;

  while (valid) {
    index++;
    valid = isValid(list[index], q);
    q.push_back(list[index]);
    q.pop_front();
  }

  const long invalidNb = list[index];

  cout << invalidNb << endl;

  auto begin = list.begin();
  auto end = list.end();

  for (auto first = begin; first != end; first++) {
    for (auto last = first + 2; last != end; last++) {
      if (reduce(first, last) == invalidNb) {
        auto [min, max] = minmax_element(first, last);
        cout << "min = " << *min << "\nmax = " << *max
             << "\nsum = " << *min + *max << "\n";
        ;
        return 0;
      }
    }
  }

  return 0;
}
