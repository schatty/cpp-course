#pragma once

#include <iostream>

void ShowRecords(const std::string& fn);

void UpdateRecords(const std::string& scores_path, std::string& user_name,
		int score, int max_val, int level);
