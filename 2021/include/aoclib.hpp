#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <unordered_map>
#include <numeric>
#include <set>
#include <unordered_set>
#include <vector>
#include <deque>
#include <list>
#include <chrono>
#include <iomanip>
#include <queue>
#include <cassert>
#include <optional>
#include <variant>

const int MAX_LINE_SIZE = 1400;
using namespace std;

constexpr auto red = "\033[31m";
constexpr auto green = "\033[32m";
constexpr auto yellow = "\033[33m";
constexpr auto blue = "\033[34m";
constexpr auto magenta = "\033[35m";
constexpr auto cyan = "\033[36m";
constexpr auto light_gray = "\033[37m";
constexpr auto gray = "\033[90m";
constexpr auto light_red = "\033[91m";
constexpr auto light_green = "\033[92m";
constexpr auto light_yellow = "\033[93m";
constexpr auto light_blue = "\033[94m";
constexpr auto light_magenta = "\033[95m";
constexpr auto light_cyan = "\033[96m";
constexpr auto white = "\033[97m";
constexpr auto bold = "\033[1m";
constexpr auto no_bold = "\033[21m\033[24m";
constexpr auto underlined = "\033[4m";
constexpr auto reset = "\033[0m";

constexpr auto debug_color = cyan;
constexpr auto result_color = white;
constexpr auto time_color = light_gray;

#ifdef NDEBUG
#define COLORED_PRINT(x, color)
#else
#define COLORED_PRINT(x, color) cout << color << #x << " = " << light_##color << x << debug_color << endl
#endif

#define PRINT_DEFAULT(x) COLORED_PRINT(x, cyan)
#define GET_3RD_ARG(arg1, arg2, arg3, ...) arg3
#define PRINT_STRING_MACRO_CHOOSER(...) GET_3RD_ARG(__VA_ARGS__, COLORED_PRINT, PRINT_DEFAULT, )
#define print(...) PRINT_STRING_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)


#define stream_io(class_name, ...) \
    void debug_print(ostream &os) const { \
        string name_str = #__VA_ARGS__; \
        deque<string> names = splitLine<deque<string>>(name_str, ", "); \
        os << #class_name << "{"; \
        print_obj(os, names, __VA_ARGS__); \
        os << "}"; \
    } \
    friend ostream &operator<<(ostream &os, const class_name &t) { \
        t.debug_print(os); \
        return os; \
    } \
    void debug_parse(istream &is) { \
        parse_obj(is, __VA_ARGS__); \
    } \
    friend istream &operator>>(istream &is, class_name &t) { \
        t.debug_parse(is); \
        return is; \
    }

template<typename Value,
		typename Hash = hash<Value>,
		typename Pred = equal_to<Value>,
		typename Alloc = allocator<Value>>
using uset = unordered_set<Value, Hash, Pred, Alloc>;
template<typename Key,
		typename Tp,
		typename Hash = hash<Key>,
		typename Pred = equal_to<Key>,
		typename Alloc = allocator<std::pair<const Key, Tp>>>
using umap = unordered_map<Key, Tp, Hash, Pred, Alloc>;

template<typename K, typename V>
ostream &operator<<(ostream &os, const map<K, V> &m) {
	os << light_gray << '{' << light_cyan;
	for (auto &&[key, value]: m) {
		if (m.size() > 1)
			os << "\n  ";
		os << key << light_gray << " : " << light_cyan << value;
	}
	if (m.size() > 1)
		os << '\n';
	os << light_gray << '}' << light_cyan;
	return os;
}

template<typename K, typename V>
ostream &operator<<(ostream &os, const unordered_map<K, V> &m) {
	os << '{';
	for (auto &&[key, value]: m) {
		if (m.size() > 1)
			os << "\n  ";
		os << key << " : " << value;
	}
	if (m.size() > 1)
		os << '\n';
	os << '}';
	return os;
}

template<typename T>
ostream &operator<<(ostream &os, const vector<T> &v) {
	bool first = true;
	bool multilines = v.size() > 5;
	os << "[";
	if (multilines) os << "\n ";
	for (auto &&e: v) {
		if (!first) {
			os << ", ";
			if (multilines) os << "\n ";
		} else {
			first = false;
		}
		os << e;
	}
	if (multilines) os << "\n";
	os << "]";
	return os;
}

template<typename T>
ostream &operator<<(ostream &os, const deque<T> &d) {
	bool first = true;
	os << "[";
	for (auto &&e: d) {
		if (!first)
			os << ", ";
		else
			first = false;
		os << e;
	}
	os << "]";
	return os;
}

