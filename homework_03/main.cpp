#include <iostream>
#include <fstream>
#include <map>
#include <sstream>

#include "argparse.h"
#include "records.h"
#include "guess_the_number.h"
#include "tests.h"

const int DEFAULT_MAX_VAL = 100;


int main(int argc, const char* argv[]) {
	const std::string scores_fn = "scores_table.txt";
	bool show_table{false};
	int difficulty_level = 0;  // 0 - no level by default
	int max_value = DEFAULT_MAX_VAL;
	bool autoplay = false;
	bool run_tests = false;

	bool args_valid = ParseArgs(argc, argv, show_table, max_value,
			difficulty_level, autoplay, run_tests);
	if (!args_valid) {
		std::cout << "Exiting..." << std::endl;
	}

	if (run_tests) {
		RunTests();
	} else if (show_table) {
		ShowRecords(scores_fn);
	} else {
		RunGame(scores_fn, max_value, difficulty_level, autoplay);
	}

	return 0;
}
