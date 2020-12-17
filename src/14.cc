#include "common.h"
#include <iostream>
#include <numeric>
#include <string>
#include <unordered_map>
#include <vector>

struct Inst_Set {
	std::uint64_t zeros_mask{0xFFFFFFFFFFFFFFFF};
	std::uint64_t ones_mask{0x0000000000000000};
	std::vector<std::uint64_t> x_zeros_masks;
	std::vector<std::uint64_t> x_ones_masks;
	std::vector<std::pair<std::uint64_t, std::uint64_t>> insts;

	Inst_Set() = default;

	Inst_Set(std::string mask_str) {
		std::uint64_t select = 1ul;
		std::vector<std::size_t> x_positions;
		for (auto it = mask_str.rbegin(); it != mask_str.rend(); ++it, select <<= 1) {
			switch (*it) {
			case '0':
				zeros_mask &= ~select;
				break;
			case '1':
				ones_mask |= select;
				break;
			case 'X':
				x_positions.push_back(std::distance(mask_str.rbegin(), it));
				break;
			}
		}

		for (std::uint64_t val = 0; val < (1ul << x_positions.size()); ++val) {
			std::uint64_t zeros_mask{0xFFFFFFFFFFFFFFFF};
			std::uint64_t ones_mask{0x0000000000000000};
			for (std::size_t bi = 0; bi < x_positions.size(); ++bi) {
				if (val & (1ul << bi))
					ones_mask |= 1ul << x_positions[bi];
				else
					zeros_mask &= ~(1ul << x_positions[bi]);
			}
			x_zeros_masks.push_back(std::move(zeros_mask));
			x_ones_masks.push_back(std::move(ones_mask));
		}
	}

	void add_inst(std::string line) {
		const auto tok0_size = std::string{"mem["}.size();
		const auto pos1 = line.find("]", tok0_size);
		insts.emplace_back(
				std::stoul(line.substr(tok0_size, pos1 - tok0_size)),
				std::stoul(line.substr(pos1 + std::string{"] = "}.size())));
	}

};

using Address_Space = std::unordered_map<std::uint64_t, std::uint64_t>;

static std::vector<Inst_Set> read_input() {
	std::vector<Inst_Set> inst_sets;
	Inst_Set curr_set;
	for (std::string line; std::getline(std::cin, line); ) {
		if (line.find("mask") != std::string::npos) {
			if (!curr_set.insts.empty())
				inst_sets.push_back(std::move(curr_set));
			curr_set = Inst_Set{line.substr(std::string{"mask = "}.size())};
		} else {
			curr_set.add_inst(std::move(line));
		}
	}
	if (!curr_set.insts.empty())
		inst_sets.push_back(std::move(curr_set));
	return inst_sets;
}

static void run_program_v1(const std::vector<Inst_Set> &inst_sets, Address_Space &address_space) {
	for (const auto &inst_set : inst_sets) {
		for (const auto &[addr, val] : inst_set.insts)
			address_space[addr] = (val & inst_set.zeros_mask) | inst_set.ones_mask;
	}
}

static void run_program_v2(const std::vector<Inst_Set> &inst_sets, Address_Space &address_space) {
	for (const auto &inst_set : inst_sets) {
		for (const auto &[addr, val] : inst_set.insts) {
			const auto new_addr = addr | inst_set.ones_mask;
			for (std::size_t i = 0; i < inst_set.x_zeros_masks.size(); ++i)
				address_space[(new_addr & inst_set.x_zeros_masks[i]) | inst_set.x_ones_masks[i]] = val;
		}
	}
}

static std::uint64_t sum_addr_space(const Address_Space &address_space) {
	std::uint64_t sum = 0;
	for (const auto &[addr, val] : address_space)
		sum += val;
	return sum;
}

int main(int argc, char *argv[]) {
	const auto inst_sets = read_input();
	Address_Space addr_space;
	switch (select_part(argc, argv)) {
	case 1:
		run_program_v1(inst_sets, addr_space);
		break;
	case 2:
		run_program_v2(inst_sets, addr_space);
		break;
	}
	std::cout << sum_addr_space(addr_space) << std::endl;
	return 0;
}
