#include "common.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class Instruction {
public:

	enum class Cmd {
		NOP, ACC, JMP
	};

	Instruction(std::string line) {
		std::istringstream iss{std::move(line)};
		std::string scmd, sarg;
		std::getline(iss, scmd, ' ');
		std::getline(iss, sarg, ' ');
		if (scmd == "nop")
			m_cmd = Cmd::NOP;
		else if (scmd == "acc")
			m_cmd = Cmd::ACC;
		else if (scmd == "jmp")
			m_cmd = Cmd::JMP;
		else
			throw std::runtime_error{"Invalid command: " + scmd};
		m_arg = std::stoi(sarg);
	}

	Cmd cmd_type() const noexcept{
		return m_cmd;
	}

	int run(int &accumulator) const {
		switch (m_cmd) {
		case Cmd::NOP:
			return 1;
		case Cmd::ACC:
			accumulator += m_arg;
			return 1;
		case Cmd::JMP:
		default:
			return m_arg;
		}
	}

	void flip_cmd() noexcept {
		switch (m_cmd) {
		case Cmd::NOP:
			m_cmd = Cmd::JMP;
			break;
		case Cmd::JMP:
			m_cmd = Cmd::NOP;
			break;
		default:
			break;
		}
	}

private:

	Cmd m_cmd;
	int m_arg;

};

static std::vector<Instruction> read_input() {
	std::vector<Instruction> instructions;
	for (std::string line; std::getline(std::cin, line); )
		instructions.emplace_back(std::move(line));
	return instructions;
}

static std::pair<int, bool> run_program(const std::vector<Instruction> &instructions) {
	int accumulator = 0;
	std::vector<bool> visited(instructions.size(), false);
	for (int pc = 0; pc < static_cast<int>(instructions.size()); ) {
		if (visited[pc])
			return std::make_pair(accumulator, false);
		visited[pc] = true;
		pc += instructions[pc].run(accumulator);
	}
	return std::make_pair(accumulator, true);
}

static int fix_program(std::vector<Instruction> &instructions) {
	for (auto &instruction : instructions) {
		if (instruction.cmd_type() != Instruction::Cmd::ACC) {
			instruction.flip_cmd();
			auto [acc, success] = run_program(instructions);
			if (success)
				return acc;
			instruction.flip_cmd();
		}
	}
	throw std::runtime_error{"Unable to fix program"};
}

int main(int argc, char *argv[]) {
	auto instructions = read_input();
	switch (select_part(argc, argv)) {
	case 1:
		std::cout << run_program(instructions).first << std::endl;
		break;
	case 2:
		std::cout << fix_program(instructions) << std::endl;
		break;
	}
}
