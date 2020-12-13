#include "common.h"
#include <iostream>
#include <string>
#include <vector>

static std::vector<std::pair<char, uint>> read_input() {
	std::vector<std::pair<char, uint>> input;
	for (std::string line; std::getline(std::cin, line); )
		input.push_back(std::make_pair(line[0], std::stoul(line.substr(1))));
	return input;
}

static int clamp_direction(int value_degrees) {
	while (value_degrees < 0)
		value_degrees += 360;
	return value_degrees % 360;
}

static int process_instructions1(const std::vector<std::pair<char, uint>> &instructions) {
	int ns_pos = 0, ew_pos = 0;
	int direction = 90;
	for (const auto &[cmd, val] : instructions) {
		switch (cmd) {
		case 'N':
			ns_pos += val;
			break;
		case 'S':
			ns_pos -= val;
			break;
		case 'E':
			ew_pos += val;
			break;
		case 'W':
			ew_pos -= val;
			break;
		case 'L':
			direction = clamp_direction(direction - val);
			break;
		case 'R':
			direction = clamp_direction(direction + val);
			break;
		case 'F':
			switch (direction) {
			case 0:
				ns_pos += val;
				break;
			case 90:
				ew_pos += val;
				break;
			case 180:
				ns_pos -= val;
				break;
			case 270:
				ew_pos -= val;
				break;
			default:
				throw std::logic_error{"unexpected value for direction: " + direction};
			}
			break;
		}
	}
	return std::abs(ns_pos) + std::abs(ew_pos);
}

static int process_instructions2(const std::vector<std::pair<char, uint>> &instructions) {
	int ns_pos = 0, ew_pos = 0, wp_ns_pos = 1, wp_ew_pos = 10;
	int tmp;
	for (const auto &[cmd, val] : instructions) {
		switch (cmd) {
		case 'N':
			wp_ns_pos += val;
			break;
		case 'S':
			wp_ns_pos -= val;
			break;
		case 'E':
			wp_ew_pos += val;
			break;
		case 'W':
			wp_ew_pos -= val;
			break;
		case 'L':
			switch (val) {
			case 0:
				break;
			case 90:
				tmp = wp_ns_pos;
				wp_ns_pos = wp_ew_pos;
				wp_ew_pos = -tmp;
				break;
			case 180:
				wp_ns_pos = -wp_ns_pos;
				wp_ew_pos = -wp_ew_pos;
				break;
			case 270:
				tmp = wp_ns_pos;
				wp_ns_pos = -wp_ew_pos;
				wp_ew_pos = tmp;
				break;
			default:
				throw std::logic_error{"unexpected value for direction: " + val};
			}
			break;
		case 'R':
			switch (val) {
			case 0:
				break;
			case 90:
				tmp = wp_ns_pos;
				wp_ns_pos = -wp_ew_pos;
				wp_ew_pos = tmp;
				break;
			case 180:
				wp_ns_pos = -wp_ns_pos;
				wp_ew_pos = -wp_ew_pos;
				break;
			case 270:
				tmp = wp_ns_pos;
				wp_ns_pos = wp_ew_pos;
				wp_ew_pos = -tmp;
				break;
			default:
				throw std::logic_error{"unexpected value for direction: " + val};
			}
			break;
		case 'F':
			ns_pos += wp_ns_pos * val;
			ew_pos += wp_ew_pos * val;
			break;
		}
	}
	return std::abs(ns_pos) + std::abs(ew_pos);
}

int main(int argc, char *argv[]) {
	auto input = read_input();
	switch (select_part(argc, argv)) {
	case 1:
		std::cout << process_instructions1(input) << std::endl;
		break;
	case 2:
		std::cout << process_instructions2(input) << std::endl;
		break;
	}
}
