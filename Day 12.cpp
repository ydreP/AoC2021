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
#include <cctype>

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
std::vector<std::string> split_string(std::string str, char delimiter) {
	std::vector<std::string> tokens;
	std::istringstream iss(str);
	std::string token;
	while (std::getline(iss, token, delimiter)) {
		tokens.push_back(token);
	}
	return tokens;
}
std::map<std::string, std::set<std::string>> caveSystem; //global variable to shorten the number of arguments in find_paths

//Returns a graph given specified input format
std::map<std::string, std::set<std::string>> generate_graph(std::vector<std::string> data) {
	std::map<std::string, std::set<std::string>> graph;
	char separator = '-';
	for (std::string& line : data) {
		auto edge = split_string(line, separator);//edge consists of two vertices 
		// if vertex edge[0] is present we update the neighborhood list, otherwise we add it to the list  
		if (graph.count(edge[0]) > 0) {
			auto neighbors = graph[edge[0]];
			neighbors.insert(edge[1]);
			graph[edge[0]] = neighbors;
		}
		else {
			std::set<std::string> newNeighbors({ edge[1] });
			graph[edge[0]] = newNeighbors;
		}
		// if vertex edge[1] is present we update, otherwise we add it to the list  
		if (graph.count(edge[1]) > 0) {
			auto neighbors = graph[edge[1]];
			neighbors.insert(edge[0]);
			graph[edge[1]] = neighbors;
		}
		else {
			std::set<std::string> newNeighbors({ edge[0] });
			graph[edge[1]] = newNeighbors;
		}
	}
	return graph;
}

//Checks if a cave is small according to the specification.
bool is_small_cave(const std::string& cave) {
	for (const char& c : cave) {
		if (!std::islower(c)) {
			return false;
		}
	}
	return true;
}

//Returns a map that counts the number a small cave has been visited. 
std::map<std::string, int> generate_visit_map(std::map<std::string, std::set<std::string>> graph) {
	std::map<std::string, int> visits;
	for (const auto& nodeNeighborsPair : graph) {
		auto node = nodeNeighborsPair.first; 
		if (is_small_cave(node)) {
			visits[node] = 0;
		}
	}
	return visits;
}
//Simple print method
void print_graph(std::map<std::string, std::set<std::string>> graph) {
	for (const auto& nodeNeighborsPair : graph) {
		auto node = nodeNeighborsPair.first; 
		auto neighbors = nodeNeighborsPair.second;
		std::cout << " Node " << node << " is connected to: ";
		for (const std::string& neighbor : neighbors) {
			std::cout << neighbor << ", ";
		}
		std::cout << "\n";
	}
}

//To speed up this method, use memoization and pruning.
std::set< std::vector<std::string>> find_paths (std::string start, std::string target , 
												std::map<std::string,int> visitedSmallCaves, bool canVisitTwice ) {
	//does a DFS search to find all paths. 
	std::set<std::vector<std::string> > paths;
	std::string special_node = "start";

	/*
	 * Here we could add a check to see if we already have  
	 * computed the set of paths using memoization-
	 */

	//If we have reached our target we are done.
	if (start == target) {
		std::vector<std::string> singlePath = { start };
		paths.insert(singlePath);
		return paths; 
	}
	std::set<std::string> remainingNeighbors;
	auto neighbors = caveSystem[start];
	// Consider all reachable neighbors:
	for (const auto& neighbor : neighbors) {
		if (neighbor != special_node) {
			if (visitedSmallCaves[neighbor] < 1 || canVisitTwice) {
				remainingNeighbors.insert(neighbor);
			}
		}
	}	
	for (const std::string& neighbor : remainingNeighbors) {
		auto newVisited = visitedSmallCaves;
		bool newCanVisitTwice = canVisitTwice;
		if (is_small_cave(neighbor)) {
			newVisited[neighbor]= newVisited[neighbor]+1;
			if(newVisited[neighbor] ==2){
				newCanVisitTwice = false;
			}
		}
		auto newPaths = find_paths(neighbor, target, newVisited, newCanVisitTwice);
		for (const std::vector <std::string>& path : newPaths) {
			std::vector<std::string> modifiedPath;
			modifiedPath.push_back(start);
			for (const auto& node : path) {
				modifiedPath.push_back(node);
			}
			paths.insert(modifiedPath);
		}	
	}
	return paths;
}






//Simple print method. 
void print_path(std::vector<std::string> path) {
	for (const auto& node : path) {
		std::cout << node << "->";
	}
	std::cout << "\n";
}

int main() {
	std::string filename = "input.txt";
	std::string start = "start";
	std::string target = "end";
	std::set< std::string>  startingPoint({ start });
	std::vector<std::string> data = read_file(filename);
	caveSystem = generate_graph(data);
	auto visitMap = generate_visit_map(caveSystem);
	visitMap[start]=1;
	bool canVisitTwice = true; 
	auto paths = find_paths(start, target, visitMap,canVisitTwice);
	//for (const auto& path : paths) {
	//	print_path(path);
	//}
	std::cout << paths.size();
	//print_graph(caveSystem);
	return 0;
}
