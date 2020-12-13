#include "common.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

std::vector<unsigned long> read_input() {
	std::vector<unsigned long> input;
	for (std::string line; std::getline(std::cin, line); )
		input.push_back(std::stoul(line));
	return input;
}

unsigned long find_invalid_num(const std::vector<unsigned long> &nums) {
	for (auto it = nums.begin() + 25; it != nums.end(); ++it) {
		try {
			two_sum(it - 25, it, *it);
		} catch (const std::logic_error &) {
			return *it;
		}
	}
	throw std::logic_error{"no invalid number found"};
}

unsigned long find_weakness(const std::vector<unsigned long> &nums, unsigned long invalid_num) {
	auto start_it = nums.begin();
	auto end_it = nums.begin() + 2;
	auto sum = *start_it + *(start_it + 1);
	while (sum != invalid_num) {
		while (sum < invalid_num) {
			if (end_it == nums.end())
				throw std::logic_error{"no weakness found"};
			sum += *end_it;
			++end_it;
		}
		while (sum > invalid_num) {
			sum -= *start_it;
			++start_it;
			if (end_it - start_it == 1) {
				if (end_it == nums.end())
					throw std::logic_error{"no weakness found"};
				sum += *end_it;
				++end_it;
			}
		}
	}
	return *std::min_element(start_it, end_it) + *std::max_element(start_it, end_it);
}

int main(int argc, char *argv[]) {
	auto input = read_input();
	auto invalid_num = find_invalid_num(input);
	switch (select_part(argc, argv)) {
	case 1:
		std::cout << invalid_num << std::endl;
		break;
	case 2:
		std::cout << find_weakness(input, invalid_num) << std::endl;
		break;
	}
	return 0;
}
