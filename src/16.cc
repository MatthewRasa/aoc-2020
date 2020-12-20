#include "common.h"
#include <iostream>
#include <numeric>
#include <sstream>
#include <unordered_map>
#include <vector>

class Range {
public:

	Range() = default;

	Range(std::string line) {
		const auto delim = line.find('-');
		m_min = std::stoul(line.substr(0, delim));
		m_max = std::stoul(line.substr(delim + 1));
	}

	bool in_range(uint val) const noexcept {
		return m_min <= val && val <= m_max;
	}

private:

	uint m_min, m_max;

};

class Rule {
public:

	Rule(std::string line) {
		const auto colon_delim = line.find(':');
		m_field = line.substr(0, colon_delim);

		const auto or_delim = line.find("or", colon_delim + 2);
		m_range1 = Range{line.substr(colon_delim + 2, or_delim - 1 - (colon_delim + 2))};
		m_range2 = Range{line.substr(or_delim + 3)};
	}

	const std::string &field() const noexcept {
		return m_field;
	}

	bool valid_value(uint val) const noexcept {
		return m_range1.in_range(val) || m_range2.in_range(val);
	}

private:

	std::string m_field;
	Range m_range1, m_range2;

};

using Ticket = std::vector<uint>;

struct Info {
	std::vector<Rule> rules;
	Ticket my_ticket;
	std::vector<Ticket> nearby_tickets;
};

static Ticket parse_ticket_line(std::string line) {
	Ticket ticket;
	std::istringstream iss{std::move(line)};
	for (std::string num_str; std::getline(iss, num_str, ','); )
		ticket.push_back(std::stoul(num_str));
	return ticket;
}

static Info read_input() {
	Info info;
	enum class Phase { RULES, MY_TICKET, NEARBY_TICKETS };
	Phase phase = Phase::RULES;
	for (std::string line; std::getline(std::cin, line); ) {
		if (line.empty())
			continue;
		if (line == "your ticket:") {
			phase = Phase::MY_TICKET;
			continue;
		}
		if (line == "nearby tickets:") {
			phase = Phase::NEARBY_TICKETS;
			continue;
		}
		switch (phase) {
		case Phase::RULES:
			info.rules.emplace_back(std::move(line));
			break;
		case Phase::MY_TICKET:
			info.my_ticket = parse_ticket_line(std::move(line));
			break;
		case Phase::NEARBY_TICKETS:
			info.nearby_tickets.push_back(parse_ticket_line(std::move(line)));
			break;
		}
	}
	return info;
}

static bool valid_for_rules(uint val, const std::vector<Rule> &rules) {
	for (const auto &rule : rules) {
		if (rule.valid_value(val))
			return true;
	}
	return false;
}

static bool valid_rule_position(std::size_t pos, const Rule &rule, const std::vector<Ticket> &tickets) {
	for (const auto &ticket : tickets) {
		if (!rule.valid_value(ticket[pos]))
			return false;
	}
	return true;
}

static bool valid_ticket(const Ticket &ticket, const std::vector<Rule> &rules) {
	for (auto val : ticket) {
		if (!valid_for_rules(val, rules))
			return false;
	}
	return true;
}

static std::vector<Ticket> valid_tickets(const Info &info) {
	std::vector<Ticket> valid_tickets;
	std::copy_if(info.nearby_tickets.begin(), info.nearby_tickets.end(), std::back_inserter(valid_tickets),
			[&info](const auto &ticket) { return valid_ticket(ticket, info.rules); });
	return valid_tickets;
}

static uint calc_error_rate(const Info &info) {
	uint error_rate = 0;
	for (const auto &ticket : info.nearby_tickets) {
		for (auto val : ticket) {
			if (!valid_for_rules(val, info.rules))
				error_rate += val;
		}
	}
	return error_rate;
}

static std::unordered_map<const Rule *, std::size_t> solve_field_positions(std::vector<std::vector<const Rule *>> &pos_rule_mapping) {
	std::unordered_map<const Rule *, std::size_t> positions;
	while (positions.size() < pos_rule_mapping.size()) {
		const auto it = std::find_if(pos_rule_mapping.begin(), pos_rule_mapping.end(), [](const auto &rule_options) { return rule_options.size() == 1; });
		if (it == pos_rule_mapping.end())
			throw std::logic_error{"More than one possible position for rule"};
		const auto p_rule = it->front();
		positions.emplace(p_rule, std::distance(pos_rule_mapping.begin(), it));
		for (auto &rule_options : pos_rule_mapping)
			rule_options.erase(std::remove_if(rule_options.begin(), rule_options.end(),
					[it, p_rule](const auto &rule) { return rule->field() == p_rule->field(); }), rule_options.end());
	}
	return positions;
}

static std::size_t determine_fields(const Info &info) {
	std::vector<std::vector<const Rule *>> pos_rule_mapping;
	auto tickets = valid_tickets(info);
	for (std::size_t pos = 0; pos < tickets[0].size(); ++pos) {
		std::vector<const Rule *> rule_options;
		for (const auto &rule : info.rules) {
			if (valid_rule_position(pos, rule, tickets))
				rule_options.push_back(&rule);
		}
		pos_rule_mapping.push_back(std::move(rule_options));
	}

	std::size_t sum = 1;
	for (const auto &[p_rule, pos] : solve_field_positions(pos_rule_mapping)) {
		if (p_rule->field().find("departure") != std::string::npos)
			sum *= info.my_ticket[pos];
	}
	return sum;
}

int main(int argc, char *argv[]) {
	const auto info = read_input();
	switch (select_part(argc, argv)) {
	case 1:
		std::cout << calc_error_rate(info) << std::endl;
		break;
	case 2:
		std::cout << determine_fields(info) << std::endl;
		break;
	}
	return 0;
}
