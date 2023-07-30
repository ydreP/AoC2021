#include <iostream>
#include <string> 
#include <vector>
#include <fstream>
#include <sstream>

// Propotype functions
std::vector<std::string> read_file(std::string str);
std::vector<std::string> splitString(std::string str, char delimiter);

int main() {
    auto test = "3,4,3,1,2";
    std::string filename = "input.txt";
    std::vector<std::string> data = read_file(filename);
    std::vector<std::string> inital_value = splitString(data[0], ',');
    //std::vector<std::string> inital_value = splitString(test, ',');
    std::vector<long long> cycle(9,0);
    for (std::string& s : inital_value) { 
        int index = std::stoi(s);
        cycle[index] = cycle[index] + 1;
    }
    int day = 1;
    int last_day = 256;

    while (day <= last_day) {
        std::vector<long long> next_day(9, 0);
        for (int i = cycle.size()-1; i >= 0; i--) {
            if (i == 0) {
                next_day[6] = next_day[6] + cycle[0];
                next_day[8] = cycle[0];
            }
            else {
                next_day[i - 1] = cycle[i];
            }
        }
        cycle = next_day;
        day++;
    }
    long long nbrOfFish = 0;
    for (long long& v : cycle) {
        std::cout << v << "\n";
        nbrOfFish = nbrOfFish + v;
    }
    std::cout << nbrOfFish;
    return 0;
}
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
