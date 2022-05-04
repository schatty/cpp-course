#include <iostream>

#include "guess_the_number.h"


void RunTests() {
	std::string test_tmp_path = "tests_record.txt";

	// No maximum value, no level
	RunGame(test_tmp_path, 0, 0, true);

	// User defined maximum target value
	RunGame(test_tmp_path, 50, 0, true);

	// User defined levels of difficulty
	RunGame(test_tmp_path, 0, 1, true);
	RunGame(test_tmp_path, 0, 2, true);
	RunGame(test_tmp_path, 0, 3, true);

	std::remove(test_tmp_path.c_str());
};
