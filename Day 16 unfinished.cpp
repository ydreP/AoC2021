#include <iostream>
#include <string> 
#include <vector>
#include <fstream>
#include <map>
#include <bitset>
#include <cmath>

std::map<char, std::string> hex_to_binary = {
    {'0', "0000"},
    {'1', "0001"},
    {'2', "0010"},
    {'3', "0011"},
    {'4', "0100"},
    {'5', "0101"},
    {'6', "0110"},
    {'7', "0111"},
    {'8', "1000"},
    {'9', "1001"},
    {'A', "1010"},
    {'B', "1011"},
    {'C', "1100"},
    {'D', "1101"},
    {'E', "1110"},
    {'F', "1111"}
};

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

std::string convertToBitString( std::string hexa_string) {
	std::string bit_string; 
	for (const char& hexa_char : hexa_string) {
		bit_string += hex_to_binary[hexa_char];
	}
	return bit_string;
}
int convertBinaryStringToInt(std::string bit_string) {
	int converted_value = 0;
	int size = bit_string.size();
	for (int i = 0; i < size; i++) {
		if (bit_string[0] != '1' || bit_string[i] != '0') {
			std::cout << "Incorrectly formated string!" << std::endl;
			break;
		}
		else {
			converted_value += static_cast<int>(std::pow(2, i + size - 1)) * (bit_string[i] - '0');
		}
	}
	return converted_value;
}
/*Computes the lengt of the packet 
* current_position points to the first position of the packet
*/
std::pair<int,int> computePacket(std::string& bit_string, int current_position, bool is_sub_packet ) {

	//Important every packet must be a multiple of 4!

	int length = 0;
	int value = 0;
	std::string version_value;
	std::string type_id;
	std::pair <int, int> value_length_pair;
	version_value = bit_string.substr(current_position, 3);
	type_id = bit_string.substr(current_position + 3, 3);
	// Move 
	std::string type_4 = "100";
	value = convertBinaryStringToInt(version_value);
	value_length_pair.first = value; 
	
	/*
	To do this, the binary number is padded with leading zeroes until its length is a multiple of four bits,
	and then it is broken into groups of four bits.
	Each group is prefixed by a 1 bit except the last group, which is prefixed by a 0 bit.
	These groups of five bits immediately follow the packet header. */
	if (type_id == type_4) {
		char current_bit = bit_string[current_position];
		std::string bit_literal_value; //this might come in handy later on. 
		while (current_bit != '0') {
			//maybe read the value of the literal value here at some point? 
			current_position += 5;
			current_bit = bit_string[current_position];
		}
		int length_without_padding = current_position + 5;
		if (!is_sub_packet) {
			length = (length_without_padding / 4 + 1) * 4;
		}
		value_length_pair.second = length; 
		return value_length_pair;
	}
	else {
		/*An operator packet contains one or more packets. To indicate which subsequent binary data represents its sub-packets, 
		* an operator packet can use one of two modes indicated by the bit immediately after the packet header; this is called the length type ID: 
		* If the length type ID is 0, then the next 15 bits are a number that represents the total length in bits of the sub-packets contained by this packet. 
		*If the length type ID is 1, then the next 11 bits are a number that represents the number of sub-packets immediately contained by this packet. */
		// The length type ID is 6 positions after the version number and the type ID 
		int length_type_position = current_position + 6;
		char length_type = bit_string[length_type_position];
		if (length_type == '0') {
			std::string total_length_in_bits = bit_string.substr(length_type_position+1,15);
			int total_length_of_packet = convertBinaryStringToInt(total_length_in_bits);
		}
		else {
			std::string number_of_subpackets_in_bits = bit_string.substr(length_type_position + 1, 11);
			int number_of_subpackets =

		}

		return value_length_pair;
	}
}

int versionSum(std::string&bit_string) {
	int version_sum = 0;
	int current_position = 0; // points at the first position of the current packeet
	while (current_position < bit_string.size()) {
		auto value_length_pair= computePacket(bit_string, current_position, false );
	}
	return version_sum;
}


int main()
{
	std::string file_name = "input.txt";
	std::vector<std::string> data = readFile(file_name);

	//std::string test_string = "D2FE28";
	//std::cout << convertToBitString(test_string);
	std::cout << convertBinaryStringToInt("0000");




}
