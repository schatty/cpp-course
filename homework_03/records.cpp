#include <iostream>
#include <fstream>
#include <map>

#include "agent.h"

/**
 * Parse single line from records file.
 *
 * @param line input line from records.
 * @param score user score.
 * @param level recorded difficulty.
 */
void ParseRecordLine(std::string& line, int& score, std::string& level) {
	// line has a structure "NAME\tSCORE\tLEVEL"
	score = std::stoi(
			line.substr(line.find("\t") + 1, line.rfind("\t") - 1)
	);
	level = line.substr(line.rfind("\t") + 1, line.length() - 1);
}

/**
 * Update record table after completed game.
 *
 * @param scores_path path to the records file.
 * @param user_name username
 * @param max_val optional user defined maximum of target value.
 * @param level optional user defined difficulty level.
 */
void UpdateRecords(const std::string& scores_path, std::string& user_name, int score,
		int max_val, int level) {
	// Set up difficulty naming
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
			ParseRecordLine(line, prev_score, prev_difficulty);
			if (prev_score > score) {
				// Case when this user has a recrod from different difficulty
				if (prev_difficulty.compare(difficulty)) {
					std::cout << "Your stored record is set for difficulty " <<
						prev_difficulty << ". Do you want to rewrite it? [yes]" << std:: endl;
					std::string answer;
					if (user_name == "BinaryAgent") {
						answer = "yes";
					} else {
						std::cin >> answer;
					}
					if (answer.compare("yes") != 0) {
						std::cout << "Keeping current record (DIFFICULTY: " << prev_difficulty
							<< ", SCORE: " << prev_score << ")." << std::endl;
						tmp_file << user_name << "\t" << prev_score << "\t" << prev_difficulty << std::endl;
						replacement_done = true;
						continue;
					}
				}
				std::cout << "Recording new record: " << difficulty << std::endl;
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

std::string spacing(const std::string& s) {
	if (s.length() < 6) {
		return "\t\t";
	}
	return "\t";
}

/**
 * Display records in stdout.
 *
 * @param path path to the records file.
 */
void ShowRecords(const std::string& path) {
	std::ifstream in_file{path};
	if (!in_file.is_open()) {
		std::cout << "Failed to open file to read: " << path << "!" << std::endl;
	}

	std::string username, difficulty;
	int user_score = 0;
	std::map<std::string, int> scores;
	std::map<std::string, std::string> difficulties;

	while (true) {
		in_file >> username;		// username
		in_file.ignore();			// \t
		in_file >> user_score;		// score
		in_file.ignore();			// \t
		in_file >> difficulty;		// difficulty
		in_file.ignore();			// \t

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

	// Header
	std::cout << std::string(55, '-') << std::endl;
	std::cout << "\tNAME\t\tSCORE\t\tLEVEL" << std::endl;
	std::cout << std::string(55, '-') << std::endl;

	// Records
	std::map<std::string, int>::iterator itr;
	for (itr = scores.begin(); itr != scores.end(); ++itr) {
		auto difficulty = difficulties.find(itr->first)->second;
		std::cout << "\t" << itr->first << spacing(itr->first) << itr->second << "\t\t" << difficulty << std::endl;
	}
	std::cout << std::string(55, '-') << std::endl << std::endl;
}
