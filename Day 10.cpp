#include <iostream>
#include <string> 
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>
#include <set>
#include <map>
#include <algorithm>
#include <climits>
#include <stack>

std::vector<std::string> read_file(std::string str) {
	std::vector< std::string > input;
	std::ifstream inputFile(str);
	if (!inputFile.is_open()) {
		// Handle the error if the file couldn't be opened
		std::cerr << "Error opening the file." << std::endl;
		return input;
	}
	std::string line;
	while (std::getline(inputFile, line)) {
		if (line != "") {
			// Process the line here
			input.push_back(line);
		}
	}
	inputFile.close();
	return input;
}
bool isValidPair(char left, char right) {
	return (left == '(' && right == ')')
		|| (left == '[' && right == ']')
		|| (left == '{' && right == '}')
		|| (left == '<' && right == '>');
}
//Checks if a line is corrupted. I assume that the input is correctly formatted.
bool isCorrupted(const std::string line) {
	std::set<char> openers = { '(', '[', '{', '<' };
	std::set<char> closers = { ')', ']', '}', '>' };
	std::stack<char> stackOfOpeners; 
	for (const char& c : line) {
		if (openers.count(c)) {
			stackOfOpeners.push(c);
		}
		else {
			if (!stackOfOpeners.empty()) {
				char left = stackOfOpeners.top();
				stackOfOpeners.pop();
				if (!isValidPair(left, c)) {
					//std::cout << "Invalid closing: " << left << c;
					return true;
				}
			} else {
				return false;
			}
		}
	}
	return false;
}

char firstCorruptedMatch(const std::string line) {
	std::set<char> openers = { '(', '[', '{', '<' };
	std::set<char> closers = { ')', ']', '}', '>' };
	std::stack<char> stackOfOpeners;
	for (const char& c : line) {
		if (openers.count(c)) {
			stackOfOpeners.push(c);
		}
		else {
			if (!stackOfOpeners.empty()) {
				char left = stackOfOpeners.top();
				stackOfOpeners.pop();
				if (!isValidPair(left, c)) {
					//std::cout << "Invalid closing: " << left << c << "\n";
					return c;
				}
			}
			else {
				return '-';
			}
		}
	}
	return '-';
}

std::stack<char>  returnIncomplete(const std::string line) {
	std::set<char> openers = { '(', '[', '{', '<' };
	std::set<char> closers = { ')', ']', '}', '>' };
	std::stack<char> stackOfOpeners;
	for (const char& c : line) {
		if (openers.count(c)) {
			stackOfOpeners.push(c);
		}
		else {
			if (!stackOfOpeners.empty()) {
				char left = stackOfOpeners.top();
				if (isValidPair(left, c)) {
					stackOfOpeners.pop();
				}
			}
		}
	}
	return stackOfOpeners;
}

std::string returnCompletion(std::stack<char> incomplete) {
	std::map<char, char> reverse;
	reverse['('] = ')', reverse['['] = ']', reverse['{'] = '}', reverse['<'] = '>';
	std::string completion;
	while (!incomplete.empty()) {
		char elem = incomplete.top();
		completion += reverse[elem];
		incomplete.pop();
	}
	return completion;
}


long long compute_completion_score(std::string completion) {
	long long score = 0;
	std::map<char, int> points;
	int increment = 5;
	points[')'] = 1, points[']'] = 2, points['}'] = 3, points['>'] = 4;
	for (const char& c : completion) {
		score *= increment;
		score += points[c];
	}
	return score;
}

int main() {
	std::string filename = "input.txt";
	std::vector<std::string> data = read_file(filename);
	std::map<char, int> score; 
	score[')'] = 3;
	score[']'] = 57;
	score['}'] = 1197; 
	score['>'] = 25137;
	int total_score = 0;
	std::vector<std::stack<char>> toBeCompleted;
	for (std::string& line : data) {
		if (isCorrupted(line)) {
			total_score += score[firstCorruptedMatch(line)];
		}
		else {
			toBeCompleted.push_back(returnIncomplete(line));
		}
	}
	//std::cout << total_score<<"\n";
	std::vector<long long> completion_score;
	for (std::stack<char>& stck : toBeCompleted) {
		auto tempstack = std::move(stck);
		std::string completion = returnCompletion(tempstack);
		long long score = compute_completion_score(completion);
		std::cout << "Completed : " << completion << " Score: " << score << "\n";
		completion_score.push_back(compute_completion_score(completion));
	}
	std::sort(completion_score.begin(), completion_score.end());
	int midpoint = completion_score.size() / 2;
	std::cout << completion_score[midpoint];


	return 0;
}
