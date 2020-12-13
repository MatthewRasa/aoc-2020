#include "common.h"
#include <functional>
#include <iostream>
#include <string>
#include <vector>

using Grid = std::vector<std::vector<char>>;

static Grid read_input() {
	Grid grid;
	for (std::string line; std::getline(std::cin, line); )
		grid.emplace_back(line.begin(), line.end());
	return grid;
}

static std::size_t count_adj_occupied1(const Grid &grid, long r, long c) {
	const long nrows = grid.size();
	const long ncols = grid[0].size();
	std::size_t count = 0;
	for (auto ri = std::max(r - 1, 0l); ri <= std::min(r + 1, nrows - 1); ++ri)
		for (auto ci = std::max(c - 1, 0l); ci <= std::min(c + 1, ncols - 1); ++ci)
			count += (ri != r || ci != c) && grid[ri][ci] == '#';
	return count;
}

static std::size_t count_adj_occupied2(const Grid &grid, long r, long c) {
	const long nrows = grid.size();
	const long ncols = grid[0].size();
	std::size_t count = 0;
	for (auto rd = -1; rd <= 1; ++rd) {
		for (auto cd = -1; cd <= 1; ++cd) {
			if (rd != 0 || cd != 0) {
				for (auto ri = r + rd, ci = c + cd; 0 <= ri && ri < nrows && 0 <= ci && ci < ncols; ri += rd, ci += cd) {
					if (grid[ri][ci] == '#') {
						++count;
						break;
					} else if (grid[ri][ci] == 'L') {
						break;
					}
				}
			}
		}
	}
	return count;
}

static std::size_t count_total_occupied(const Grid &grid) {
	std::size_t count = 0;
	for (const auto &row : grid)
		for (auto seat : row)
			count += seat == '#';
	return count;
}

static std::size_t simulate(Grid grid, std::size_t occupied_max,
		std::function<std::size_t(const Grid &, long, long)> count_adj_occupied_func) {
	const auto ncols = grid[0].size();
	std::size_t changes;
	do {
		changes = 0;
		auto new_grid = grid;
		for (std::size_t r = 0; r < grid.size(); ++r) {
			for (std::size_t c = 0; c < ncols; ++c) {
				if (grid[r][c] == 'L') {
					if (count_adj_occupied_func(grid, r, c) == 0) {
						new_grid[r][c] = '#';
						++changes;
					}
				} else if (grid[r][c] == '#') {
					if (count_adj_occupied_func(grid, r, c) >= occupied_max) {
						new_grid[r][c] = 'L';
						++changes;
					}
				}
			}
		}
		grid = std::move(new_grid);
	} while (changes != 0);
	return count_total_occupied(grid);
}

int main(int argc, char *argv[]) {
	auto grid = read_input();
	switch (select_part(argc, argv)) {
	case 1:
		std::cout << simulate(grid, 4, count_adj_occupied1) << std::endl;
		break;
	case 2:
		std::cout << simulate(grid, 5, count_adj_occupied2) << std::endl;
		break;
	}
	return 0;
}
