#include <iostream>

#include "guess_the_number.h"
#include "argparse.h"

/**
 * @returns number of failed tests.
 */
int ParseArgsTests() {
	int n_failed = 0;
	// Dummy variables for tests
	int _test_int = 100;
	bool _test_bool = true;

	// Correct cases

	const char* argv1[1] = {"path"};
	n_failed += !ParseArgs(1, argv1, _test_bool, _test_int, _test_int, _test_bool, _test_bool);

	const char* argv2[2] {"path", "-table"};
	n_failed += !ParseArgs(2, argv2, _test_bool, _test_int, _test_int, _test_bool, _test_bool);

	const char* argv3[2] {"path", "-autogame"};
	n_failed += !ParseArgs(2, argv3, _test_bool, _test_int, _test_int, _test_bool, _test_bool);

	const char* argv4[3] {"path", "-max", "100"};
	n_failed += !ParseArgs(3, argv4, _test_bool, _test_int, _test_int, _test_bool, _test_bool);

	const char* argv5[3] {"path", "-level", "1"};
	n_failed += !ParseArgs(3, argv5, _test_bool, _test_int, _test_int, _test_bool, _test_bool);

	// Incorrect cases

	const char* argv6[2] {"path", "-level"};
	n_failed += ParseArgs(2, argv6, _test_bool, _test_int, _test_int, _test_bool, _test_bool);

	const char* argv7[2] {"path", "-max"};
	n_failed += ParseArgs(2, argv7, _test_bool, _test_int, _test_int, _test_bool, _test_bool);

	return n_failed;
}

/**
 * @returns number of failed tests.
 */
int RunGameTests() {
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

	return 0;
}

void RunTests() {
	int n_failed = 0;

	n_failed += ParseArgsTests();
	n_failed += RunGameTests();

	if (n_failed == 0) {
		std::cout << "All tests PASSED." << std::endl;
	} else {
		std::cout << "Number of failed tests: " << n_failed << std::endl;
	}
};
