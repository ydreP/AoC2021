#include <iostream>
#include <string> 
#include <vector>
#include <fstream>
#include <sstream>
// Propotype functions
std::vector<std::string> read_file(std::string str);
std::vector<std::string> splitString(std::string str, char delimiter);
int move(std::vector<std::string> moves);
int main() {
    std::string filename = "input.txt";
    std::vector<std::string> data = read_file(filename);
    int product=0;
    product = move(data);
    std::cout << product;
    return 0;
}
int move(std::vector<std::string> moves) {
    std::string forward = "forward";
    std::string up = "up";
    std::string down = "down";
    int xpos = 0;
    int ypos = 0;
    int aim = 0;
    for (const std::string& move : moves) {
        std::vector<std::string> command = splitString(move, ' ');
        std::string direction = command[0]; 
        int distance = 0;
        try {
            distance = std::stoi(command[1]);
        }
        catch (const std::invalid_argument& e) {
            std::cerr << "Invalid argument: " << e.what() << std::endl;
        }
        catch (const std::out_of_range& e) {
            std::cerr << "Out of range: " << e.what() << std::endl;
        }
        if (direction == forward) {
            xpos = xpos + distance;
            ypos = ypos+distance * aim;
        }
        else if (direction == up) {
            aim = aim - distance;
        }
        else if (direction == down) {
            aim = aim + distance;
        }
        else {
            std::cout << "Not a valid order!";
            return 0;
        }
    }
    return (xpos * ypos);
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
//Splits the string
std::vector<std::string> splitString(std::string str, char delimiter) {
    std::vector<std::string> tokens;
    std::istringstream iss(str);
    std::string token;
    while (std::getline(iss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

