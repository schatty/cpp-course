#pragma once

#include <iostream>



class BinarySearchAgent {
	private:
		int _upper;
		int _lower = 0;
		int _guess;

	public:
		BinarySearchAgent(int upper_limit) {
			_upper = upper_limit;
		}

		int makeGuess() {	
			_guess = _lower + (_upper - _lower) / 2;
			std::cout << "BinaryAgent's guess: " << _guess << std::endl;
			return _guess;
		}

		void acceptFeedback(int sign) {
			if (sign < 0) {
				_upper = _guess;
			} else if (sign > 0) {
				_lower = _guess;
			}
		}
};
