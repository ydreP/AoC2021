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
#include <unordered_set>
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
struct DumboOctopus {
	bool hasFlashed;
	bool added;
	int energyLevel;
	int numberOfFlashes;
};



std::vector<std::vector<int>> convertToEnergyGrid(std::vector<std::string> input_data) {
	std::vector<std::vector<int>> energyMap;
	for (std::string& row : input_data) {
		std::vector<int> intRow;
		for (char& charInt : row) {
			intRow.push_back(charInt - '0');//shift the value to the correct numerical value.
		}
		energyMap.push_back(intRow);
	}
	return energyMap;
}
std::vector<std::vector<DumboOctopus>> createOctpiiGrid(std::vector<std::vector<int>> energyMap) {
	std::vector<std::vector<DumboOctopus>> octoGrid;
	for (std::vector<int>& energyRow : energyMap) {
		std::vector<DumboOctopus> octoRow;
		for (int& energyLvl : energyRow) {
			DumboOctopus octo;
			octo.energyLevel = energyLvl;
			octo.hasFlashed = false;
			octo.added = false;
			octo.numberOfFlashes = 0;
			octoRow.push_back(octo);
		}
		octoGrid.push_back(octoRow);
	}
	return octoGrid;
}
void octoGridToString(std::vector<std::vector<DumboOctopus>> octoGrid) {
	for (std::vector<DumboOctopus>& octo_row : octoGrid) {
		for (DumboOctopus& octo : octo_row) {
			std::cout << " " << octo.energyLevel << " ";
		}
		std::cout << "\n";
	}
}


void update(std::vector<std::vector<DumboOctopus>>& octoGrid) {
	std::queue<std::pair<int, int>> flashingOctos; // Keep a queue of all octopii that can flash; 
	std::queue<std::pair<int, int>> flashedOctos; // Keep a queue of all octopii that have flashed; 
	/*First, the energy level of each octopus increases by 1.*/
	for (int row = 0; row < octoGrid.size(); row++) {
		for (int col = 0; col < octoGrid[0].size(); col++) {
			octoGrid[row][col].energyLevel++;
			if (octoGrid[row][col].energyLevel > 9) { //Any octopus that has level greater than 9 will flash later on 
				std::pair<int, int> newPair({ row,col });
				flashingOctos.push(newPair);
				octoGrid[row][col].added = true;
				//std::cout << "Octopus at position (" << row + 1 << "," << col + 1 << ") will flash \n";
			}
		}
	}
	//octoGridToString(octoGrid);
	/*Then, any octopus with an energy level greater than 9 flashes.
	* This increases the energy level of all adjacent octopuses by 1,
	* including octopuses that are diagonally adjacent.
	* If this causes an octopus to have an energy level greater than 9, it also flashes.
	* This process continues as long as new octopuses keep having their energy level increased beyond 9.
	* (An octopus can only flash at most once per step.) */
	std::vector<std::pair<int, int> > shifts = { {-1,0},{1,0},{0,1},{0,-1},{1,1},{-1,-1},{1,-1},{-1,1} }; // Possible shifts
	//std::cout << flashingOctos.size();
	while (!flashingOctos.empty()) {
		auto flash_index = flashingOctos.front();
		flashingOctos.pop();
		flashedOctos.push(flash_index);
		int row = flash_index.first;
		int col = flash_index.second;
		//std::cout << "Octopus at position (" << row + 1 << "," << col + 1 << ") flashes \n";
		// the octopus flashes. 
		octoGrid[row][col].energyLevel = 0;
		octoGrid[row][col].hasFlashed = true;
		octoGrid[row][col].numberOfFlashes++;
		// for each octopus in the *-neighbourhood, increase the level, check if it has flashed, otherwise add it to the queue and keep going
		for (std::pair<int, int>& shift : shifts) {
			int row_shift = shift.first;
			int col_shift = shift.second;
			int adj_row = row + row_shift;
			int adj_col = col + col_shift;
			if (adj_row >= 0 && adj_row < octoGrid.size() && adj_col >= 0 && adj_col < octoGrid[0].size()) { // Check if it is a valid neighbor
				//std::cout << "Octopus at position (" << adj_row +1 << "," << adj_col +1 << ") increases it's energy \n";
				octoGrid[adj_row][adj_col].energyLevel++;
				if (octoGrid[adj_row][adj_col].energyLevel > 9 && !octoGrid[adj_row][adj_col].hasFlashed && !octoGrid[adj_row][adj_col].added ) {
					// If it has an energy level higher than 9 and hasn't flashed yet 
					//std::cout << "Octopus at position (" << adj_row + 1 << "," << adj_col + 1 << ") will flash \n";
					std::pair<int, int> newPair({ adj_row,adj_col });
					octoGrid[adj_row][adj_col].added = true;
					flashingOctos.push(newPair);
				}
			}
		}
	}
	/*Finally, any octopus that flashed during this step
	has its energy level set to 0, as it used all of its energy to flash. */
	while (!flashedOctos.empty()) {
		auto flash_index = flashedOctos.front();
		flashedOctos.pop();
		int row = flash_index.first;
		int col = flash_index.second;
		octoGrid[row][col].energyLevel = 0;
		octoGrid[row][col].added = false;
		octoGrid[row][col].hasFlashed = false;
	}
	//octoGridToString(octoGrid);
}

int totalNumberOfFlashes(std::vector<std::vector<DumboOctopus>> octoGrid) {
	int number_of_flashes = 0;
	for (auto& octoRow : octoGrid) {
		for (DumboOctopus& octo : octoRow) {
			number_of_flashes += octo.numberOfFlashes;
		}
	}
	return number_of_flashes;
}

bool synchronized(std::vector<std::vector<DumboOctopus>> octoGrid) {
	for (auto& octoRow : octoGrid) {
		for (DumboOctopus& octo : octoRow) {
			if (octo.energyLevel != 0) {
				return false;
			}
		}
	}
	return true;
}

int main() {
	std::string filename = "test.txt";
	std::vector<std::string> data = read_file(filename);
	auto convertedData = convertToEnergyGrid(data);
	std::vector<std::vector<DumboOctopus>> octo_grid;
	octo_grid = createOctpiiGrid(convertedData);
	int step = 1;
	int max_steps = 101;
	while (!synchronized(octo_grid)) {
		//std::cout << "Octogrid before : \n";
		//octoGridToString(octo_grid);
		update(octo_grid);
		//std::cout << "Octogrid after : \n";
		//octoGridToString(octo_grid);
		step++;
	}
	std::cout << step;
	//std::cout << totalNumberOfFlashes(octo_grid);
	return 0;
}
