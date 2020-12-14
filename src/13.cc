#include "common.h"
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

static std::pair<unsigned long, std::vector<unsigned long>> read_input() {
	std::string line;
	std::getline(std::cin, line);
	unsigned long estimate = std::stoul(line);

	std::vector<unsigned long> bus_ids;
	std::getline(std::cin, line);
	std::istringstream iss{std::move(line)};
	for (std::string bus_id; std::getline(iss, bus_id, ','); )
		bus_ids.push_back(bus_id == "x" ? 0 : std::stoul(bus_id));

	return std::make_pair(estimate, bus_ids);
}

static std::pair<unsigned long, unsigned long> find_earliest(unsigned long estimate, const std::vector<unsigned long> &bus_ids) {
	unsigned long earliest = 0;
	unsigned long min_wait_time = std::numeric_limits<unsigned long>::max();
	for (auto bus_id : bus_ids) {
		if (bus_id != 0) {
			const auto bus_time = bus_id * ((estimate + bus_id - 1) / bus_id);
			const auto wait_time = bus_time - estimate;
			if (wait_time < min_wait_time) {
				min_wait_time = wait_time;
				earliest = bus_id;
			}
		}
	}
	return std::make_pair(earliest, min_wait_time);
}

static std::unordered_map<unsigned long, std::size_t> make_sparse(const std::vector<unsigned long> &bus_ids) {
	std::unordered_map<unsigned long, std::size_t> sparse_ids;
	for (auto it = bus_ids.begin(); it != bus_ids.end(); ++it) {
		if (*it != 0)
			sparse_ids.emplace(*it, std::distance(bus_ids.begin(), it));
	}
	return sparse_ids;
}

static long solve_contest(const std::vector<unsigned long> &bus_id_list) {
	const auto bus_ids = make_sparse(bus_id_list);
	auto max_bus_id_it = std::max_element(bus_ids.begin(), bus_ids.end());
	auto [max_id, max_offset] = *max_bus_id_it;

	for (long t = -max_offset; ; t += max_id) {
		for (const auto &[id, offset] : bus_ids) {
			if ((t + offset) % id != 0)
				goto next_time;
		}
		return t;

next_time:
		continue;
	}
}

int main(int argc, char *argv[]) {
	auto [estimate, bus_ids] = read_input();
	switch (select_part(argc, argv)) {
	case 1:
	{
		auto [bus_id, wait_time] = find_earliest(estimate, bus_ids);
		std::cout << (bus_id * wait_time) << std::endl;
		break;
	}
	case 2:
		std::cout << solve_contest(bus_ids) << std::endl;
		break;
	}
}
