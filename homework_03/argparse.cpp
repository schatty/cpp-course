#include <iostream>


/**
 * Parses arguments from a command line.
 *
 * @param arc number of arguments.
 * @param argv array of arguments.
 * @param show_table flag for showing records instead of gaming.
 * @param max_val user defined maximum target value.
 * @param autoplay if true the game is played without user.
 * @param run_tests if true run tests for the game.
 * @return true if all arguments are set correctly.
 */
bool ParseArgs(int argc, const char** argv, bool& show_table, int& default_max_val,
		int& difficulty_level, bool& autoplay, bool& run_tests) {
	std::string cur_arg;
	int i{0};
	bool max_set = false, level_set = false;
	while (++i < argc) {
		cur_arg = argv[i];
		// (1) Check -table argument
		if (cur_arg.compare("-table") == 0) {
			show_table = true;

		// (2) Check -max N arguments
		} else if (cur_arg.compare("-max") == 0) {
			max_set = true;
			if (i == argc-1) {
				std::cout << "Wrong usage. The argument -max requires some value." << std::endl;
				return false;
			}
			// Read next argument and set it as new maximum value
			cur_arg = argv[++i];
			try {
				default_max_val = std::stoi(cur_arg);
			} catch (const std::invalid_argument& ia) {
				std::cerr << "Invalid argument for -max: " << ia.what() << std::endl;
				return false;
			}
			if (default_max_val <= 0) {
				std::cout << "Wrong -max value, expected value greater than 0." << std::endl;
				return false;
			}

		// (3) Check -level N arguments
		} else if (cur_arg.compare("-level") == 0) {
			level_set = true;
			if (i == argc-1) {
				std::cout << "Wrong usage. The argument -level requires some value." << std::endl;
				return false;
			}
			cur_arg = argv[++i];
			try {
				difficulty_level = std::stoi(cur_arg);
			} catch (const std::invalid_argument& ia) {
				std::cerr << "Invalid argument for -level: " << ia.what() << std::endl;
				return false;
			}
			if (difficulty_level < 1 || difficulty_level > 3) {
				std::cerr << "Invalid -level value. Exepected 1, 2 or 3." << std::endl;
			}

		// (4) Check -autoplay argument
		} else if (cur_arg.compare("-autoplay") == 0) {
			autoplay = true;
		// (5) Check -tests argument
		} else if (cur_arg.compare("-tests") == 0) {
			run_tests = true;
		}
	}

	// Check arguments compatibility
	if (max_set && level_set) {
		std::cerr << "Wrong usage, can set both -max and -level arguments." << std::endl;
		return false;
	}
	return true;
}
