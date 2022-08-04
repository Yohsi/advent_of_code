#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <numeric>
#include <set>
#include <vector>

const int MAX_LINE_SIZE = 200;
using namespace std;

vector<string> splitLine(const string &input, const string &delimiter) {
  size_t begin = 0;
  size_t end;
  vector<string> ret{};

  while ((end = input.find(delimiter, begin)) != string::npos) {
    ret.push_back(input.substr(begin, end - begin));
    begin = end + delimiter.size();
  }
  ret.push_back(input.substr(begin));

  return ret;
}

template <typename K, typename V>
ostream &operator<<(ostream &os, const map<K, V> &m) {
  os << '{';
  for (auto &&[key, value] : m) {
    if (m.size() > 1)
      os << "\n  ";
    os << key << " : " << value;
  }
  if (m.size() > 1)
    os << '\n';
  os << '}';
  return os;
}

template <typename T> ostream &operator<<(ostream &os, const vector<T> &v) {
  bool first = true;
  os << "[";
  for (auto &&e : v) {
    if (!first)
      os << ", ";
    else
      first = false;

    os << e;
  }
  os << "]";
  return os;
}
template <typename T> ostream &operator<<(ostream &os, const set<T> &v) {
  bool first = true;
  os << "{";
  for (auto &&e : v) {
    if (!first)
      os << ", ";
    else
      first = false;
    os << e;
  }
  os << "}";
  return os;
}

int countNestedBags(const string &bag,
                    const map<string, map<string, int>> &bagMap) {
  int ret = 0;
  for (auto &&[innerBag, nb] : bagMap.at(bag)) {
    ret += nb + nb * countNestedBags(innerBag, bagMap);
  }
  return ret;
}

int main() {
  ifstream file;
  file.open("input7.txt");

  if (file.fail()) {
    cerr << "Impossible d'ouvrir le fichier" << endl;
    return 1;
  }

  vector<string> list;

  char fileLine[MAX_LINE_SIZE];
  while (file.getline(fileLine, MAX_LINE_SIZE)) {
    list.emplace_back(fileLine);
  }

  map<string, map<string, int>> bagMap{};
  set<string> validBagColors{"shiny gold"};

  for (auto &&line : list) {
    auto lineSplit = splitLine(line, " bags contain ");
    string containerBag = lineSplit[0];

    bagMap.insert({containerBag, {}});

    if (lineSplit[1] != "no other bags.") {
      vector<string> carriedBagsTemp = splitLine(lineSplit[1], ", ");
      transform(carriedBagsTemp.begin(), carriedBagsTemp.end(),
                inserter(bagMap[containerBag], bagMap[containerBag].begin()),
                [](const string &s) -> pair<string, int> {
                  return {s.substr(2, s.find(" bag") - 2),
                          stoi(s.substr(0, s.find(" ")))};
                });
    }
  }

  //   bool updated = true;
  //
  //   while (!updated) {
  //     for (auto &&[containerBag, carriedBags] : bagMap) {
  //       set<string> validBagsCarried{};
  //       set_intersection(validBagColors.begin(), validBagColors.end(),
  //                        carriedBags.begin(), carriedBags.end(),
  //                        inserter(validBagsCarried,
  //                        validBagsCarried.begin()));
  //
  //             cout << "valid bags carried = " << validBagsCarried << '\n';
  //
  //       if (!validBagsCarried.empty() &&
  //           validBagColors.find(containerBag) == validBagColors.end()) {
  //         validBagColors.insert(containerBag);
  //         updated = true;
  //       }
  //     }
  //         cout << "\nCurrent valid colors are :\n" << validBagColors <<
  //         "\n\n";
  //   }

  //   cout << "Part 1 :\n";
  //   cout << "nb = " << validBagColors.size() - 1 << endl;

  cout << countNestedBags("shiny gold", bagMap) << endl;

  return 0;
}
