#include <iostream>
#include <fstream>
#include <map>
#include <sstream>

#include "records.h"
#include "agent.h"

const int DEFAULT_MAX_VAL = 100;


/**
 * Get username for current play.
 *
 * @param autoplay if true then username is set to "BinarySearchAgent"
 * @returns string with username.
 */
std::string GetUsername(bool autoplay) {
	if (autoplay) {
		std::cout << "The Game is played in an auto-mode by BinarySearchAgent" << std::endl;
		return "BinaryAgent";
	}
	
	std::cout << "Hi! Enter your name, please:" << std::endl;
	std::string user_name;
	std::cin >> user_name;
	return user_name;
}

/**
 * Make guess to reach target value.
 *
 * @param autoplay if true the guess is made by binary search.
 * @returns int as guessed number.
 */
int MakeGuess(bool autoplay, BinarySearchAgent& agent) {
	int val;
	if (autoplay) {
		return agent.makeGuess();
	}

	std::cin >> val;
	return val;
}

/**
 * Run Guess-the-number game
 *
 * @param scores_path path to the record table file.
 * @param max_val optional maximum value of target. If 0 then default value is used.
 * @param level optional difficulty constant (can be 1, 2 or 3). If 0 then no level is set.
 * @param autoplay if true the game is play without user.
 */
void RunGame(const std::string& scores_path, int max_val=0,
		int level=0, bool autoplay=false) {
	std::string user_name = GetUsername(autoplay);

	// Upper limit setup
	if (!max_val && !level) {
		std::cout << "No upper limit is set. Using default value " << DEFAULT_MAX_VAL << std::endl;
		max_val = DEFAULT_MAX_VAL;
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

	// Binary Search agent. if auto is false, then is won't be used
	BinarySearchAgent agent(max_val);

	// Target value setup
	std::srand(std::time(nullptr));
	const int target_value = std::rand() % max_val;

	// Game
	int current_value = 0;
	bool not_win = true;
	int num_guesses = 0;

	std::cout << "Enter your guess:" << std::endl;
	do {
		current_value = MakeGuess(autoplay, agent);
		num_guesses++;

		if (current_value < target_value) {
			std::cout << "Target number is greater than " << current_value << std::endl;
			if (autoplay) {
				agent.acceptFeedback(1);
			}
		} else if (current_value > target_value) {
			std::cout << "Target number is less than " << current_value << std::endl;
			if (autoplay) {
				agent.acceptFeedback(-1);
			}
		} else {
			std::cout << "You win! Score: " << num_guesses << std::endl;
			break;
		}
		std::cout << std::endl;
	} while (true);

	UpdateRecords(scores_path, user_name, num_guesses, max_val, level);
	ShowRecords(scores_path);
}