template<typename T>
ostream &operator<<(ostream &os, const list<T> &l) {
	bool first = true;
	os << "[";
	for (auto &&e: l) {
		if (!first)
			os << ", ";
		else
			first = false;
		os << e;
	}
	os << "]";
	return os;
}

template<typename T, size_t N>
ostream &operator<<(ostream &os, const array<T, N> &a) {
	bool first = true;
	os << "[";
	for (auto &&e: a) {
		if (!first)
			os << ", ";
		else
			first = false;
		os << e;
	}
	os << "]";
	return os;
}

template<typename T, size_t N>
istream &operator>>(istream &is, array<T, N> &a) {
	for (auto &&e: a) {
		is >> e;
	}
	return is;
}

template<typename T>
ostream &operator<<(ostream &os, const set<T> &v) {
	bool first = true;
	os << '{';
	for (auto &&e: v) {
		if (!first)
			os << ", ";
		else
			first = false;
		os << e;
	}
	os << '}';
	return os;
}

template<typename T>
ostream &operator<<(ostream &os, const unordered_set<T> &v) {
	bool first = true;
	os << '{';
	for (auto &&e: v) {
		if (!first)
			os << ", ";
		else
			first = false;
		os << e;
	}
	os << '}';
	return os;
}

template<typename T, typename U>
ostream &operator<<(ostream &os, const pair<T, U> &p) {
	os << "(" << p.first << "," << p.second << ")";
	return os;
}

template<typename T1, typename T2>
istream &operator>>(istream &is, pair<T1, T2> &p) {
	is >> p.first >> p.second;
	return is;
}

void print_obj(ostream &os, deque<string> &names) {}

template<typename T>
void print_obj(ostream &os, deque<string> &names, T &&val) {
	os << names.front() << "=" << val;
	names.pop_front();
	print_obj(os, names);
}

template<typename T, typename... Args>
void print_obj(ostream &os, deque<string> &names, T &&val, Args &&... args) {
	os << names.front() << "=" << val << " ";
	names.pop_front();
	print_obj(os, names, args...);
}

void parse_obj(istream &is) {}

template<typename T, typename... Args>
void parse_obj(istream &is, T &val, Args &&... args) {
	is >> val;
	parse_obj(is, args...);
}

string genSpaces(int nb) {
	string ret;
	for (int i = 0; i < nb; ++i) {
		ret += "  ";
	}
	return ret;
}

string repeat_str(int nb, string str) {
	string ret;
	for (int i = 0; i < nb; ++i) {
		ret += str;
	}
	return ret;
}


vector<string> readLines(istream &file) {
	vector<string> lines{};
	char line[MAX_LINE_SIZE];
	while (file.getline(line, MAX_LINE_SIZE)) {
		lines.emplace_back(line);
	}
	return lines;
}

string readBuf(istream &file) {
	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}

string readLine(istream &file) {
	char line[MAX_LINE_SIZE];
	file.getline(line, MAX_LINE_SIZE);
	return line;
}

template<typename T>
T parse_full(const string &s) {
	if constexpr(std::is_same_v<T, string>) {
		return s;
	} else {
		istringstream ss{s};
		T t;
		if (ss >> t)
			return t;
		throw runtime_error{"Parsing failed"};
	}
}

template<typename Container = vector<string>>
Container splitLine(const string &input, const string &delimiter) {
	size_t begin = 0;
	size_t end;
	Container ret{};
	typename Container::value_type val;

	if (delimiter == "") {
		ret.emplace_back(input);
		return ret;
	}

	while ((end = input.find(delimiter, begin)) != string::npos) {
		auto substr = input.substr(begin, end - begin);
		ret.emplace_back(parse_full<typename Container::value_type>(substr));
		begin = end + delimiter.size();
	}
	auto substr = input.substr(begin);
	ret.emplace_back(parse_full<typename Container::value_type>(substr));
	return ret;
}

template<typename Container>
void parse(istream &f, Container &cont) {
	typename Container::value_type val;
	while (f >> val) {
		cont.emplace_back(std::move(val));
	}
}


template<typename Container>
void parse(istream &f, Container &cont, const vector<string> &delimiters) {
	std::stringstream buffer;
	buffer << f.rdbuf();
	vector<string> split{1, buffer.str()};
	vector<string> new_split{};
	for (auto &&d: delimiters) {
		for (auto &&s: split) {
			auto tmp = splitLine(s, d);
			move(tmp.begin(), tmp.end(), back_inserter(new_split));
		}
		swap(split, new_split);
		new_split.clear();
	}
	stringstream ss;
	copy(split.begin(), split.end(), ostream_iterator<string>(ss, " "));
	parse(ss, cont);
}

