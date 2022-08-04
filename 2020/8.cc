#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <numeric>
#include <set>
#include <vector>

const int MAX_LINE_SIZE = 20;
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

enum OpCode { NOP, ACC, JMP };

struct Operation {
  OpCode opCode;
  int arg;

  Operation(OpCode op, int a) : opCode{op}, arg{a} {}
};

OpCode strToOp(string str) {
  if (str == "nop")
    return OpCode::NOP;
  if (str == "acc")
    return OpCode::ACC;
  if (str == "jmp")
    return OpCode::JMP;
  else
    throw runtime_error("Unknown op code");
}

void swapOperation(Operation &op) {
  if (op.opCode == NOP) {
    op.opCode = JMP;
  } else if (op.opCode == JMP) {
    op.opCode = NOP;
  } else {
    throw runtime_error("Operation is not a NOP nor a JMP");
  }
}

int main() {
  ifstream file;
  file.open("input8.txt");

  if (file.fail()) {
    cerr << "Impossible d'ouvrir le fichier" << endl;
    return 1;
  }

  vector<Operation> operations{};

  char fileLine[MAX_LINE_SIZE];
  while (file.getline(fileLine, MAX_LINE_SIZE)) {
    if (string{fileLine} != "") {
      vector<string> lineSplit = splitLine(fileLine, " ");
      operations.emplace_back(strToOp(lineSplit[0]), stoi(lineSplit[1]));
    }
  }

  size_t pc = 0;
  long acc = 0;
  set<size_t> executedOperations{};

  bool terminated = false;
  long changedOp = -1;
  bool first = true;

  while (!terminated) {

    // Reset the simulator
    pc = 0;
    acc = 0;
    executedOperations.clear();

    while (executedOperations.count(pc) == 0 && !terminated) {
      executedOperations.insert(pc);

      auto currentOp = operations[pc];

      switch (currentOp.opCode) {
      case ACC:
        acc += currentOp.arg;
        pc++;
        break;

      case JMP:
        pc += currentOp.arg;
        break;

      case NOP:
        pc++;
        break;
      }
      terminated = (pc >= operations.size());
    }

    if (!first) {
      // reset current substitution
      swapOperation(operations[changedOp]);
    }
    first = false;

    // Find next JMP or NOP
    changedOp = find_if(operations.begin() + changedOp + 1, operations.end(),
                        [](const Operation &op) {
                          return op.opCode == JMP || op.opCode == NOP;
                        }) -
                operations.begin();

    // Substitute it
    swapOperation(operations[changedOp]);
  }

  cout << acc << endl;

  return 0;
}
