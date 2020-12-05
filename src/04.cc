#include "common.h"
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

class Passport {
public:

	static const std::vector<std::string> REQUIRED_FIELDS;
	static const std::unordered_set<std::string> EYE_COLORS;

	Passport(std::string line) {
		std::istringstream iss{std::move(line)};
		for (std::string part; std::getline(iss, part, ' '); ) {
			auto delim_pos = part.find(':');
			m_fields.emplace(part.substr(0, delim_pos), part.substr(delim_pos + 1));
		}
	}

	bool valid1() const {
		return fields_present();
	}

	bool valid2() const {
		return fields_present()
				&& valid_byr()
				&& valid_iyr()
				&& valid_eyr()
				&& valid_hgt()
				&& valid_hcl()
				&& valid_ecl()
				&& valid_pid();
	}

private:

	std::unordered_map<std::string, std::string> m_fields;

	bool fields_present() const {
		for (const auto &field : REQUIRED_FIELDS) {
			if (m_fields.find(field) == m_fields.end())
				return false;
		}
		return true;
	}

	bool valid_byr() const {
		return num_between(m_fields.at("byr"), 1920, 2002);
	}

	bool valid_iyr() const {
		return num_between(m_fields.at("iyr"), 2010, 2020);
	}

	bool valid_eyr() const {
		return num_between(m_fields.at("eyr"), 2020, 2030);
	}

	bool valid_hgt() const {
		const auto &hgt = m_fields.at("hgt");
		std::size_t unit_pos;
		if ((unit_pos = hgt.find("cm")) != std::string::npos)
			return num_between(hgt.substr(0, unit_pos), 150, 193);
		else if ((unit_pos = hgt.find("in")) != std::string::npos)
			return num_between(hgt.substr(0, unit_pos), 59, 76);
		else
			return false;
	}

	bool valid_hcl() const {
		const auto &hcl = m_fields.at("hcl");
		if (hcl.size() != 7)
			return false;
		if (hcl[0] != '#')
			return false;
		for (std::size_t i = 1; i < 7; ++i) {
			if ((hcl[i] < '0' || hcl[i] > '9') && (hcl[i] < 'a' || hcl[i] > 'f'))
				return false;
		}
		return true;
	}

	bool valid_ecl() const {
		return EYE_COLORS.find(m_fields.at("ecl")) != EYE_COLORS.end();
	}

	bool valid_pid() const {
		if (m_fields.at("pid").size() != 9)
			return false;
		for (const auto &c : m_fields.at("pid")) {
			if (c < '0' || c > '9')
				return false;
		}
		return true;
	}

	bool num_between(const std::string &val, uint min, uint max) const {
		auto lval = std::stoul(val);
		return min <= lval && lval <= max;
	}

};

const std::vector<std::string> Passport::REQUIRED_FIELDS {
	"byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"
};

const std::unordered_set<std::string> Passport::EYE_COLORS {
	"amb", "blu", "brn", "gry", "grn", "hzl", "oth"
};

std::vector<Passport> read_input() {
	std::vector<Passport> passports;
	std::string full_line;
	for (std::string line; std::getline(std::cin, line); ) {
		if (line.empty()) {
			passports.emplace_back(std::move(full_line));
			full_line.clear();
		} else {
			if (!full_line.empty())
				full_line += ' ';
			full_line += line;
		}
	}
	if (!full_line.empty())
		passports.emplace_back(std::move(full_line));
	return passports;
}

int main(int argc, char *argv[]) {
	auto valid_func = select_part(argc, argv) == 1 ? [](const Passport &p) { return p.valid1(); } : [](const Passport &p) { return p.valid2(); };
	auto passports = read_input();
	std::cout << std::count_if(passports.begin(), passports.end(), std::move(valid_func)) << std::endl;
	return 0;
}
