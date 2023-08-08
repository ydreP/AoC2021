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
#include <queue>

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
//converts the raw input to a matrix of integers.
std::vector<std::vector<int>> convert_to_height_map(std::vector<std::string> rawData) {
	std::vector<std::vector<int>> convertedData;
	char shift = '0';
	//loop through each row and convert it into a vector of ints. 
	for (std::string& stringRow : rawData) {
		std::vector<int> intRow;
		for (char& charValue : stringRow) {
			intRow.push_back(charValue - shift);// shift the character to its proper int value
		}
		convertedData.push_back(intRow);
	}
	return convertedData;
}
//finds the lowest points, returns a vector of tuples. 
std::vector<std::pair<int, int> > find_low_points(std::vector<std::vector<int>> heightMap) {
	std::vector<std::pair<int, int> > positions;
	std::vector<std::pair<int, int> > shifts = { {-1,0},{1,0},{0,1},{0,-1} };
	for (int row = 0; row < heightMap.size(); row++) {
		for (int col = 0; col < heightMap[0].size(); col++) {
			bool isLowPoint = true;
			for (const auto& shift : shifts) {
				int row_shift = shift.first;
				int col_shift = shift.second;
				int adj_row = row + row_shift;
				int adj_col = col + col_shift;
				//Check if shift is allowed
				if (adj_row >= 0 && adj_row < heightMap.size() && adj_col >= 0 && adj_col < heightMap[0].size()) {
					isLowPoint = (heightMap[row][col] < heightMap[adj_row][adj_col]) && isLowPoint;
				}
			}
			if (isLowPoint) {
				positions.push_back({ row,col });
			}
		}
	}
	return positions;
}
int total_risk(std::vector<std::vector<int>> heightMap, std::vector<std::pair<int, int> > lowPoints) {
	int totalRisk = 0;
	for (const auto& lowPoint : lowPoints) {
		int row = lowPoint.first;
		int col = lowPoint.second;
		totalRisk = totalRisk + heightMap[row][col] + 1;
	}
	return totalRisk;
}

int basin_size(std::vector<std::vector<int>> heightMap, std::vector<std::vector<bool>> isVisited, int row, int col) {
	//Check if out of bounds
	if (row < 0 || row >= heightMap.size() || col < 0 || col >= heightMap[0].size()) {
		return 0;

	}
	else if (heightMap[row][col] == 9 || isVisited[row][col]) {

		//Check if we are at height 9 or already visited.
		//std::cout << " ... but we already visited that one or it has height 9 \n";
		return 0;
	}
	else {
		int size = 1;
		isVisited[row][col] = true;
		std::vector<std::pair<int, int> > shifts = { {-1,0},{1,0},{0,1},{0,-1} };
		for (auto& shift : shifts) {
			int row_shift = shift.first;
			int col_shift = shift.second;
			int adj_row = row + row_shift;
			int adj_col = col + col_shift;
			if (adj_row >= 0 && adj_row < heightMap.size()
				&& adj_col >= 0 && adj_col < heightMap[0].size()) {
				if (heightMap[adj_row][adj_col] > heightMap[row][col]) {
					//std::cout << "( " << row << "," << col << ") is connected to " << " (" << adj_row << "," << adj_col << ") \n";
					size = size + basin_size(heightMap, isVisited, adj_row, adj_col);
				}
			}
		}
		return size;
	}
}


std::set<std::pair<int, int>> basin_cluster(std::vector<std::vector<int>> heightMap,
	std::vector<std::vector<bool>> isVisited, int row, int col) {
	//Check if out of bounds
	if (row < 0 || row >= heightMap.size() || col < 0 || col >= heightMap[0].size()) {
		std::set<std::pair<int, int>> emptySet;
		return emptySet;
	}
	if (heightMap[row][col] == 9 || isVisited[row][col]) {

		//Check if we are at height 9 or already visited.
		//std::cout << " ... but we already visited that one or it has height 9 \n";
		std::set<std::pair<int, int>> emptySet;
		return emptySet;
	}

	std::set<std::pair<int, int>> cluster = { {row,col} };
	isVisited[row][col] = true;
	std::vector<std::pair<int, int> > shifts = { {-1,0},{1,0},{0,1},{0,-1} };
	for (auto& shift : shifts) {
		int row_shift = shift.first;
		int col_shift = shift.second;
		int adj_row = row + row_shift;
		int adj_col = col + col_shift;
		if (adj_row >= 0 && adj_row < heightMap.size()
			&& adj_col >= 0 && adj_col < heightMap[0].size()) {
			if (heightMap[adj_row][adj_col] > heightMap[row][col]) {
				//std::cout << "( " << row << "," << col << ") is connected to " << " (" << adj_row << "," << adj_col << ") \n";
				std::set<std::pair<int, int>> newPoints = basin_cluster(heightMap, isVisited, adj_row, adj_col);
				for (const std::pair<int, int>& point : newPoints) {
					if (cluster.count(point) == 0) {
						cluster.insert(point);
					}
				}

			}
		}
	}
	return cluster;
}

void printCluster(std::set<std::pair<int, int>> cluster) {
	for (const auto& point:cluster) {
		int row = point.first;
		int col = point.second;
		std::cout << "(" << row << "," << col << ") \n";
	}
}


int main() {
	std::string filename = "input.txt";
	std::vector<std::string> data = read_file(filename);
	std::vector< std::vector<int> > height_map = convert_to_height_map(data);
	/*
	for (const std::vector<int>& row : height_map) {
		for (const int& value : row) {
			std::cout << value;
		}
		std::cout << "\n";
	}
	*/
	auto lowPoints = find_low_points(height_map);//returns a vector of std::pair<int,int>
	// std::cout << "Total risk:  " << total_risk(height_map, lowPoints) << "\n";
	std::vector<int> basin_sizes; // Store basin sizes
	for (const auto& lowPoint : lowPoints) {
		int row = lowPoint.first;
		int col = lowPoint.second;
		//std::cout << "We start with low point:  (" << row << ", " << col << ") \n";
		std::vector<std::vector<bool>> isVisited(height_map.size(), std::vector<bool>(height_map[0].size()));
		int s = basin_cluster(height_map, isVisited, row, col).size();
		basin_sizes.push_back(s);
	}


	std::sort(basin_sizes.rbegin(), basin_sizes.rend()); // Sort basin sizes in descending order

	int largest_basin_product = 1;
	for (int i = 0; i < 3; i++) { // Multiply the sizes of the three largest basins
		largest_basin_product *= basin_sizes[i];
	}

	std::cout << "Largest basin product: " << largest_basin_product << "\n";
	return 0;
}
