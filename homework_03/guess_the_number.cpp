#include <iostream>
#include <fstream>
#include <map>
#include <sstream>

const int DEFAULT_MAX_VAL = 100;

void update_table(const std::string& scores_path, std::string& user_name,
		int user_score, int max_val, int level);


bool parse_args(int argc, char** argv, bool& show_table, int& default_max_val,
		int& difficulty_level) {
	std::string cur_arg;
	int i{1};
	bool max_set = false, level_set = false;
	do {
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
		}

		if (i++ == argc-1) {
			break;
		}
	} while(true);

	// Check arguments compatibility
	if (max_set && level_set) {
		std::cerr << "Wrong usage, can set both -max and -level arguments." << std::endl;
		return false;
	}

	return true;
}

void start_game(const std::string& scores_path, int max_val=0,
		int level=0) {
	// Greeting
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

	update_table(scores_path, user_name, num_guesses, max_val, level);
}

void parse_record_line(std::string& line, int& score, std::string& level) {
	// line has a structure "NAME\tSCORE\tLEVEL"
	score = std::stoi(
			line.substr(line.find("\t") + 1, line.rfind("\t") - 1)
	);
	level = line.substr(line.rfind("\t") + 1, line.length() - 1);
}

void update_table(const std::string& scores_path, std::string& user_name, int score,
		int max_val, int level) {
	// Set up difficulty repr
	auto difficulty = "max-" + std::to_string(max_val);
	if (level) {
		difficulty = std::to_string(level);
	}

	std::ifstream in_file{scores_path};
	if (!in_file.is_open()) {
		std::cerr << "Failed to open scores file " << scores_path << std::endl;
	}

	std::string tmp_path = "./tmp.txt";
	std::ofstream tmp_file{tmp_path};
	if (!tmp_file.is_open()) {
		std::cout << "Failed to open file for write: " << tmp_path << std::endl;
	}

	// Create new file with updated records
	std::string line2find = user_name + "\t";
	bool replacement_done = false;
	bool user_found = false;
	std::string line;
	while (std::getline(in_file, line)) {
		if (!user_found && line.find(line2find) != std::string::npos) {
			user_found = true;

			int prev_score;
			std::string prev_difficulty;
			parse_record_line(line, prev_score, prev_difficulty);
			if (prev_score > score) {
				// Case when this user has a recrod from different difficulty
				if (prev_difficulty.compare(difficulty)) {
					std::cout << "Your stored record is set for difficulty " << 
						prev_difficulty << ". Do you want to rewrite it? [yes]" << std:: endl;
					std::string answer;
					std::cin >> answer;
					if (answer.compare("yes") != 0) {
						std::cout << "Keeping current record (DIFFICULTY: " << prev_difficulty 
							<< ", SCORE: " << prev_score << ")." << std::endl;
						tmp_file << user_name << "\t" << prev_score << "\t" << prev_difficulty << std::endl;
						replacement_done = true;
						continue;
					}
				}
				tmp_file << user_name << "\t" << score << "\t" << difficulty << std::endl;
				replacement_done = true;
				continue;
			}
			user_found = true;
		}
		tmp_file << line << std::endl;
	}

	// Add new record if the user is new
	if (!user_found) {
		std::cout << "Adding new user " << user_name << std::endl;
		tmp_file << user_name << "\t" << score << "\t" << difficulty << std::endl;
	}

	// Rename updated file to original one
	tmp_file.close();
	in_file.close();
	std::rename(tmp_path.c_str(), scores_path.c_str());
}

void show_table(const std::string& fn) {
	std::ifstream in_file{fn};
	if (!in_file.is_open()) {
		std::cout << "Failed to open file to read: " << fn << "!" << std::endl;
	}

	std::string username, difficulty;
	int user_score = 0;
	std::map<std::string, int> scores;
	std::map<std::string, std::string> difficulties;

	while (true) {
		// Read username (tab) score (tab) (difficulty)
		in_file >> username;
		in_file.ignore();
		in_file >> user_score;
		in_file.ignore();
		in_file >> difficulty;
		in_file.ignore();

		if (in_file.fail()) {
			break;
		}

		// Keep track only on minimal scores
		auto score_found = scores.find(username);
		if (score_found == scores.end() || score_found->second < user_score) {
			scores.insert(std::pair<std::string, int>(username, user_score));
			difficulties.insert(std::pair<std::string, std::string>(username, difficulty));
		}
	}

	std::cout << "RECORD TABLE" << std::endl;
	std::map<std::string, int>::iterator itr;
	std::cout << "NAME\t\tSCORE\tLEVEL" << std::endl;
	for (itr = scores.begin(); itr != scores.end(); ++itr) {
		auto difficulty = difficulties.find(itr->first)->second;
		std::cout << itr->first << "\t\t" << itr->second << "\t" << difficulty << std::endl;
	}
}

int main(int argc, char** argv) {
	const std::string scores_fn = "scores_table.txt";
	bool show_table_arg{false};
	int difficulty_level = 0;  // 0 is no level by default
	int max_value = DEFAULT_MAX_VAL;

	bool args_valid = parse_args(argc, argv, show_table_arg, max_value, difficulty_level);
	if (!args_valid) {
		std::cout << "Exiting..." << std::endl;
	}

	if (!show_table_arg) {
		start_game(scores_fn, max_value, difficulty_level);
	}
	show_table(scores_fn);

	return 0;
}