template<typename T>
void parse2(const string &str, T &val) {
	stringstream ss{str};
	ss >> val;
}

template<typename Container, typename... Delimiters, typename = typename Container::value_type>
void parse2(const string &str, Container &cont, const string &delim, Delimiters &&...delimiters) {
	auto split = splitLine(str, delim);
	for (auto &&s: split) {
		if (s != "") {
			typename Container::value_type value{};
			parse2(s, value, delimiters...);
			cont.push_back(move(value));
		}
	}
}

template<typename Container>
Container parse(ifstream &f) {
	Container cont;
	parse(f, cont);
	return cont;
}


template<class T>
static auto set_intersection(const set<T> &a, const set<T> &b) {
	set<T> new_set;
	std::set_intersection(a.begin(), a.end(),
	                      b.begin(), b.end(),
	                      inserter(new_set, new_set.end()));
	return new_set;
}

template<class T, class... Ts>
static auto set_intersection(const set<T> &a, const set<T> &b, Ts &&...cs) {
	set<T> new_set;
	std::set_intersection(a.begin(), a.end(),
	                      b.begin(), b.end(),
	                      inserter(new_set, new_set.end()));
	return set_intersection(new_set, cs...);
}


template<class T>
static auto set_union(const set<T> &a, const set<T> &b) {
	set<T> new_set;
	std::set_union(a.begin(), a.end(),
	               b.begin(), b.end(),
	               inserter(new_set, new_set.end()));
	return new_set;
}

template<class T, class... Ts>
static auto set_union(const set<T> &a, const set<T> &b, Ts &&...cs) {
	set<T> new_set;
	std::set_union(a.begin(), a.end(),
	               b.begin(), b.end(),
	               inserter(new_set, new_set.end()));
	return set_union(new_set, cs...);
}

template<class T>
static auto set_difference(const set<T> &a, const set<T> &b) {
	set<T> new_set;
	std::set_difference(a.begin(), a.end(),
	                    b.begin(), b.end(),
	                    inserter(new_set, new_set.end()));
	return new_set;
}

struct Vec3 {
	Vec3() = default;

	Vec3(long x, long y, long z) : x{x}, y{y}, z{z} {}

	long x, y, z;

	friend auto operator<=>(const Vec3 &, const Vec3 &) = default;

	friend ostream &operator<<(ostream &os, const Vec3 &v) {
		return os << "(" << v.x << "," << v.y << "," << v.z << ")";
	}

	friend istream &operator>>(istream &is, Vec3 &v) {
		return is >> v.x >> v.y >> v.z;
	}
};

Vec3 operator-(const Vec3 &l, const Vec3 &r) {
	return Vec3{l.x - r.x, l.y - r.y, l.z - r.z};
}

Vec3 operator+(const Vec3 &l, const Vec3 &r) {
	return Vec3{l.x + r.x, l.y + r.y, l.z + r.z};
}


namespace std {
	template<>
	struct hash<Vec3> {
		size_t operator()(const Vec3 &c) const {
			// http://stackoverflow.com/a/1646913/126995
			size_t res = 17;
			res = res * 31 + hash<long>()(c.x);
			res = res * 31 + hash<long>()(c.y);
			res = res * 31 + hash<long>()(c.z);
			return res;
		}
	};
}

struct Vec2 {
	Vec2() = default;

	Vec2(long x, long y) : x{x}, y{y} {}

	long x;
	long y;

	friend auto operator<=>(const Vec2 &, const Vec2 &) = default;

	stream_io(Vec2, x, y);
};
namespace std {
	template<>
	struct hash<Vec2> {
		size_t operator()(const Vec2 &c) const {
			// http://stackoverflow.com/a/1646913/126995
			size_t res = 17;
			res = res * 31 + hash<long>()(c.x);
			res = res * 31 + hash<long>()(c.y);
			return res;
		}
	};
}

/**
 * Applies f to each element of the container
 * @param cont The container
 * @param f The function to apply. Needs to have the following signature: f(ulong l, ulong c)
 */
template<typename Container, typename Function>
requires requires(Container cont, Function f) {f(0u, 0u);}
void loop2d(Container &&cont, Function &&f) {
	auto vsize = cont.size();
	auto hsize = vsize == 0 ? 0 : cont[0].size();
	for (int l = 0; l < vsize; ++l) {
		for (int c = 0; c < hsize; ++c) {
			f(l, c);
		}
	}
}

/**
 * Applies f to each element of the container
 * @param cont The container (passed by reference)
 * @param f The function to apply. Needs to have the following signature: f(Container::value_type &val)
 */
