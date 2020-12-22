#include "common.h"
#include <array>
#include <iostream>
#include <vector>

static constexpr std::size_t GRID_SIZE = 32;

struct Grid {
	std::size_t y_start, y_end, x_start, x_end, z_start, z_end, w_start, w_end;
	std::array<std::array<std::array<std::array<bool, GRID_SIZE>, GRID_SIZE>, GRID_SIZE>, GRID_SIZE> data;
};

static Grid read_input() {
	std::vector<std::vector<bool>> input;
	for (std::string line; std::getline(std::cin, line); ) {
		std::vector<bool> input_line;
		for (auto c : line)
			input_line.push_back(c == '#');
		input.push_back(std::move(input_line));
	}

	Grid grid{
		(GRID_SIZE - input.size()) / 2, (GRID_SIZE + input.size()) / 2,
		(GRID_SIZE - input[0].size()) / 2, (GRID_SIZE + input[0].size()) / 2,
		GRID_SIZE / 2, GRID_SIZE / 2 + 1,
		GRID_SIZE / 2, GRID_SIZE / 2 + 1,
		{}};
	for (std::size_t r = grid.y_start, input_r = 0; r < grid.y_end; ++r, ++input_r) {
		for (std::size_t c = grid.x_start, input_c = 0; c < grid.x_end; ++c, ++input_c) {
			for (std::size_t d = grid.z_start; d < grid.z_end; ++d) {
				for (std::size_t w = grid.w_start; w < grid.w_end; ++w)
					grid.data[r][c][d][w] = input[input_r][input_c];
			}
		}
	}
	return grid;
}

static std::size_t count_active_neighbors(const Grid &grid, std::size_t r, std::size_t c, std::size_t d, std::size_t w) {
	std::size_t count = 0;
	for (std::size_t ri = r - 1; ri <= r + 1; ++ri) {
		for (std::size_t ci = c - 1; ci <= c + 1; ++ci) {
			for (std::size_t di = d - 1; di <= d + 1; ++di) {
				for (std::size_t wi = w - 1; wi <= w + 1; ++wi) {
					if (ri != r || ci != c || di != d || wi != w)
						count += grid.data[ri][ci][di][wi];
				}
			}
		}
	}
	return count;
}

static std::size_t count_active(const Grid &grid) {
	std::size_t count = 0;
	for (std::size_t r = grid.y_start; r < grid.y_end; ++r) {
		for (std::size_t c = grid.x_start; c < grid.x_end; ++c) {
			for (std::size_t d = grid.z_start; d < grid.z_end; ++d) {
				for (std::size_t w = grid.w_start; w < grid.w_end; ++w)
					count += grid.data[r][c][d][w];
			}
		}
	}
	return count;
}

static std::size_t simulate(Grid grid, bool sim4d) {
	for (std::size_t i = 0; i < 6; ++i) {
		--grid.y_start;
		++grid.y_end;
		--grid.x_start;
		++grid.x_end;
		--grid.z_start;
		++grid.z_end;
		if (sim4d) {
			--grid.w_start;
			++grid.w_end;
		}
		Grid new_grid{grid};
		for (std::size_t r = grid.y_start; r < grid.y_end; ++r) {
			for (std::size_t c = grid.x_start; c < grid.x_end; ++c) {
				for (std::size_t d = grid.z_start; d < grid.z_end; ++d) {
					for (std::size_t w = grid.w_start; w < grid.w_end; ++w) {
						const auto active_neighbors = count_active_neighbors(grid, r, c, d, w);
						if (grid.data[r][c][d][w]) {
							if (!(active_neighbors == 2 || active_neighbors == 3))
								new_grid.data[r][c][d][w] = false;
						} else {
							if (active_neighbors == 3)
								new_grid.data[r][c][d][w] = true;
						}
					}
				}
			}
		}
		grid = std::move(new_grid);
	}
	return count_active(grid);
}

int main(int argc, char *argv[]) {
	auto grid = read_input();
	switch (select_part(argc, argv)) {
	case 1:
		std::cout << simulate(grid, false) << std::endl;
		break;
	case 2:
		std::cout << simulate(grid, true) << std::endl;
		break;
	}
	return 0;
}
