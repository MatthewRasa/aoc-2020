#include <iostream>
#include <vector>
#include <cstring>

static std::vector<std::vector<char>> read_input() {
	std::vector<std::vector<char>> input;
	for (std::string line; std::getline(std::cin, line); )
		input.push_back(std::vector<char>(line.begin(), line.end()));
	return input;
}

static std::size_t traverse_map(const std::vector<std::vector<char>> &map, std::size_t x_slope, std::size_t y_slope) {
	const std::size_t ncols = map[0].size();
	std::size_t count = 0;
	for (uint r = 0, c = 0; r < map.size(); r += y_slope, c = (c + x_slope) % ncols) {
		if (map[r][c] == '#')
			++count;
	}
	return count;
}

int main(int argc, char *argv[]) {
	if (argc < 2) {
		std::cerr << "Usage: " << argv[0] << " <1|2>" << std::endl;
		return 1;
	}

	bool part1;
	if (strncmp(argv[1], "1", 1) == 0) {
		part1 = true;
	} else if (strncmp(argv[1], "2", 1) == 0) {
		part1 = false;
	} else {
		std::cerr << argv[0] << ": invalid argument '" << argv[1] << "'" << std::endl;
		return 1;
	}

	const auto input = read_input();
	if (part1) {
		std::cout << traverse_map(input, 3, 1) << std::endl;
	} else {
		std::cout << (
			traverse_map(input, 1, 1)
			* traverse_map(input, 3, 1)
			* traverse_map(input, 5, 1)
			* traverse_map(input, 7, 1)
			* traverse_map(input, 1, 2)
		) << std::endl;
	}
}