template<typename Container, typename Function>
requires requires(Container cont, Function f) {f(cont[0][0]);}
void loop2d(Container &cont, Function &&f) {
	for (auto &&line: cont) {
		for (auto &&elem: line) {
			f(elem);
		}
	}
}

/**
 * Applies f to each element of the container
 * @param cont The container (passed by reference)
 * @param f The function to apply. Needs to have the following signature: f(ulong l, ulong c, Container::value_type &val)
 */
template<typename Container, typename Function>
requires requires(Container cont, Function f) {f(0u, 0u, cont[0][0]);}
void loop2d(Container &cont, Function &&f) {
	auto vsize = cont.size();
	auto hsize = vsize == 0 ? 0 : cont[0].size();
	for (int l = 0; l < vsize; ++l) {
		for (int c = 0; c < hsize; ++c) {
			f(l, c, cont[l][c]);
		}
	}
}

template<typename Container>
vector<std::reference_wrapper<typename Container::value_type::value_type>>
adjacents(Container &cont, ulong l, ulong c, bool diag = true) {
	auto vsize = cont.size();
	auto hsize = vsize == 0 ? 0 : cont[0].size();
	vector<std::reference_wrapper<typename Container::value_type::value_type>> result;
	for (int dl = -1; dl <= 1; ++dl) {
		for (int dc = -1; dc <= 1; ++dc) {
			if ((dl != 0 || dc != 0) && (diag || abs(dc + dl) == 1)) {
				auto adj_c = c + dc;
				auto adj_l = l + dl;
				if (adj_l >= 0 && adj_l < vsize &&
				    adj_c >= 0 && adj_c < hsize) {
					result.push_back(cont[adj_l][adj_c]);
				}
			}
		}
	}
	return result;
}

template<typename Container, typename Function>
requires requires(typename Container::value_type e1, typename Container::value_type e2, Function f){
	f(e1, e2);
}
void arrangements(Container &c, Function &&f) {
	for (auto &&e1: c) {
		for (auto &&e2: c) {
			f(e1, e2);
		}
	}
}

template<typename Node>
struct AStarResult {
	deque<Node> path;
	ulong cost;

	stream_io(AStarResult<Node>, path, cost);
};

template<typename T, typename = std::void_t<>>
struct is_std_hashable : std::false_type {
};

template<typename T>
struct is_std_hashable<T, std::void_t<decltype(std::declval<std::hash<T>>()(std::declval<T>()))>> : std::true_type {
};

template<typename T>
constexpr bool is_std_hashable_v = is_std_hashable<T>::value;

template<typename Key, typename Value>
using smart_map = std::conditional_t<is_std_hashable_v<Key>, unordered_map<Key, Value>, map<Key, Value>>;
template<typename Key>
using smart_set = std::conditional_t<is_std_hashable_v<Key>, unordered_set<Key>, set<Key>>;

template<typename NeighborsFunction,
		typename CostFunction,
		typename HeuristicFunction,
		typename Node>
requires requires(NeighborsFunction neighbors,
                  CostFunction cost,
                  HeuristicFunction heuristic,
                  const Node node) {
	{neighbors(node)} -> std::convertible_to<vector<Node>>;
	{cost(node, node)} -> std::convertible_to<ulong>;
	{heuristic(node, node)} -> std::convertible_to<ulong>;
	smart_map<Node, ulong>{};
}
AStarResult<Node> a_star(NeighborsFunction &&neighbors,
                         CostFunction &&cost,
                         HeuristicFunction &&heuristic,
                         Node &&start,
                         Node &&dest) {
	struct Path {
		Path(Node node, Path *parent, ulong cost) : node{move(node)}, parent{parent}, cost{cost} {}

		Node node;
		Path *parent;
		ulong cost;
	};
	auto comp = [&](const Path *a, const Path *b) -> bool {
		return a->cost + heuristic(a->node, dest) > b->cost + heuristic(b->node, dest);
	};

	deque<Path> paths{};
	deque<Path *> q{};

	paths.emplace_back(start, nullptr, 0);
	q.emplace_back(&paths.back());

	smart_map<Node, ulong> explored{};

	bool found = false;
	while (!q.empty()) {
		ranges::pop_heap(q, comp);
		auto *current = q.back();
		if (current->node == dest) {
			found = true;
			break;
		}
		q.pop_back();
		for (auto &&n: neighbors(current->node)) {
			auto n_cost = current->cost + cost(current->node, n);
			if (!explored.contains(n) || n_cost < explored[n]) {
				paths.emplace_back(n, current, n_cost);
				q.emplace_back(&paths.back());
				ranges::push_heap(q, comp);
				explored[n] = n_cost;
			}
		}
		explored[current->node] = 0;
	}
	deque<Node> result;
	ulong total_cost = 0;
	if (found) {
		Path *path = q.back();
		total_cost = path->cost;
		while (path) {
			result.emplace_front(path->node);
			path = path->parent;
		}
	}
	return {result, total_cost};
}


