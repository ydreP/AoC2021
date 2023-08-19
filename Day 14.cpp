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
#include <limits>
#include <utility>
#include <cctype>
#include <regex>
using namespace std;


std::vector<std::string> readFile(std::string str) {
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
std::vector<std::string> splitString(std::string str, const std::string& delimiter) {
	std::vector<std::string> tokens;
	size_t pos = 0;

	while ((pos = str.find(delimiter)) != std::string::npos) {
		tokens.push_back(str.substr(0, pos));
		str.erase(0, pos + delimiter.length());
	}
	tokens.push_back(str); // Add the last part after the last delimiter
	return tokens;
}


map<string, long long> initalizePolymerPair(string polymer_template) {
	map<string, long long> polymer_pairs;
	for (int i = 0; i < polymer_template.size() - 1; ++i) {
		std::string sub_polymer = polymer_template.substr(i, 2);
		if (polymer_pairs.count(sub_polymer) > 0) {
			polymer_pairs[sub_polymer]++;
		}
		else {
			polymer_pairs[sub_polymer] = 1;
		}
	}
	return polymer_pairs;
}
std::map<std::string, std::string> constructInsertionRules(std::vector<std::string> raw_insert_rules) {
	//Assumes each string is of the form "XY -> Z"
	std::map<std::string, std::string> mappedRules;
	std::string delim = " -> ";
	for (const auto& raw_rule : raw_insert_rules) {
		auto split_rule = splitString(raw_rule, delim);
		mappedRules[split_rule[0]] = split_rule[1];
	}
	return mappedRules;
}
//Naive method 
std::string insertionProcess(std::string polymer_template, std::map<std::string, std::string> insert_rules) {
	std::stringstream new_polymer; // more memory efficient than a normal string.
	for (int i = 0; i < polymer_template.size() - 1; ++i) {
		std::string sub_polymer = polymer_template.substr(i, 2);
		new_polymer << polymer_template[i] << insert_rules[sub_polymer];
	}
	new_polymer << polymer_template.back();
	return new_polymer.str();
}

//Refined method, started using the "using namespace std; for clarity here
pair<map<string, long long> , map<string, long long>> 
	insertionProcess2(map<string, long long> polymer_template_pairs,
						map<string, long long> element_counter, 
						map<string, string> insertion_rules) {

	map<string, long long> updated_element_counter =element_counter;
	map<string, long long> updated_polymer_temp;

	for (const auto& polymer_freq_pair : polymer_template_pairs) {
		auto insertion = insertion_rules[polymer_freq_pair.first];
		//Each insertion yields to new pairs
		string firstPair = polymer_freq_pair.first[0] + insertion;
		string secondPair = insertion + polymer_freq_pair.first[1];
		// if the key of the updated_polymer  isn't already defined it will be created with value 0.
		//each pair is added to the new map:
		updated_polymer_temp[firstPair] = polymer_freq_pair.second+updated_polymer_temp[firstPair]; 
		updated_polymer_temp[secondPair] = polymer_freq_pair.second+updated_polymer_temp[secondPair];
		// If we have x pairs and we perform the insertion proceedure we obtain x new characters of the inserted 
		updated_element_counter[insertion] += polymer_freq_pair.second;
	}



	return { updated_polymer_temp ,  updated_element_counter };
}  


std::map<string, long long> frequencyMap(std::string polymer) {
	std::map<string, long long> freq_map;
	for (const auto& elem : polymer) {
		string elem_string(1, elem);
		if (freq_map[elem_string] > 0) {
			freq_map[elem_string]++;
		}
		else {
			freq_map[elem_string] = 1;
		}
	}
	return freq_map;
}

int main() {
	std::string filename = "input.txt";
	std::vector<std::string> data = readFile(filename);
	// Get the relevant data
	std::string polymer_temp = data[0];
	int insert_rule_start = 1; 
	std::vector<std::string> raw_insert_rules(insert_rule_start+data.begin(), data.end());
	//Format the data 
	auto insertion_rules = constructInsertionRules(raw_insert_rules);
	// Generate all the pairs of the polymer.
	auto polymer_temp_pairs = initalizePolymerPair(polymer_temp);
	//generate the frequency map counting the number of times each element appears
	auto frequency_map = frequencyMap(polymer_temp);

	// Run the insertion process for 40 steps
	int max_steps=40;
	int step = 1;
	while (step <= max_steps) {
		auto map_pair = insertionProcess2(polymer_temp_pairs, frequency_map, insertion_rules);
		polymer_temp_pairs = map_pair.first;
		frequency_map = map_pair.second;
		step++;
	}

	//Find the largest and the smallest key-value pair: 
	std::string maxKey = "";
	long long maxValue = std::numeric_limits<long long>::min(); // Start with smallest possible int value

	std::string minKey = "";
	long long minValue = std::numeric_limits<long long>::max(); // Start with largest possible int value

	for (const auto& pair : frequency_map) {
		if (pair.second > maxValue) {
			maxKey = pair.first;
			maxValue = pair.second;
		}

		if (pair.second < minValue) {
			minKey = pair.first;
			minValue = pair.second;
		}
	}

	std::cout << "Key with largest value: " << maxKey << " (" << maxValue << ")" << std::endl;
	std::cout << "Key with smallest value: " << minKey << " (" << minValue << ")" << std::endl;
	std::cout << "The difference is: " << maxValue - minValue;
	return 0;
}