#include "aoclib.hpp"
#include <ranges>
#include <bitset>

struct packet {
	static pair<unique_ptr<packet>, uint> parse(const string &s, uint pos = 0);

	uint version;
	uint type;

	virtual ostream &print_packet(ostream &os, int spaces) = 0;
	virtual ulong calc_value() = 0;

	virtual ~packet() = default;
};

struct literal : public packet {
	static pair<unique_ptr<literal>, uint> parse(const string &s, uint pos = 0);
	ulong nb;
	ostream &print_packet(ostream &os, int indent) override;
	ulong calc_value() override;
	~literal() override = default;
};

struct operation : public packet {
	static pair<unique_ptr<operation>, uint> parse(const string &s, uint pos = 0);

	vector<unique_ptr<packet>> sub_packets;

	ostream &print_packet(ostream &os, int indent) override;
	ulong calc_value() override;
	~operation() override = default;
};

pair<unique_ptr<packet>, uint> packet::parse(const string &s, uint pos) {
	if (s.size() - pos < 6) {
		return {nullptr, pos};
	}
	unique_ptr<packet> pkt{};
	auto version = bitset<3>{s, pos, 3}.to_ulong();
	pos += 3;
	auto type = bitset<3>{s, pos, 3}.to_ulong();
	pos += 3;
	if (type == 4) {
		std::tie(pkt, pos) = literal::parse(s, pos);
	} else {
		std::tie(pkt, pos) = operation::parse(s, pos);
	}
	pkt->type = type;
	pkt->version = version;
	return {move(pkt), pos};
}

pair<unique_ptr<operation>, uint> operation::parse(const string &s, uint pos) {
	auto op = make_unique<operation>();
	char length_type = s[pos];
	pos += 1;
	if (length_type == '0') { // length in nb of bits
		const bitset<15> &length_bs = bitset<15>{s, pos, 15};
		auto length = length_bs.to_ulong();
		pos += 15;
		auto end_pos = pos + length;
		while (pos < end_pos) {
			unique_ptr<packet> pkt{};
			std::tie(pkt, pos) = packet::parse(s, pos);
			op->sub_packets.push_back(move(pkt));
		}
	} else {                  // length in nb of sub-packets
		auto length = bitset<11>{s, pos, 11}.to_ulong();
		pos += 11;
		for (int i = 0; i < length; i++) {
			unique_ptr<packet> pkt{};
			std::tie(pkt, pos) = packet::parse(s, pos);
			op->sub_packets.push_back(move(pkt));
		}
	}
	return {move(op), pos};
}

ostream &operation::print_packet(ostream &os, int indent) {
	auto spaces = genSpaces(indent);
	os <<
	   spaces << bold << "OPERATOR" << no_bold << '\n' <<
	   spaces << "version = " << version << '\n' <<
	   spaces << "pkttype = " << type << '\n' <<
	   spaces << "packets = [" << '\n';
	for (auto &&pkt: sub_packets) {
		pkt->print_packet(os, indent + 1);
	}
	return os << spaces << "]\n";
}

ulong operation::calc_value() {
	switch (type) {
		case 0: // sum
			return accumulate(sub_packets.begin(), sub_packets.end(), 0ull,
			                  [](ulong a, const unique_ptr<packet> &p) {
				                  return a + p->calc_value();
			                  });
		case 1: // product
			return accumulate(sub_packets.begin(), sub_packets.end(), 1ull,
			                  [](ulong a, const unique_ptr<packet> &p) {
				                  return a * p->calc_value();
			                  });
		case 2: // min
			return min_element(sub_packets.begin(), sub_packets.end(),
			                   [](const unique_ptr<packet> &a, const unique_ptr<packet> &b) {
				                   return a->calc_value() < b->calc_value();
			                   })->get()->calc_value();
		case 3: // max
			return max_element(sub_packets.begin(), sub_packets.end(),
			                   [](const unique_ptr<packet> &a, const unique_ptr<packet> &b) {
				                   return a->calc_value() < b->calc_value();
			                   })->get()->calc_value();
		case 5: // greater than
			return sub_packets[0]->calc_value() > sub_packets[1]->calc_value();
		case 6: // less than
			return sub_packets[0]->calc_value() < sub_packets[1]->calc_value();
		case 7: // equal
			return sub_packets[0]->calc_value() == sub_packets[1]->calc_value();
		default:
			assert(false);
	}
}


pair<unique_ptr<literal>, uint> literal::parse(const string &s, uint pos) {
	auto lit = make_unique<literal>();
	char prefix;
	string number{};
	do {
		prefix = s[pos];
		pos += 1;
		number.append(s.substr(pos, 4));
		pos += 4;
	} while (prefix == '1');
	lit->nb = stoull(number, nullptr, 2);
	return {move(lit), pos};
}

ostream &literal::print_packet(ostream &os, int indent) {
	auto spaces = genSpaces(indent);
	return os <<
	          spaces << bold << "LITTERAL\n" << no_bold <<
	          spaces << "version = " << version << '\n' <<
	          spaces << "pkttype = " << type << '\n' <<
	          spaces << "literal = " << nb << '\n';
}

ulong literal::calc_value() {
	return nb;
}


struct Day16 : public Aoc {
	string bin_input;

	explicit Day16(ifstream &f) : Aoc{f} {
		auto hex_input = readLine(f);
		for (auto &&c: hex_input) {
			uint i;
			istringstream ss{string{c}};
			ss >> hex >> i;
			std::bitset<4> bs{i};
			bin_input.append(bs.to_string());
		}
	}

	static ulong sum_version_nb(packet *pkt) {
		if (dynamic_cast<literal *>(pkt)) {
			return pkt->version;
		} else {
			auto &sub_packets = static_cast<operation *>(pkt)->sub_packets;
			return accumulate(sub_packets.begin(), sub_packets.end(), pkt->version,
			                  [](ulong v, const unique_ptr<packet> &p) {
				                  return v + sum_version_nb(p.get());
			                  });
		}
	}

	ulong part1() override {
		auto[pkt, pos] = packet::parse(bin_input);
		pkt->print_packet(cout, 0);
		return sum_version_nb(pkt.get());
	}

	ulong part2() override {
		auto[pkt, pos] = packet::parse(bin_input);
		return pkt->calc_value();
	}
};

int main() {
	Aoc::run<Day16>(31, 54);
}
