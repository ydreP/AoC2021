#include <iostream>
#include <string> 
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>

// Propotype functions
std::vector<std::string> read_file(std::string str);
std::vector<std::string> splitString(std::string str, char delimiter);
std::vector<int> stringToBinaryArray(std::string str);
int binaryToDecimal(std::vector<int> binArray);
int compute_power(std::vector<std::string> vec);
std::vector<int> compute_ones(std::vector<std::string> vec);
std::vector<std::string> find_oxy_bit(std::vector<std::string> vec, int freq_ind);
std::vector<std::string> find_co2_bit(std::vector<std::string> vec, int freq_ind);

int main() {
    std::string filename = "input.txt";
    std::vector<std::string> data = read_file(filename);
    //std::cout << compute_power(data);
    std::vector<std::string> oxy_bit_vec = find_oxy_bit(data,0);
    std::vector<std::string> co2_bit_vec = find_co2_bit(data, 0);
    std::string oxy_bit = oxy_bit_vec[0];
    std::string co2_bit = co2_bit_vec[0];
    std::cout << oxy_bit << "\n";
    std::cout << co2_bit << "\n";
    std::cout << binaryToDecimal(stringToBinaryArray(oxy_bit)) * binaryToDecimal(stringToBinaryArray(co2_bit));
    return 0;
}
std::vector<std::string> find_co2_bit(std::vector<std::string> vec, int freq_ind) {
    int number_of_bits = vec.size();
    if (vec.size() == 1) {
        return vec;
    }
    else {
        std::vector<int> nbr_of_ones = compute_ones(vec);
        std::vector<int> least_frequent(nbr_of_ones.size());
        for (size_t i = 0; i < nbr_of_ones.size(); i++) {
            least_frequent[i] = static_cast<int>(!( 2 * nbr_of_ones[i] - number_of_bits >= 0) );
        }
        std::vector<std::string> new_bits;
        for (const std::string& str : vec) {
            if ((int(str[freq_ind]) - 48) == least_frequent[freq_ind]) {
                new_bits.push_back(str);
            }
        }
        return find_co2_bit(new_bits, freq_ind + 1);
    }
}



std::vector<std::string> find_oxy_bit(std::vector<std::string> vec, int freq_ind) {
    int number_of_bits = vec.size();
    if (vec.size() == 1) {
        return vec;
    }
    else {
        std::vector<int> nbr_of_ones = compute_ones(vec);
        std::vector<int> most_frequent(nbr_of_ones.size());

        for (size_t i = 0; i < nbr_of_ones.size(); i++) {
            most_frequent[i] = static_cast<int>( 2*nbr_of_ones[i]-number_of_bits >= 0);
        }
        std::vector<std::string> new_bits;
        for (const std::string& str : vec) {
            if ( (int(str[freq_ind])-48) ==  most_frequent[freq_ind] ) {
                new_bits.push_back(str);
            }
        }
        return find_oxy_bit(new_bits,freq_ind+1);
    }
}


int compute_power(std::vector<std::string> vec) {
    std::vector<int> number_of_ones = compute_ones(vec);
    std::vector<int> most_frequent(number_of_ones);
    std::vector<int> least_frequent(number_of_ones.size());
    int number_of_bits = vec.size();
    for (size_t i = 0; i < number_of_ones.size(); i++) {
        least_frequent[i] = static_cast<int>(most_frequent[i] != 1);
    }
    for (size_t i = 0; i < number_of_ones.size(); ++i) {
        most_frequent[i] = static_cast<int>(number_of_ones[i] > number_of_bits / 2);
        least_frequent[i] = static_cast<int>(!(number_of_ones[i] > number_of_bits / 2));
    }
    return binaryToDecimal(most_frequent)*binaryToDecimal(least_frequent);
}
std::vector<int> compute_ones(std::vector<std::string> vec) {
    std::vector<int> number_of_ones(vec[1].size());
    std::vector<int> most_frequent(vec[1].size());
    std::vector<int> least_frequent(vec[1].size());
    for (const std::string& str : vec) {
        std::vector<int> bits;
        bits = stringToBinaryArray(str);
        for (size_t i = 0; i < vec[1].size(); ++i ) {
            number_of_ones[i] = number_of_ones[i] + bits[i];
        }
    }
    return number_of_ones;
}
int binaryToDecimal(std::vector<int> binArray) {
    int dec = 0;
    int len = binArray.size();
    for (int i = 0; i < len; ++i) {
        dec = dec + std::pow(2 , (len - i-1)) * binArray[i];
    }
    return dec;
}
std::vector<int> stringToBinaryArray(std::string str) {
    std::vector<int> binArr;
    for (char ch : str) {
        if (ch == '0') {
            binArr.push_back(0);
        }
        else if (ch == '1') {
            binArr.push_back(1);
        }
        else {
            std::cerr << "Error: Invalid character found in the binary string." << std::endl;
            return {}; // Return an empty vector to indicate an error.
        }
    }
    return binArr;
}
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
