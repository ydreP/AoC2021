#include <iostream>
#include <string> 
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>    
#include <regex>
#include <tuple>

class LineSegment {
public:
    std::vector<std::tuple<int, int>> points;
    bool diagonal;
    LineSegment(int xs, int ys, int  xe, int ye) {
        //check what type of line it is. 
        // Is it 1 point, is it horizontal, vertical or diagonal?

        if (xs == xe) { // if xs =xe then it is either a point or vertical
            for (int i = std::min(ys, ye); i <= std::max(ys, ye); i++) {
                    auto point = std::make_tuple(xs, i);
                    points.push_back(point);
            }
            diagonal = false; 
        }
        else { // if xs != xe then either ys=ye or the line is a diagonal line 
            if (ys == ye) {
                for (int i = std::min(xs, xe); i <= std::max(xs, xe); i++) {
                    auto point = std::make_tuple(i, ys);
                    points.push_back(point);
                }
                diagonal = false;
            }
            else {
                // if xs != xe and ys != ye then it is a diagonal line. Each line is of the form y = (+/-) x+b.
                if (xs < xe) {
                    int sign = (ye - ys) / (xe - xs);
                    if (sign != 1 && sign != -1) {
                        std::cout << "Weird stuff is going on with coordinates: \n";
                        std::cout << "(" << xs << "," << ys << ")" << "->" << "(" << xe << "," << ye << ") \n";
                    }
                    for (int i = xs; i <= xe; i++) {
                        auto point = std::make_tuple(i, sign*(i-xs)+ys );
                        points.push_back(point);
                    }
                }
                else { // if xe<xs then we 'reparametrize'
                    int x1 = xe; int x2 = xs; int y1 = ye; int y2 = ys;
                    int sign = (y2 - y1) / (x2 - x1);
                    if (sign != 1 && sign != -1) {
                        std::cout << "Weird stuff is going on with coordinates: \n";
                        std::cout << "(" << xs << "," << ys << ")" << "->" << "(" << xe << "," << ye << ") \n";
                    }
                    for (int i = x1; i <= x2; i++) {
                        auto point = std::make_tuple(i, sign * (i-x1) + y1 );
                        points.push_back(point);
                    }
                }
                diagonal = true;
            }
        }
    }
    void toString() {
        std::tuple<int,int> starting_point = points[0];
        std::tuple<int, int> ending_point = points.back();
        int x1 = std::get<0>(starting_point); 
        int y1 = std::get<1>(starting_point);
        int x2 = std::get<0>(ending_point);
        int y2 = std::get<1>(ending_point);
        std::cout << "(" << x1 << "," << y1 << ") -> (" << x2 << "," << y2 << ")";
    }

};
bool overlap(LineSegment l1, LineSegment l2) {
    for (std::tuple<int, int>& t1 : l1.points) {
        for (std::tuple<int, int>& t2 : l2.points) {
            if (t1 == t2) {
                return true;
            }
        }
    }
    return false;    
}

std::vector<std::tuple<int, int>> intersect(LineSegment l1, LineSegment l2) {
    std::vector<std::tuple<int, int>> intersection;
    for (std::tuple<int, int>& t1 : l1.points) {
        for (std::tuple<int, int>& t2 : l2.points) {
            if (t1 == t2) {
                intersection.push_back(t1);
            }
        }
    }
    return intersection;
}


// Propotype functions
std::vector<std::string> read_file(std::string str);
std::vector<std::string> splitString(std::string str, char delimiter);
int main() {
    std::string filename = "input.txt";
    std::vector<std::string> data = read_file(filename);
    std::vector<LineSegment> line_segments;
    std::regex pattern(R"(\d+,\d+)");
    for (const std::string& line : data) {
        std::sregex_iterator currentMatch(line.begin(), line.end(), pattern);
        std::sregex_iterator lastMatch;
        std::vector<std::string> pairs; // Vector to store the matched pairs
        while (currentMatch != lastMatch) {
            std::smatch match = *currentMatch;
            pairs.push_back(match.str());
            ++currentMatch;
        }
        // std::cout << "Starting pos: " << pairs[0] << "ending pos " << pairs[1] << "\n";
        std::vector<std::string> start_pos = splitString(pairs[0], ',');
        // std::cout << "x1= "<< start_pos[0] << " y1 = " << start_pos[1] << "\n";
        std::vector<std::string> end_pos = splitString(pairs[1], ',');
        int xs = std::stoi(start_pos[0]);
        int xe = std::stoi(end_pos[0]);
        int ys = std::stoi(start_pos[1]);
        int ye = std::stoi(end_pos[1]);
        LineSegment line(xs, ys, xe, ye);
        line_segments.push_back(line);
    }
    int nbr_of_intersects = 0;
    int nbr_of_overlaps = 0;
    std::vector<std::tuple<int, int>> intersection_pattern;
    for (size_t i = 0; i < line_segments.size(); i++) {
        for (size_t j = i + 1; j < line_segments.size(); j++) {
            if (overlap(line_segments[i], line_segments[j]) ){
                auto intersection = intersect(line_segments[i], line_segments[j]);
                //for each tuple, t
                for (std::tuple<int, int>& t : intersection) {
                    // check if tuple t is already in intersection_pattern,
                    auto it = std::find(intersection_pattern.begin(), intersection_pattern.end(), t);
                    if (it == intersection_pattern.end()) {
                        intersection_pattern.push_back(t);
                        //std::cout << "Element (" << std::get<0>(t) << "," << std::get<1>(t) << ") " << " is not found in the vector." << std::endl;
                    }
                }
            }
        }
    }
    std::cout << intersection_pattern.size();
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
