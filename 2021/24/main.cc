#include "aoclib.hpp"
#include <cmath>
#include <ranges>
#include <memory>
#include <utility>

enum struct Command {
	INP,
	ADD,
	MUL,
	DIV,
	MOD,
	EQL
};

ostream &operator<<(ostream &os, Command c) {
	switch (c) {
		case Command::ADD:
			return os << "ADD";
		case Command::INP:
			return os << "INP";
		case Command::MUL:
			return os << "MUL";
		case Command::DIV:
			return os << "DIV";
		case Command::MOD:
			return os << "MOD";
		case Command::EQL:
			return os << "EQL";
	}
	return os;
}

istream &operator>>(istream &is, Command &c) {
	string s;
	is >> s;
	if (s == "add")
		c = Command::ADD;
	else if (s == "inp")
		c = Command::INP;
	else if (s == "mul")
		c = Command::MUL;
	else if (s == "div")
		c = Command::DIV;
	else if (s == "mod")
		c = Command::MOD;
	else if (s == "eql")
		c = Command::EQL;
	else
		throw runtime_error("Unexpected operation: " + s);
	return is;
}

struct Instruction {
	Command cmd;
	char arg1;
	optional<variant<char, long>> arg2 = nullopt;

	[[nodiscard]] unsigned short reg1() const {
		return arg1 - 'w';
	}

	[[nodiscard]] unsigned short reg2() const {
		return get<char>(arg2.value()) - 'w';
	}

	friend ostream &operator<<(ostream &os, const Instruction &instruction) {
		os << instruction.cmd << " " << instruction.arg1 << " " << instruction.arg2;
		return os;
	}

	friend istream &operator>>(istream &is, Instruction &instruction) {
		is >> instruction.cmd >> instruction.arg1;
		long a;
		is >> a;
		if (!is.fail()) {
			instruction.arg2 = a;
			return is;
		}
		is.clear();
		char c;
		is >> c;
		if (!is.fail()) {
			instruction.arg2 = c;
		}
		return is;
	}
};

struct Node {
	friend ostream &operator<<(ostream &os, const Node &n) {
		n.print_rec(os, 0);
		return os;
	}

	virtual void optimize(shared_ptr<Node> &this_node, const umap<u_short, u_short> &partial_assignment) = 0;

	virtual void print_rec(ostream &os, int indent) const = 0;
	virtual ~Node() = default;
};

struct Number : public Node {
	explicit Number(long nb) : nb(nb) {}

	void print_rec(ostream &os, int indent) const override {
		os << genSpaces(indent) << light_blue << nb << '\n';
	}

	void optimize(shared_ptr<Node> &this_node, const umap<u_short, u_short> &partial_assignment) override {}

	long nb;
	~Number() override = default;
};

struct Input : public Node {
	explicit Input(u_short index) : index(index) {}

	void optimize(shared_ptr<Node> &this_node, const umap<u_short, u_short> &partial_assignment) override {
		if (partial_assignment.contains(index)) {
			this_node = make_shared<Number>(partial_assignment.at(index));
		}
	}

	void print_rec(ostream &os, int indent) const override {
		os << genSpaces(indent) << green << "input " << index << '\n';
	}

	u_short index;
	~Input() override = default;
};

struct Operation : public Node {
	Operation(Command op, shared_ptr<Node> arg1, shared_ptr<Node> arg2) :
			op(op), arg1(move(arg1)), arg2(move(arg2)) {}

	void print_rec(ostream &os, int indent) const override {
		os << genSpaces(indent) << magenta << op << '\n';
		arg1->print_rec(os, indent + 1);
		arg2->print_rec(os, indent + 1);
	}

	void optimize(shared_ptr<Node> &this_node, const umap<u_short, u_short> &partial_assignment) override {
		arg1->optimize(arg1, partial_assignment);
		arg2->optimize(arg2, partial_assignment);
		auto number1 = dynamic_pointer_cast<Number>(arg1);
		auto input1 = dynamic_pointer_cast<Input>(arg1);
		auto number2 = dynamic_pointer_cast<Number>(arg2);
		auto input2 = dynamic_pointer_cast<Input>(arg2);
		switch (op) {
			case Command::ADD:
				if (number1 && number2)
					this_node = make_shared<Number>(number1->nb + number2->nb);
				else if (number1 && number1->nb == 0)
					this_node = arg2;
				else if (number2 && number2->nb == 0)
					this_node = arg1;
				break;
			case Command::MUL:
				if (number1 && number1->nb == 0 || number2 && number2->nb == 0)
					this_node = make_shared<Number>(0);
				else if (number1 && number2)
					this_node = make_shared<Number>(number1->nb * number2->nb);
				else if (number1 && number1->nb == 1)
					this_node = arg2;
				else if (number2 && number2->nb == 1)
					this_node = arg1;
				break;
			case Command::DIV:
				if (number1 && number2)
					this_node = make_shared<Number>(number1->nb / number2->nb);
				else if (number1 && number1->nb == 0)
					this_node = make_shared<Number>(0);
				else if (number2 && number2->nb == 1)
					this_node = arg1;
				break;
			case Command::MOD:
				if (number1 && number2)
					this_node = make_shared<Number>(number1->nb % number2->nb);
				else if (number1 && number1->nb == 0 || number2 && number2->nb == 1)
					this_node = make_shared<Number>(0);
				break;
			case Command::EQL:
				if (number1 && number2)
					this_node = make_shared<Number>(number1->nb == number2->nb);
				else if ((number1 && (number1->nb < 1 || number1->nb > 9) && input2) ||
				         (number2 && (number2->nb < 1 || number2->nb > 9) && input1))
					this_node = make_shared<Number>(0);
				break;
			default:
				assert(false);
		}
	}

