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


void printSet(std::set<char> s) {
    for (const char& element : s) {
        std::cout << element;
    }
    std::cout << "\n";
}
class SevenSegmentDisplay {
public: 
    std::vector< std::set<char> > segments;
    std::vector< std::set<char> > output;
    std::map< std::set<char>, int> types; 
    std::map< int, std::set<char> > types_reversed;

    SevenSegmentDisplay(std::vector< std::set<char> > sequence, std::vector< std::set<char> > outputs) {
        segments = sequence;
        output = outputs;
        for (auto& segment : sequence) {
            types.insert({ segment,-1 });
        }
    }
    void determine_types() {
        //bug here. we identify 6's as 0's and 3's as 5's.
        for (const std::set<char>& segment : segments) {
            /* If size is 2, then it corresponds to a 1. 
            * If size is 3, then it corresponds to a 7.
            * If size is 4, then it corresponds to a 4.
            * If size is 7, then it corresponds to a 8.
            */
            if (segment.size() == 2) { types[segment] = 1; types_reversed[1] = segment; }
            if (segment.size() == 3) { types[segment] = 7; types_reversed[7] = segment; }
            if (segment.size() == 4) { types[segment] = 4; types_reversed[4] = segment; }
            if (segment.size() == 7) { types[segment] = 8; types_reversed[8] = segment; }
        }
        auto one = types_reversed[1];
        //std::cout << "1 is given by: \n";
        //printSet(one);
        auto four = types_reversed[4];
        //std::cout << "4 is given by: \n";
        //printSet(four);
        auto seven = types_reversed[7];
        //std::cout << "7 is given by: \n";
        //printSet(seven);
        auto eight = types_reversed[8];
        //std::cout << "8 is given by: \n";
        //printSet(eight);
        /* 9,6,0 are determined by the fact that |set(0)|,|set(6)|,|set(9)|=6
        * and set(8)-set(0), contains an element of set(4), set(8)-set(6) contains an element of set(1)
        * and set(8)-set(9) doesn't contain one from either of those.
        */
        for (const std::set<char>& segment : segments) {
            if (segment.size() == 6) {
                std::set<char> set_diff;
                std::set_difference(eight.begin(), eight.end(), segment.begin(), segment.end(), std::inserter(set_diff, set_diff.begin()));
                //std::cout << "We now look at segment: "; 
                //printSet(segment);
                //std::cout << "The set difference between set(8) and segment is: \n";
                //printSet(set_diff);
                for (const char& element : set_diff) {
                    //if the remaining element is in set(4) then segment corresponds to 0
                    std::set<char> fourMinusOne;
                    std::set_difference(four.begin(), four.end(), one.begin(), one.end(), std::inserter(fourMinusOne, fourMinusOne.begin()));
                    if (fourMinusOne.count(element) > 0) {
                        // here is the problem when segment = set(6) since they contain the same segment.
                        //std::cout << "The type is therefore 0! \n";
                        types[segment] = 0;
                        types_reversed[0] = segment;
                    }
                    //if the remaining element is in set(1) then the segment corresponds to 6 
                    else if (one.count(element) > 0) {
                        //std::cout << "The type is therefore 6! \n";
                        types[segment] = 6;
                        types_reversed[6] = segment;
                    }
                    //otherwise the element corresponds to 9
                    else {
                        //std::cout << "The type is therefore 9! \n";
                        types[segment] = 9;
                        types_reversed[9] = segment;
                    }
                }

            }

        }
        auto nine = types_reversed[9];
        auto zero = types_reversed[0];
        auto six = types_reversed[6];
        /* Finally we determine 2,3,5 by the fact that |set(x)|=5, for x=2,3,5 and the following:
        * 5 is uniquely determined by set(6)-set(5) contains only one element
        * 3 is uniquely determined by set(9)-set(3) contains only one element
        */
        for (const std::set<char>& segment : segments) {
            if (segment.size() == 5) {
                std::set<char> set_diff;
                std::set_difference(six.begin(), six.end(), segment.begin(), segment.end(), std::inserter(set_diff, set_diff.begin()));
                if (set_diff.size() == 1) {
                    types[segment] = 5;
                    types_reversed[5] = segment;
                }
                else {
                    std::set<char> set_diff;
                    std::set_difference(nine.begin(), nine.end(), segment.begin(), segment.end(), std::inserter(set_diff, set_diff.begin()));
                    if (set_diff.size() == 1) {
                        types[segment] = 3;
                        types_reversed[3] = segment;
                    }
                    else {
                        types[segment] = 2;
                        types_reversed[2] = segment;
                    }
                }
            }
        }
    }
    int number_of_1478() {
        int nbr = 0;
        for (std::set<char>& segment : output) {
            auto it = types.find(segment);
            if (it != types.end() && it->second >= 0) {
                nbr++;
            }
        }
        return nbr;
    }
    int determine_output() {
        std::string output_val;
        int out = 0;
        for (size_t i = 0; i < output.size(); i++) {
            out = out + std::pow(10, 4 - i)* types[output[i]];
        }
        return out;
    }
};
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
std::set<char> stringToSet(std::string str) {
    std::set<char> charSet;
    for (char& c : str) {
        charSet.insert(c);
    }
    return charSet;
}


int main() {
    std::string filename = "input.txt";
    std::vector<std::string> data = read_file(filename);
    std::vector<SevenSegmentDisplay> displays;
    for (std::string& line : data) {
        auto split_string = splitString(line, '|');
        std::string input = split_string[0];
        std::string output = split_string[1];
        auto segments = splitString(input, ' '); // returns a vector of strings
        auto output_segments = splitString(output, ' ');
        std::vector< std::set<char> > sequence;
        for (std::string& segment : segments) {
            auto setSeg = stringToSet(segment);
            sequence.push_back(setSeg);
        }
        std::vector< std::set<char> > outputs;
        for (std::string& str : output_segments) {
            auto setSeg = stringToSet(str);
            outputs.push_back(setSeg);
        }
        SevenSegmentDisplay display(sequence,outputs);
        displays.push_back(display);
    }
    int total = 0;
    int sum_of_output = 0;
    for (auto& display : displays) {
        display.determine_types();
        total = display.number_of_1478() + total;
        sum_of_output = sum_of_output + display.determine_output();
        std::cout << "Output is: " << display.determine_output();
        std::cout << "\n";
    }
    std::cout << "Total output is: " << sum_of_output;
    return 0;
}
