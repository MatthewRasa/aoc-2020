#include "common.h"
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

std::vector<uint> read_input() {
	std::vector<uint> input;
	for (std::string line; std::getline(std::cin, line); )
		input.push_back(std::stoul(line));
	return input;
}

std::size_t find_chain(const std::vector<uint> &adapters) {
	std::size_t diff1_count = 0;
	std::size_t diff3_count = 1;
	switch (adapters.front()) {
	case 1:
		++diff1_count;
		break;
	case 3:
		++diff3_count;
		break;
	}
	for (auto it = std::next(adapters.begin()); it != adapters.end(); ++it) {
		switch (*it - *std::prev(it)) {
		case 1:
			++diff1_count;
			break;
		case 3:
			++diff3_count;
			break;
		}
	}
	return diff1_count * diff3_count;
}

std::size_t enumerate_arrangements(std::vector<uint>::iterator begin, std::vector<uint>::iterator end,
		std::unordered_map<uint, std::size_t> &cache) {
	const auto dist = std::distance(begin, end);
	if (dist == 1)
		return 1;

	auto it = cache.find(dist);
	if (it == cache.end()) {
		std::size_t sum = 0;
		for (auto nit = std::next(begin); nit != end && *nit - *begin <= 3; ++nit)
			sum += enumerate_arrangements(nit, end, cache);
		it = cache.emplace(dist, sum).first;
	}
	return it->second;
}

std::size_t enumerate_arrangements(std::vector<uint>::iterator begin, std::vector<uint>::iterator end) {
	std::unordered_map<uint, std::size_t> cache{};
	return enumerate_arrangements(begin, end, cache);
}

int main(int argc, char *argv[]) {
	auto input = read_input();
	std::sort(input.begin(), input.end());
	switch (select_part(argc, argv)) {
	case 1:
		std::cout << find_chain(input) << std::endl;
		break;
	case 2:
		input.insert(input.begin(), 0);
		std::cout << enumerate_arrangements(input.begin(), input.end()) << std::endl;
		break;
	}
}
