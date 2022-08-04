#include "lib.h"
#include <sstream>

enum Operation { MUL, ADD };

struct Node {
  virtual long compute() = 0;
  virtual ~Node() = default;
};

struct OpNode : public Node {
  Node *lhs = nullptr;
  Node *rhs = nullptr;
  Operation op;

  OpNode(Operation op) : op{op} {}

  long compute() override {
    if (op == ADD)
      return lhs->compute() + rhs->compute();
    else
      return lhs->compute() * rhs->compute();
  }
  
  ~OpNode() override {delete lhs; delete rhs;}
};

struct NbNode : public Node {
  long nb;
  NbNode(long nb) : nb{nb} {}

  long compute() override { return nb; }
  ~NbNode() override = default;
};

Node *createNode(stringstream &expr) {
  Node *currentNode;
	bool doNotModifyCurrent = false;

  char c;
  expr >> c;

  if (c == '(') {
    currentNode = createNode(expr);
		doNotModifyCurrent = true;
  } else {
    currentNode = new NbNode{c - '0'};
		doNotModifyCurrent = false;
  }

  while (expr.good()) {
    OpNode *newNode;

    // Operation
    char c;
    expr >> c;

    if (expr.eof() || c == ')') {
      return currentNode;
    } else if (c == '+') {
      newNode = new OpNode(ADD);
    } else if (c == '*') {
      newNode = new OpNode(MUL);
    }

    // RHS
    expr >> c;

    if (c == '(') {
      newNode->rhs = createNode(expr);
    } else {
			newNode->rhs = new NbNode{c - '0'};
    }
    
    // Insertion
    if (newNode->op == MUL || dynamic_cast<NbNode *>(currentNode) != nullptr || dynamic_cast<OpNode *>(currentNode)->op == ADD || doNotModifyCurrent) {
			newNode->lhs = currentNode;
			currentNode = newNode;
			doNotModifyCurrent = false;
		} else {
			auto opCurrentNode = dynamic_cast<OpNode *>(currentNode);
			newNode->lhs = opCurrentNode->rhs;
			opCurrentNode->rhs = newNode;
		}
  }
  return currentNode;
}

long compute(stringstream &expr) {
  // lhs
  u_long lhs;
  expr >> lhs;
  if (expr.fail()) {
    char c;
    expr.clear();
    expr >> c;
    lhs = compute(expr);
  }
  cout << lhs << endl;
  u_long accumulator = lhs;

  while (expr.good()) {
    // op
    char op;
    expr >> op;
    if (op == ')' || expr.eof()) {
      break;
    }

    cout << op << endl;

    // rhs
    u_long rhs;
    expr >> rhs;
    if (expr.fail()) {
      char c;
      cout << "parenthese detected\n";
      expr.clear();
      expr >> c;
      rhs = compute(expr);
    }
    cout << rhs << endl;

    if (op == '+') {
      accumulator += rhs;
    } else if (op == '*') {
      accumulator *= rhs;
    } else {
      cerr << "unexpected operator: " << op << "\n";
    }
  }
  return accumulator;
}

int main() {

  vector<string> lines = readFileLines("input18.txt");

  u_long accumulator = 0ul;

  for (auto &&line : lines) {
    stringstream ss(line);
    accumulator += createNode(ss)->compute();
  }

  cout << accumulator << endl;

  return 0;
}
