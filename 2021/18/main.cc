#include "aoclib.hpp"

struct snailfish_pair;

struct snailfish_nb {
	static unique_ptr<snailfish_nb> parse(istream &f, snailfish_pair *parent);

	explicit snailfish_nb(snailfish_pair *parent) : parent(parent) {}

	snailfish_pair *parent;

	[[nodiscard]] virtual ulong magnitude() const = 0;
	virtual bool recurse_explode() = 0;
	virtual bool recurse_split() = 0;

	[[nodiscard]] uint depth() const;

	[[nodiscard]] snailfish_nb *root();

	friend unique_ptr<snailfish_nb> operator+(unique_ptr<snailfish_nb> a, unique_ptr<snailfish_nb> b);

	friend ostream &operator<<(ostream &os, const snailfish_nb &nb);
	virtual void print_snailfish(ostream &os, snailfish_nb *highlight) const = 0;

	virtual ~snailfish_nb() = default;
};


class snailfish_regular_nb;
struct snailfish_pair : public snailfish_nb {
	snailfish_pair(istream &f, snailfish_pair *parent) : snailfish_nb{parent} {
		left = snailfish_nb::parse(f, this);
		f >> ",";
		right = snailfish_nb::parse(f, this);
		f >> "]";
	}

	snailfish_pair(unique_ptr<snailfish_nb> left, unique_ptr<snailfish_nb> right, snailfish_pair *parent) :
			snailfish_nb{parent},
			left{move(left)},
			right{move(right)} {
		if (this->left)
			this->left->parent = this;
		if (this->right)
			this->right->parent = this;
	}

	snailfish_pair(const snailfish_pair &o);

	unique_ptr<snailfish_nb> left;
	unique_ptr<snailfish_nb> right;

	[[nodiscard]] ulong magnitude() const override {
		return 3 * left->magnitude() + 2 * right->magnitude();
	}

	bool recurse_explode() override {
		if (depth() >= 4) {
			explode();
			return false;
		}
		return left->recurse_explode() && right->recurse_explode();
	}

	bool recurse_split() override {
		return left->recurse_split() && right->recurse_split();
	}

	void reduce() {
		bool reduced = false;
		while (!reduced) {
			reduced = recurse_explode() && recurse_split();
		}
	}

	void print_snailfish(ostream &os, snailfish_nb *highlight) const override {
		if (highlight == this)
			os << light_green;
		os << "[";
		left->print_snailfish(os, highlight);
		os << ",";
		right->print_snailfish(os, highlight);
		os << "]";
		if (highlight == this)
			os << light_gray;
	}

	snailfish_regular_nb *get_first_left();
	snailfish_regular_nb *get_first_right();

	void explode();

	~snailfish_pair() override = default;
};

struct snailfish_regular_nb : public snailfish_nb {
	snailfish_regular_nb(ulong nb, snailfish_pair *parent) : snailfish_nb{parent}, nb{nb} {}

	snailfish_regular_nb(const snailfish_regular_nb &o) : snailfish_nb(nullptr), nb{o.nb} {}

	ulong nb;

	[[nodiscard]] ulong magnitude() const override {
		return nb;
	}

	bool recurse_split() override {
		if (nb >= 10) {
			split();
			return false;
		}
		return true;
	}

	bool recurse_explode() override;

	void split();

	void print_snailfish(ostream &os, snailfish_nb *highlight) const override {
		if (highlight == this)
			os << light_green;
		os << nb;
		if (highlight == this)
			os << light_gray;
	}

	~snailfish_regular_nb() override = default;
};

void snailfish_regular_nb::split() {
//	cout << light_gray;
//	root()->print_snailfish(cout, this);
//	cout << light_magenta << "\nsplitting " << nb << "\n";
	auto new_pair = make_unique<snailfish_pair>(nullptr, nullptr, parent);
	new_pair->left = make_unique<snailfish_regular_nb>(nb / 2, new_pair.get());
	new_pair->right = make_unique<snailfish_regular_nb>((nb + 1) / 2, new_pair.get());
	if (parent->right.get() == this) {
		parent->right = move(new_pair);
	} else if (parent->left.get() == this) {
		parent->left = move(new_pair);
	} else {
		assert(false);
	}
}

bool snailfish_regular_nb::recurse_explode() {
	return true;
}

snailfish_regular_nb *snailfish_pair::get_first_left() {
	snailfish_nb *n = this;
	while (n->parent && n->parent->left.get() == n) {
		n = n->parent;
	}
	if (!n->parent) return nullptr;
	n = n->parent->left.get();
	while (auto p = dynamic_cast<snailfish_pair *>(n)) {
		n = p->right.get();
	}
	assert(dynamic_cast<snailfish_regular_nb *>(n) != nullptr);
	return dynamic_cast<snailfish_regular_nb *>(n);
}

snailfish_regular_nb *snailfish_pair::get_first_right() {
	snailfish_nb *n = this;
	while (n->parent && n->parent->right.get() == n) {
		n = n->parent;
	}
	if (!n->parent) return nullptr;
	n = n->parent->right.get();
	while (auto p = dynamic_cast<snailfish_pair *>(n)) {
		n = p->left.get();
	}
	return dynamic_cast<snailfish_regular_nb *>(n);
}

