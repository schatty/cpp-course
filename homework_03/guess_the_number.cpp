#include <iostream>
#include <fstream>
#include <map>
#include <sstream>

#include "records.h"

const int DEFAULT_MAX_VAL = 100;


/**
 * Runs Guess-the-number game
 *
 * @param scores_path path to the record table file.
 * @param max_val optional maximum value of target. If 0 then default value is used.
 * @param level optional difficulty constant (can be 1, 2 or 3). If 0 then no level is set.
 */
void RunGame(const std::string& scores_path, int max_val=0,
		int level=0) {
	std::cout << "Hi! Enter your name, please:" << std::endl;
	std::string user_name;
	std::cin >> user_name;

	// Upper limit setup
	if (!max_val && !level) {
		std::cout << "No upper limit is set. Using default value " << DEFAULT_MAX_VAL << std::endl;
	} else if (level) {
		switch (level) {
			case 1: max_val = 10; break;
			case 2: max_val = 50; break;
			case 3: max_val = 100; break;
		}
		std::cout << "Difficulty level: " << level << ". Upper limit: " << max_val << std::endl;
	} else {
		std::cout << "Using user defined upper limit " << max_val << std::endl;
	}

	// Target value setup
	std::srand(std::time(nullptr));
	const int target_value = std::rand() % max_val;

	// Game
	int current_value = 0;
	bool not_win = true;
	int num_guesses = 0;

	std::cout << "Enter your guess:" << std::endl;
	do {
		std::cin >> current_value;
		num_guesses++;

		if (current_value < target_value) {
			std::cout << "Target number is greater than " << current_value << std::endl;
		} else if (current_value > target_value) {
			std::cout << "Target number is less than " << current_value << std::endl;
		} else {
			std::cout << "you win!" << std::endl;
			break;
		}
	} while (true);

	UpdateRecords(scores_path, user_name, num_guesses, max_val, level);
}
