#include <iostream>
#include <string> 
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>

//Reads the file, returns a vector of strings 
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
std::vector<std::string> splitString(std::string str, char delimiter) {
    std::vector<std::string> tokens;
    std::istringstream iss(str);
    std::string token;
    while (std::getline(iss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}




int main() {
    std::string filename = "input.txt";
    std::vector<std::string> data = read_file(filename);
    std::vector<std::string> split_data = splitString(data[0],',');
    int max_pos = 0;
    for (std::string& s : split_data) {
        max_pos = std::max(max_pos, std::stoi(s));
    }
    std::vector<int> crabs(max_pos + 1, 0);
    for (std::string& s : split_data) {
        int index = std::stoi(s);
        crabs[index]++;
    }
    std::vector<int> fuel_cost(max_pos + 1, 0);
    for (int i = 0; i < fuel_cost.size(); i++) {
        for (int j = 0; j < crabs.size(); j++) {
            int n = std::abs(i - j);
            fuel_cost[i] = n*(n+1) * crabs[j]/2 + fuel_cost[i]; // part one: n*crabs[j]+fuel_cost[i]
        }
    }
   
    int min_cost = INT_MAX;
    for (int& c : fuel_cost) {
        min_cost = std::min(min_cost, c);
    }


    std::cout << min_cost;
    return 0;
}