std::ostream &operator<<(std::ostream &os, const chrono::high_resolution_clock::duration &duration) {
	using namespace chrono;
	auto s = duration_cast<seconds>(duration).count();
	auto ms = duration_cast<milliseconds>(duration).count();
	auto us = duration_cast<microseconds>(duration).count();
	auto ns = duration_cast<nanoseconds>(duration).count();
	os << setprecision(3);
	if (s != 0) {
		os << ms / 1000. << " s";
	} else if (ms != 0) {
		os << us / 1000. << " ms";
	} else if (us != 0) {
		os << ns / 1000. << " µs";
	} else {
		os << ns << " ns";
	}
	return os;
}

istream &operator>>(istream &is, string &&s) {
	for (auto c: s) {
		if (is.get() != c) {
			throw runtime_error("Character not expected: " + to_string(c));
		}
	}
	return is;
}

template<typename T0, typename ... Ts>
std::ostream &operator<<(std::ostream &s, std::variant<T0, Ts...> const &v) {
	std::visit([&](auto &&arg) {s << arg;}, v);
	return s;
}

template<typename T>
std::ostream &operator<<(std::ostream &os, const std::optional<T> &o) {
	if (o)
		os << o.value();
	else
		os << "nullopt";
	return os;
}

struct Aoc {
	explicit Aoc(ifstream &f) : file{f} {}

	virtual ulong part1() = 0;
	virtual ulong part2() = 0;

	ifstream &file;

	template<typename AocClass>
	static void run(ulong expected1 = 0, ulong expected2 = 0, bool runReal = true, bool runTest = true) {
		using namespace chrono;
		ifstream test;
		std::optional<AocClass> aocTest;
		if (runTest) {
			string testFileLocation = INPUT_DIR "/test.txt";
			cout << "Reading test input file at " << testFileLocation << debug_color << '\n';
			test = ifstream{testFileLocation};
			aocTest.emplace(test);
		}

		ifstream real;
		std::optional<AocClass> aocReal;
		if (runReal) {
			string realFileLocation = INPUT_DIR "/input.txt";
			cout << reset << "Reading real input file at " << realFileLocation << debug_color << '\n';
			real = ifstream{realFileLocation};
			aocReal.emplace(real);
		}

		if (runTest) {
			cout << "\n" << bold << yellow << "### Part 1 - test ###" << reset << debug_color << '\n';
			test.seekg(ifstream::beg);
			auto start = high_resolution_clock::now();
			auto res1test = aocTest->part1();
			auto stop = high_resolution_clock::now();
			auto color = res1test == expected1 ? result_color : red;
			cout << reset << result_color << "Result = " << bold << color << res1test << reset << '\n';
			cout << time_color << "Execution time = " << stop - start << reset << endl;
		}

		if (runReal) {
			cout << "\n" << bold << green << "### Part 1 - real ###" << reset << debug_color << '\n';
			real.seekg(ifstream::beg);
			auto start = high_resolution_clock::now();
			auto res1real = aocReal->part1();
			auto stop = high_resolution_clock::now();
			cout << reset << result_color << "Result = " << bold << res1real << reset << '\n';
			cout << time_color << "Execution time = " << stop - start << reset << endl;
		}

		if (runTest) {
			cout << "\n" << bold << yellow << "### Part 2 - test ###" << reset << debug_color << '\n';
			test.seekg(ifstream::beg);
			auto start = high_resolution_clock::now();
			auto res2test = aocTest->part2();
			auto stop = high_resolution_clock::now();
			auto color = res2test == expected2 ? result_color : red;
			cout << reset << result_color << "Result = " << bold << color << res2test << reset << '\n';
			cout << time_color << "Execution time = " << stop - start << reset << endl;
		}

		if (runReal) {
			cout << "\n" << bold << green << "### Part 2 - real ###" << reset << debug_color << '\n';
			real.seekg(ifstream::beg);
			auto start = high_resolution_clock::now();
			auto res2real = aocReal->part2();
			auto stop = high_resolution_clock::now();
			cout << reset << result_color << "Result = " << bold << res2real << reset << '\n';
			cout << time_color << "Execution time = " << stop - start << reset << endl;
		}
	}

};