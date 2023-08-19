#include <iostream>
#include <string> 
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>
#include <map>
#include <algorithm>
#include <climits>
#include <limits>
#include <utility>
#include <cctype>
#include <queue>
#include <chrono>
#include <set>

using namespace std;

const int INF = numeric_limits<int>::max();

vector<vector<int>> risk_map;


vector<string> readFile(string str) {
	vector< string > input;
	ifstream inputFile(str);
	if (!inputFile.is_open()) {
		// Handle the error if the file couldn't be opened
		std::cerr << "Error opening the file." << std::endl;
		return input;
	}
	string line;
	while (getline(inputFile, line)) {
		if (line != "") {
			// Process the line here
			input.push_back(line);
		}
	}
	inputFile.close();
	return input;
}
vector<string> splitString(string str, const string& delimiter) {
	vector<string> tokens;
	size_t pos = 0;

	while ((pos = str.find(delimiter)) != string::npos) {
		tokens.push_back(str.substr(0, pos));
		str.erase(0, pos + delimiter.length());
	}
	tokens.push_back(str); // Add the last part after the last delimiter
	return tokens;
}
vector<vector<int>> generateRiskMap(vector<string> data) {
	int rows = data.size();
	int cols = data[0].size();
	vector<vector<int>> matrix;
	for (const string& row : data) {
		vector<int> matrix_row;
		for (const char& risk_value : row) {
			// Convert the entire substring into an integer
			matrix_row.push_back(risk_value-'0');
		}
		matrix.push_back(matrix_row);
	}
	return matrix;
}

vector<vector< int> > enlargeRiskMap(vector < vector<int> > small_map, int enlargement_parameter) {
	int rows = small_map.size();
	int cols = small_map[0].size();
	vector<vector<int> > enlarged_risk_map( rows*enlargement_parameter,vector<int>(cols*enlargement_parameter,0) );
	// Create enlarged risk map by following the rules 
	for (int row = 0; row < rows; row++) {
		for (int col = 0; col < cols; col++) {
			for (int i = 0; i < enlargement_parameter; i++) {
				for (int j = 0; j < enlargement_parameter; j++) {
					enlarged_risk_map[row + i * rows][col + j * cols] = small_map[row][col] + i + j;
					// If we exceed 9, we shift it down by 9 (can be done by using modulo things).
					if (enlarged_risk_map[row + i * rows][col + j * cols] > 9) { 
						enlarged_risk_map[row + i * rows][col + j * cols] = enlarged_risk_map[row + i * rows][col + j * cols] - 9; 
					}
				}
			}
		}
	}
	return enlarged_risk_map;
}
/* custom comparator for dijkstra method, nodes in the graph are effectively pair<int,int>
* and i want to order them according to the distance from the source node.
*/
struct Comparator {
	typedef pair<pair<int, int>, int> value_type;
	bool operator()(const pair<pair<int, int>, int>& node_a, const pair<pair<int, int>, int>& node_b) const {
		return node_a.second > node_b.second;
	}
};

vector<pair<int, int>> dijkstraSearch(int row_pos, int col_pos, int final_row_pos, int final_col_pos) {
	map<pair<int, int>, int> distances;
	map<pair<int, int>, pair<int, int>> previous;
	priority_queue< pair<pair<int, int>, int>, vector<pair<pair<int, int>, int>>, Comparator > pq;
	distances[{row_pos, col_pos}] = 0;
	for (int row = 0; row < risk_map.size(); row++) {
		for (int col = 0; col < risk_map[0].size(); col++) {
			if (row != row_pos || col != col_pos) {
				distances[{row, col}] = INF;
			}
			pair< pair<int, int>, int> new_pos = { {row, col   },distances[{row, col}] };
			pq.push(new_pos);
		}
	}
	vector<pair<int, int> > shifts = { { 0,1 }, { 1,0 } ,{0,-1},{-1,0} };
	while (!pq.empty()) {
		//Get the best vertex, 
		pair< pair<int, int>, int> best_vertex = pq.top();
		pair<int, int> node = best_vertex.first;
		pq.pop();
		//If we have reached our target return path.
		if (node.first == final_row_pos && node.second == final_col_pos) {
			vector<pair<int, int>> min_path;
			min_path.push_back(node);
			auto previous_node = previous[node];
			while ( ! ( previous_node.first == row_pos && previous_node.second == col_pos) ) {
				min_path.push_back(previous_node);
				node = previous[previous_node];
				//min_path.push_back(node);
				previous_node = node;

			}
			min_path.push_back(previous_node);
			return min_path;
		}
		if (best_vertex.second > distances[node]) {
			continue;
		}
		//For each neighbor 
		for (const auto& shift : shifts) {

			int neighbor_row = node.first + shift.first;
			int neighbor_col = node.second + shift.second;
			pair<int, int> neighbor = { neighbor_row,neighbor_col };
			//If actual neighbor, i.e not out of bounds.
			if (neighbor_col < risk_map[0].size() && neighbor_row < risk_map.size() && neighbor_col>= 0 && neighbor_row>=0) {
				int alt = distances[node] + risk_map[neighbor_row][neighbor_col];
				if (alt < distances[neighbor]) {
					distances[neighbor] = alt;
					pq.push({ neighbor,alt });
					previous[neighbor] =node; 
				}
			}
		}
	}
}
void printDijkstraPath(vector<pair<int, int>> path) {
	int rows = risk_map.size();
	int cols = risk_map[0].size();
	for (int row = 0; row < rows; row++) {
		for (int col = 0; col < cols; col++) {
			pair<int, int> possible_path = { row,col };
			bool exists = std::find(path.begin(), path.end(), possible_path) != path.end();
			if (exists) {
				std::cout <<  "*";
			}
			else {
				std::cout << risk_map[row][col] ;
			}
		}
		std::cout << "\n";
	}
}

int main() {
	string test_file = "test.txt";
	string input_file = "input.txt";
	string enlarged_test = "enlarged.txt";
	vector<string> data = readFile(test_file);
	risk_map = generateRiskMap(data);
	risk_map = enlargeRiskMap(risk_map,5);
	auto test_data = readFile(enlarged_test);
	auto large_test_map = generateRiskMap(test_data);

	vector<pair<int, int>> min_path = dijkstraSearch(0, 0, risk_map.size() - 1, risk_map[0].size() - 1);
	std::reverse(min_path.begin(), min_path.end());
		int risk = -risk_map[0][0];
	for (const auto& node : min_path ) {
		risk += risk_map[node.first][node.second];
	}
	std::cout << "Minimal risk is: " << risk << endl;
	return 0;
}
