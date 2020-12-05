#include "common.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

static const std::unordered_map<char, std::uint8_t> BINARY_VALUES {
	{'F', 0},
	{'B', 1},
	{'L', 0},
	{'R', 1}
};

class Seat {
public:

	Seat(std::string seat_str)
			: m_row{0}, m_col{0} {
		for (std::size_t i = 0; i < 7; ++i)
			m_row |= BINARY_VALUES.at(seat_str[i]) << (6 - i);
		for (std::size_t i = 7; i < 10; ++i)
			m_col |= BINARY_VALUES.at(seat_str[i]) << (9 - i);
		m_seat_id = m_row * 8 + m_col;
	}

	uint seat_id() const noexcept {
		return m_seat_id;
	}

	bool operator<(const Seat &other) const noexcept {
		return seat_id() < other.seat_id();
	}

private:

	std::uint8_t m_row;
	std::uint8_t m_col;
	uint m_seat_id;

};

std::vector<Seat> read_input() {
	std::vector<Seat> seats;
	for (std::string line; std::getline(std::cin, line); )
		seats.emplace_back(std::move(line));
	return seats;
}

int main(int argc, char *argv[]) {
	auto seats = read_input();
	switch (select_part(argc, argv)) {
	case 1:
		std::cout << std::max_element(seats.begin(), seats.end())->seat_id() << std::endl;
		break;
	case 2:
		std::sort(seats.begin(), seats.end());
		for (auto it = std::next(seats.begin()); it != seats.end(); ++it) {
			if (it->seat_id() - std::prev(it)->seat_id() == 2) {
				std::cout << it->seat_id() - 1 << std::endl;
				break;
			}
		}
		break;
	}
	return 0;
}
