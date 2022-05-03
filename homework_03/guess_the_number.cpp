#include <iostream>
#include <fstream>
#include <map>
#include <sstream>

const int DEFAULT_MAX_VAL = 100;

void update_table(const std::string& scores_path, std::string& user_name, int user_score);
std::string file2String(const std::string& path);


bool parse_args(int argc, char** argv, bool& show_table, int& default_max_val) {
	std::string cur_arg;
	int i{1};
	do {
		cur_arg = argv[i];
		// (1) Check -table argument
		if (cur_arg.compare("-table") == 0) {
			show_table = true;
		// (2) Check -max N arguments
		} else if (cur_arg.compare("-max") == 0) {
			if (i == argc-1) {
				std::cout << "Wrong usage. The argument -max requires some value." << std::endl;
				break;
			}
			// Read next argument and set it as new maximum value
			cur_arg = argv[++i];
			try {
				default_max_val = std::stoi(cur_arg);
				if (default_max_val <= 0) {
					std::cout << "Wrong -max value, expected value greater than 0." << std::endl;
					return false;
				}
				std::cout << "Using user defined upper limit " << default_max_val << std::endl;
			} catch (const std::invalid_argument& ia) {
				std::cerr << "Invalid argument for -max: " << ia.what() << std::endl;
				return false;
			}
		}

		if (i++ == argc-1) {
			break;
		}
	} while(true);

	return true;
}

void start_game(const std::string& scores_path, int max_val=0) {
	// Greeting
	std::cout << "Hi! Enter your name, please:" << std::endl;
	std::string user_name;
	std::cin >> user_name;

	// Upper limit setup
	if (!max_val) {
		std::cout << "No upper limit is set. Using default value " << DEFAULT_MAX_VAL << std::endl;
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

	update_table(scores_path, user_name, num_guesses);
}

void update_table(const std::string& scores_path, std::string& user_name, int user_score) {
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
	std::string line;
	while (std::getline(in_file, line)) {
		if (!replacement_done && line.find(line2find) != std::string::npos) {
			int prev_score = std::stoi(
				line.substr(line.find("\t") + 1, line.length() - 1)
			);
			if (prev_score > user_score) {
				tmp_file << user_name << "\t" << user_score << std::endl;
				replacement_done = true;
				continue;
			}
		}
		tmp_file << line << std::endl;
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

	std::string username;
	int high_score = 0;
	std::map<std::string, int> scores;

	while (true) {
		// Read the username
		in_file >> username;
		// Read the score
		in_file >> high_score;
		in_file.ignore();

		if (in_file.fail()) {
			break;
		}

		// Keep track only on minimal scores
		auto score_found = scores.find(username);
		if (score_found == scores.end() || score_found->second < high_score) {
			scores.insert(std::pair<std::string, int>(username, high_score));
		}
	}

	std::cout << "High scores table:" << std::endl;
	std::map<std::string, int>::iterator itr;
	for (itr = scores.begin(); itr != scores.end(); ++itr) {
		std::cout << itr->first << '\t' << itr->second << std::endl;
	}
}

int main(int argc, char** argv) {
	const std::string scores_fn = "high_scores.txt";
	bool show_table_arg{false};
	int max_value = DEFAULT_MAX_VAL;

	bool args_valid = parse_args(argc, argv, show_table_arg, max_value);
	if (!args_valid) {
		std::cout << "Exiting..." << std::endl;
	}

	if (!show_table_arg) {
		start_game(scores_fn, max_value);
	}
	show_table(scores_fn);

	return 0;
}
