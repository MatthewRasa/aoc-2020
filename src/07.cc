#include "common.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class Bag {
public:

	Bag(std::string color) noexcept
			: m_color{std::move(color)} { }

	const std::string &color() const noexcept {
		return m_color;
	}

	const std::vector<Bag *> &parents() const noexcept {
		return m_parents;
	}

	const std::vector<std::pair<Bag *, std::size_t>> &children() const noexcept {
		return m_children;
	}

	void add_child(Bag &bag, std::size_t count) {
		m_children.push_back(std::make_pair(&bag, count));
		bag.m_parents.push_back(this);
	}

private:

	std::string m_color;
	std::vector<Bag *> m_parents;
	std::vector<std::pair<Bag *, std::size_t>> m_children;

};

static std::unordered_map<std::string, Bag> read_input() {
	std::unordered_map<std::string, Bag> bags;
	std::unordered_map<std::string, std::vector<std::pair<std::string, std::size_t>>> bag_subcolor_lists;
	for (std::string line; std::getline(std::cin, line); ) {
		std::istringstream line_stream{std::move(line)};

		std::string color;
		for (std::string word; std::getline(line_stream, word, ' '); ) {
			if (word == "contain") {
				break;
			} else if (word != "bags") {
				if (!color.empty())
					color += ' ';
				color += std::move(word);
			}
		}
		const auto &bag = bags.emplace(color, Bag{color}).first->second;

		std::vector<std::pair<std::string, std::size_t>> bag_subcolors;
		color.clear();
		std::size_t count = 0;
		for (std::string word; std::getline(line_stream, word, ' '); ) {
			if (word.find("bag") != std::string::npos) {
				bag_subcolors.emplace_back(std::move(color), count);
				color.clear();
				count = 0;
			} else {
				if (count == 0) {
					if (word == "no")
						break;
					count = std::stoul(word);
				} else {
					if (!color.empty())
						color += ' ';
					color += std::move(word);
				}
			}
		}

		bag_subcolor_lists.emplace(bag.color(), std::move(bag_subcolors));
	}

	for (auto &[color, bag] : bags) {
		for (const auto &[sub_color, sub_count] : bag_subcolor_lists.at(color))
			bag.add_child(bags.at(sub_color), sub_count);
	}

	return bags;
}

std::unordered_set<std::string> parent_bags(const Bag &curr) {
	std::unordered_set<std::string> colors;
	for (const auto &p_parent : curr.parents()) {
		colors.insert(p_parent->color());
		auto parent_colors = parent_bags(*p_parent);
		colors.insert(parent_colors.begin(), parent_colors.end());
	}
	return colors;
}

std::size_t count_child_bags(const Bag &curr) {
	std::size_t total = 0;
	for (const auto &[p_bag, count] : curr.children())
		total += count + count * count_child_bags(*p_bag);
	return total;
}

int main(int argc, char *argv[]) {
	const auto bags = read_input();
	const auto &mine = bags.at("shiny gold");
	switch (select_part(argc, argv)) {
	case 1:
		std::cout << parent_bags(mine).size() << std::endl;
		break;
	case 2:
		std::cout << count_child_bags(mine) << std::endl;
		break;
	}
}
