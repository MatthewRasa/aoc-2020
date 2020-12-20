#include "common.h"
#include <array>
#include <iostream>
#include <unordered_map>
#include <vector>

static const std::array<uint, 7> INPUT{0,8,15,2,12,1,4};

static uint play_game(uint end_turn) {
	std::unordered_map<uint, std::vector<uint>> spoken;
	uint turn;
	for (turn = 0; turn < INPUT.size(); ++turn)
		spoken[INPUT[turn]].push_back(turn);

	uint last_spoken = INPUT[turn - 1];
	for (; turn < end_turn; ++turn) {
		const auto &spoken_turns = spoken[last_spoken];
		last_spoken = spoken_turns.size() < 2 ? 0 : spoken_turns.back() - spoken_turns[spoken_turns.size() - 2];
		spoken[last_spoken].push_back(turn);
	}
	return last_spoken;
}

int main(int argc, char *argv[]) {
	uint end_turn;
	switch (select_part(argc, argv)) {
	case 1:
		end_turn = 2020;
		break;
	case 2:
	default:
		end_turn = 30000000;
		break;
	}
	std::cout << play_game(end_turn) << std::endl;
	return 0;
}
