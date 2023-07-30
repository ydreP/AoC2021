#include <iostream>
#include <string> // Include the <string> header for std::string
#include <fstream>
#include <vector>
// Propotype functions
std::vector<int> read_file(std::string str);
int number_of_increases(std::vector<int> vec);
int rolling_average_increase(std::vector<int> vec);
int main() {
    std::string filename = "input.txt";
    std::vector<int> data= read_file(filename);
    int n = rolling_average_increase(data);
    std::cout << n;
    return 0;
}
//Reads the file, returns a vector of ints 
std::vector<int> read_file(std::string str) {
    std::vector<int> input; 
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
            try {
                input.push_back(std::stoi(line));
            }
            catch (const std::invalid_argument& e) {
                std::cerr << "Invalid argument: " << e.what() << std::endl;
            }
            catch (const std::out_of_range& e) {
                std::cerr << "Out of range: " << e.what() << std::endl;
            }
        }
    }
    inputFile.close();
    return input;
}
// Computes the number of positive increments
int number_of_increases(std::vector<int> vec) {
    int previous_value = 0;
    int number_of_increases = 0;
    for (const int& num : vec) {
        if (previous_value < num) {
            number_of_increases++;
        }
        previous_value = num;
    }
    return number_of_increases-1;
}
int rolling_average_increase(std::vector<int> vec) {
    int previous_value = 0;
    int number_of_increases = 0;
    int windows_size = 3;
    for (size_t i = 0; i < vec.size() - windows_size; ++i) {
        int roll_avg = 0;
        for (size_t j = i; j < i + windows_size; ++j) {
            roll_avg = roll_avg + vec[j];
        }
        std::cout << roll_avg;
        std::cout << "\n";
        if (roll_avg > previous_value) {
            number_of_increases++;
        }
        previous_value = roll_avg;
    }
    return number_of_increases ;
}