void snailfish_pair::explode() {
//	cout << light_gray;
//	root()->print_snailfish(cout, this);
//	cout << light_magenta << "\nExploding ";
//	print_snailfish(cout, nullptr);
//	cout << "\n";
	assert(dynamic_cast<snailfish_regular_nb *>(left.get()) != nullptr);
	assert(dynamic_cast<snailfish_regular_nb *>(right.get()) != nullptr);
	ulong left_nb = dynamic_cast<snailfish_regular_nb *>(left.get())->nb;
	ulong right_nb = dynamic_cast<snailfish_regular_nb *>(right.get())->nb;
	auto first_left = get_first_left();
	if (first_left) {
		first_left->nb += left_nb;
	}
	auto first_right = get_first_right();
	if (first_right) {
		first_right->nb += right_nb;
	}
	unique_ptr<snailfish_nb> new_nb = make_unique<snailfish_regular_nb>(0, parent);
	if (parent->right.get() == this) {
		parent->right = move(new_nb);
	} else if (parent->left.get() == this) {
		parent->left = move(new_nb);
	} else {
		assert(false);
	}
}

snailfish_pair::snailfish_pair(const snailfish_pair &o) :
		snailfish_nb(nullptr) {
	assert(o.right != nullptr);
	assert(o.left != nullptr);
	if (dynamic_cast<snailfish_regular_nb *>(o.left.get()) != nullptr) {
		left = make_unique<snailfish_regular_nb>(dynamic_cast<snailfish_regular_nb &>(*o.left));
	} else {
		left = make_unique<snailfish_pair>(dynamic_cast<snailfish_pair &>(*o.left));
	}
	if (dynamic_cast<snailfish_regular_nb *>(o.right.get()) != nullptr) {
		right = make_unique<snailfish_regular_nb>(dynamic_cast<snailfish_regular_nb &>(*o.right));
	} else {
		right = make_unique<snailfish_pair>(dynamic_cast<snailfish_pair &>(*o.right));
	}
	left->parent = this;
	right->parent = this;
}

unique_ptr<snailfish_nb> snailfish_nb::parse(istream &f, snailfish_pair *parent) {
	int c = f.get();
	if (c == '[') {
		return make_unique<snailfish_pair>(f, parent);
	} else if (c >= '0' && c <= '9') {
		return make_unique<snailfish_regular_nb>(c - '0', parent);
	} else {
		return nullptr;
	}
}

ostream &operator<<(ostream &os, const snailfish_nb &nb) {
	nb.print_snailfish(os, nullptr);
	return os;
}

unique_ptr<snailfish_nb> operator+(unique_ptr<snailfish_nb> a, unique_ptr<snailfish_nb> b) {
	auto result = make_unique<snailfish_pair>(move(a), move(b), nullptr);
	result->reduce();
	return result;
}

uint snailfish_nb::depth() const {
	uint d = 0;
	const snailfish_pair *p = parent;
	while (p != nullptr) {
		d++;
		p = p->parent;
	}
	return d;
}

snailfish_nb *snailfish_nb::root() {
	if (parent == nullptr) return this;
	snailfish_nb *p = parent;
	while (p->parent != nullptr) {
		p = p->parent;
	}
	return p;
}

struct Day18 : public Aoc {
	vector<unique_ptr<snailfish_nb>> numbers;

	explicit Day18(ifstream &f) : Aoc{f} {
		unique_ptr<snailfish_nb> temp;
		while (true) {
			temp = snailfish_nb::parse(f, nullptr);
			if (temp == nullptr) break;
			numbers.push_back(move(temp));
			f >> "\n";
		}
//		for (auto &&nb: numbers) {
//			cout << *nb << '\n';
//		}
	}

	ulong part1() override {
		vector<unique_ptr<snailfish_nb>> numbers1{};
		numbers1.reserve(numbers.size());
		for (auto &&nb: numbers) {
			unique_ptr<snailfish_nb> copy = make_unique<snailfish_pair>(dynamic_cast<snailfish_pair &>(*nb));
			numbers1.push_back(move(copy));
		}
		unique_ptr<snailfish_nb> result{move(numbers1[0])};
		for (int i = 1; i < numbers1.size(); ++i) {
			result = move(result) + move(numbers1[i]);
		}
		return result->magnitude();
	}

	ulong part2() override {
		ulong max_magnitude = 0;
		for (auto &a: numbers) {
			for (auto &b: numbers) {
				auto a_cpy = make_unique<snailfish_pair>(dynamic_cast<snailfish_pair &>(*a));
				auto b_cpy = make_unique<snailfish_pair>(dynamic_cast<snailfish_pair &>(*b));
				auto c = move(a_cpy) + move(b_cpy);
				max_magnitude = max(max_magnitude, c->magnitude());
			}
		}
		return max_magnitude;
	}
};

int main() {
	Aoc::run<Day18>(4140, 3993);
}
