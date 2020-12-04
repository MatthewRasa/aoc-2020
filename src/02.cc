#include "common.h"
#include <algorithm>
#include <iostream>
#include <functional>
#include <sstream>
#include <cstring>

struct Input {
	std::size_t start, end;
	char c;
	std::string password;
};

static Input parse_input_line(std::string line) {
	Input input;
	std::string range, cstr, password;
	std::istringstream ss{std::move(line)};
	ss >> range >> cstr >> input.password;

	std::istringstream rss{range};
	std::string sstart, send;
	std::getline(rss, sstart, '-');
	std::getline(rss, send, '-');

	input.start = std::stoul(sstart);
	input.end = std::stoul(send);
	input.c = cstr[0];
	return input;
}

static bool part1_is_valid(std::size_t start, std::size_t end, char c, const std::string &password) {
	std::size_t count = std::count(password.begin(), password.end(), c);
	return start <= count && count <= end;
}

static bool part2_is_valid(std::size_t start, std::size_t end, char c, const std::string &password) {
	return (password[start - 1] == c) != (password[end - 1] == c);
}

int main(int argc, char *argv[]) {
	std::function<bool(std::size_t, std::size_t, char, const std::string &)> is_valid_func;
	switch (select_part(argc, argv)) {
	case 1:
		is_valid_func = part1_is_valid;
		break;
	case 2:
		is_valid_func = part2_is_valid;
		break;
	}

	std::size_t count = 0;
	for (std::string line; std::getline(std::cin, line); ) {
		auto input = parse_input_line(std::move(line));
		count += is_valid_func(input.start, input.end, input.c, input.password);
	}
	std::cout << count << std::endl;
	return 0;
}