	Command op;
	shared_ptr<Node> arg1;
	shared_ptr<Node> arg2;
	~Operation() override = default;
};


struct Day24 : public Aoc {
	vector<Instruction> instructions;
	array<u_short, 14> input{9,9,7,9,9,2,1,2,9,4,9,9,6,7};
	int input_index = 0;
	array<long, 4> registers{0, 0, 0, 0};

	explicit Day24(ifstream &f) : Aoc{f} {
		parse2(readBuf(f), instructions, "\n");
	}

	void emulate(const Instruction &ins) {
		long arg2;
		if (ins.arg2.has_value()) {
			if (holds_alternative<char>(ins.arg2.value())) {
				arg2 = registers[ins.reg2()];
			} else {
				arg2 = get<long>(ins.arg2.value());
			}
		}
		switch (ins.cmd) {
			case Command::INP:
				registers[ins.reg1()] = input[input_index++];
				break;
			case Command::ADD:
				registers[ins.reg1()] = registers[ins.reg1()] + arg2;
				break;
			case Command::MUL:
				registers[ins.reg1()] = registers[ins.reg1()] * arg2;
				break;
			case Command::DIV:
				registers[ins.reg1()] = registers[ins.reg1()] / arg2;
				break;
			case Command::MOD:
				registers[ins.reg1()] = registers[ins.reg1()] % arg2;
				break;
			case Command::EQL:
				registers[ins.reg1()] = registers[ins.reg1()] == arg2;
				break;
		}
	}

	array<shared_ptr<Node>, 4> operation_tree() {
		int index = 0;
		array<shared_ptr<Node>, 4> reg;
		reg.fill(make_shared<Number>(0));

		for (auto &&ins: instructions) {
			if (ins.cmd == Command::INP) {
//				cout << green << ins << " (" << index << ")\n";
				reg[ins.reg1()] = make_shared<Input>(index);
				index++;
			} else {
				if (holds_alternative<char>(*ins.arg2)) {
//					cout << light_blue << ins << '\n';
					reg[ins.reg1()] = make_shared<Operation>(ins.cmd,
					                                         reg[ins.reg1()],
					                                         reg[ins.reg2()]);
				} else {
//					cout << light_magenta << ins << '\n';
					reg[ins.reg1()] = make_shared<Operation>(ins.cmd,
					                                         reg[ins.reg1()],
					                                         make_shared<Number>(get<long>(*ins.arg2)));
				}
			}
		}

		return reg;
	}

	int decrement_index = 13;

	bool decrement_input() {
//		for (int i = input.size() - 1; i >= 0; i--) {
//			if (--input[i] == 0) {
//				input[i] = 9;
//				break;
//			}
//		}
		if (--input[decrement_index] == 0) {
			input[decrement_index] = 9;
			decrement_index--;
			if (decrement_index < 0) return false;
			--input[decrement_index];
		}
		return true;

	}

	void emulate_all() {
		for (auto &&ins: instructions) {
			emulate(ins);
		}
	}

	long emulate_simplified() {
		vector<bool> A{};
		vector<long> B{};
		vector<long> C{};
		long z = 0;
		for (int i = 0; i < 14; ++i) {
			A.push_back(get<long>(*instructions[18 * i + 4].arg2) == 26);
			B.push_back(get<long>(*instructions[18 * i + 5].arg2));
			C.push_back(get<long>(*instructions[18 * i + 15].arg2));
		}
//		print(A);
//		print(B);
//		print(C);
		for (int i = 0; i < 14; i++) {
			long mod = z % 26;
			if (A[i])
				z /= 26;
			if (mod != input[i] - B[i])
				z = z * 26 + (input[i] + C[i]);
		}
		print(z);
		return z;
	}

	[[nodiscard]] ulong input_to_long() const {
		ulong ret = 0;
		for (int i = 0; i < input.size(); ++i) {
			ret += static_cast<ulong>(pow(10, input.size() - i - 1)) * input[i];
		}
		return ret;
	}

	ulong part1() override {
//		auto tree = operation_tree();
//		umap<u_short, u_short> partial_assignment{};
//		ifstream f{INPUT_DIR"/partial_assignment.txt"};
//		int r, a;
//		while (f >> r) {
//			f >> a;
//			partial_assignment[r] = a;
//		}
//		tree[0]->optimize(tree[0], partial_assignment);
//		tree[1]->optimize(tree[1], partial_assignment);
//		tree[2]->optimize(tree[2], partial_assignment);
//		tree[3]->optimize(tree[3], partial_assignment);
////		cout << *tree[0];
////		cout << *tree[1];
////		cout << *tree[2];
//		cout << *tree[3];
////		return 0;
//
//		long count = 0;
//		do {
//			if (count % 100000 == 0) {
		cout << "input = " << input << "\n";
//			}
//		count++;
		registers = {0, 0, 0, 0};
		input_index = 0;
		emulate_all();
		cout << registers[3] << '\n';
//		} while (registers[3] != 0 && decrement_input());


		emulate_simplified();

//		return input_to_long();
		return 0;
	}

	ulong part2()
	override {

		return 0;
	}
};

int main() {
	Aoc::run<Day24>(0, 0, true, false);
}
