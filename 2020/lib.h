#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <numeric>
#include <set>
#include <vector>
#include <deque>
#include <list>
#include <chrono>

const int MAX_LINE_SIZE = 1000;
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

template <typename T> ostream &operator<<(ostream &os, const deque<T> &d) {
  bool first = true;
  os << "[";
  for (auto &&e : d) {
    if (!first)
      os << ", ";
    else
      first = false;

    os << e;
  }
  os << "]";
  return os;
}

template <typename T> ostream &operator<<(ostream &os, const list<T> &l) {
  bool first = true;
  os << "[";
  for (auto &&e : l) {
    if (!first)
      os << ", ";
    else
      first = false;

    os << e;
  }
  os << "]";
  return os;
}

template <typename T, size_t N> ostream &operator<<(ostream &os, const array<T, N> &a) {
  bool first = true;
  os << "[";
  for (auto &&e : a) {
    if (!first)
      os << ", ";
    else
      first = false;

    os << int(e);
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

template <typename T, typename U>
ostream &operator<<(ostream &os, const pair<T,U> &p) {
	os << "(" << p.first << "," << p.second << ")";
	return os;
}

string genSpaces(int nb) {
    string ret;
    for (int i = 0; i < nb; ++i) {
        ret += " ";
    }
    return ret;
}

vector<string> readFileLines(const string &path) {
  ifstream file;
  file.open(path);

  if (file.fail()) {
    throw runtime_error("Impossible d'ouvrir le fichier");
  }

  vector<string> lines{};

  char line[MAX_LINE_SIZE];

  while (file.getline(line, MAX_LINE_SIZE)) {
    lines.emplace_back(line);
  }
  return lines;
}
