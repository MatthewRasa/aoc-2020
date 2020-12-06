#include "common.h"
#include <algorithm>
#include <numeric>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

class Group {
public:

	Group(std::vector<std::string> members)
			: m_nmembers{members.size()} {
		for (const auto &member : members) {
			for (auto q : member) {
				auto q_it = m_questions.find(q);
				if (q_it == m_questions.end())
					m_questions[q] = 1;
				else
					++q_it->second;
			}
		}
	}

	std::size_t count1() const noexcept {
		return m_questions.size();
	}

	std::size_t count2() const noexcept {
		return std::count_if(m_questions.begin(), m_questions.end(), [this](const auto &pair) {
			return pair.second == m_nmembers;
		});
	}

private:

	std::size_t m_nmembers;
	std::unordered_map<char, std::size_t> m_questions;
};

std::vector<Group> read_input() {
	std::vector<Group> groups;
	std::vector<std::string> members;
	for (std::string line; std::getline(std::cin, line); ) {
		if (line.empty()) {
			groups.emplace_back(std::move(members));
			members.clear();
		} else {
			members.push_back(std::move(line));
		}
	}
	if (!members.empty())
		groups.emplace_back(std::move(members));
	return groups;
}

int main(int argc, char *argv[]) {
	auto groups = read_input();
	std::size_t sum = 0;
	switch (select_part(argc, argv)) {
	case 1:
		for (const auto &group : groups)
			sum += group.count1();
		break;
	case 2:
		for (const auto &group : groups)
			sum += group.count2();
		break;
	}
	std::cout << sum << std::endl;
	return 0;
}
