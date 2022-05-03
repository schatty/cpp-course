#include <iostream>
#include <fstream>

int main() {
	const std::string high_scores_filename = "high_scores.txt";

	// Greeting
	std::cout << "Hi! Enter your name, please:" << std::endl;
	std::string user_name;
	std::cin >> user_name;

	// Target value setup
	const int max_value = 100;
	std::srand(std::time(nullptr));
	const int target_value = std::rand() % 100;

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

	// Write new score to the table
	{
		std::ofstream out_file{high_scores_filename, std::ios_base::app};
		if (!out_file.is_open()) {
			std::cout << "Failed to open file for write: " << high_scores_filename << "!" << std::endl;
			return -1;
		}

		// Append new results to the table
		out_file << user_name << ' ';
		out_file << num_guesses << std::endl;
	}

	// Read scores and print all the results
	{
		std::ifstream in_file{high_scores_filename};
		if (!in_file.is_open()) {
			std::cout << "Failed to open file to read: " << high_scores_filename << "!" << std::endl;
			return -1;
		}

		std::cout << "High scores table:" << std::endl;

		std::string username;
		int high_score = 0;
		while (true) {
			// Read the username
			in_file >> username;
			// Read the score
			in_file >> high_score;
			in_file.ignore();

			if (in_file.fail()) {
				break;
			}

			std::cout << username << '\t' << high_score << std::endl;
		}
	}

	return 0;
}
