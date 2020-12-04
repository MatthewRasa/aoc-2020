#include "common.h"
#include <iostream>
#include <vector>
#include <set>
#include <tuple>
#include <cstring>

template<class Iter>
static std::pair<uint, uint> two_sum(Iter begin, Iter end, uint total) {
	std::set<uint> diffs;
	for (auto it = begin; it != end; ++it) {
		const auto diff = total - *it;
		if (diffs.find(*it) != diffs.end())
			return std::make_pair(diff, *it);
		diffs.insert(diff);
	}
	throw std::runtime_error{"no match found"};
}

template<class Iter>
static std::tuple<uint, uint, uint> three_sum(Iter begin, Iter end, uint total) {
	for (auto it = begin; it != end; ++it) {
		try {
			auto [val1, val2] = two_sum(it + 1, end, total - *it);
			return std::make_tuple(val1, val2, *it);
		} catch (const std::exception &ex) { }
	}
	throw std::runtime_error{"no match found"};
}

static std::vector<uint> read_input() {
	std::vector<uint> input;
	for (std::string line; std::getline(std::cin, line); )
		input.push_back(std::stoul(line));
	return input;
}

int main(int argc, char *argv[]) {
	auto part = select_part(argc, argv);
	const auto input = read_input();
	if (part == 1) {
		auto [val1, val2] = two_sum(input.begin(), input.end(), 2020);
		std::cout << (val1 * val2) << std::endl;
	} else {
		auto [val1, val2, val3] = three_sum(input.begin(), input.end(), 2020);
		std::cout << (val1 * val2 * val3) << std::endl;
	}
	return 0;
}
