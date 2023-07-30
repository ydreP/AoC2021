#include <iostream>
#include <string> 
#include <vector>
#include <fstream>
#include <sstream>
#include <regex>
class BingoBoard {
private: 
    int winning_round;
    int last_value; 
    std::vector<std::vector<std::string>> board;
    std::vector<std::vector<bool>> marked;
    bool done; 
    bool winning_round_set;
    bool last_value_set;
public: 
    BingoBoard(std::vector<std::vector<std::string>> b) {
        board = b; 
        int rows = b.size();
        int cols = b[0].size();
        winning_round = INT_MAX;
        marked = std::vector<std::vector<bool>>(rows, std::vector<bool>(cols));
        done = false;
        winning_round_set = false;
        last_value_set = false;
        last_value = INT_MAX;
    }
    void check_number(std::string c) {
        if (!done) {
            for (size_t i = 0; i < board.size(); i++) {
                for (size_t j = 0; j < board[i].size(); j++) {
                    marked[i][j] = marked[i][j] || (board[i][j] == c);
                }
            }
        }
    }
    bool won() {
        if (!done) {
            //loop over rows, check every row for 5 marks 
            std::vector<bool> row_bools(marked.size(),true);
            for (size_t row = 0; row < marked.size(); row++) {
                for (size_t col = 0; col < marked[0].size(); col++) {
                    row_bools[row] = (row_bools[row] && marked[row][col]);
                }
            }
            //loop over columns, check every columns for 5 marks 
            std::vector<bool> col_bools(marked[0].size(),true);
            for (size_t col = 0; col < marked[0].size(); col++) {
                for (size_t row = 0; row < marked.size(); row++) {
                    col_bools[col] = (col_bools[col] && marked[row][col]);
                }
            }
            // Check if one row has 5 marks
            bool row_win = false;
            bool col_win = false;
            for (const bool& b : row_bools) {
            row_win = (b || row_win);
            }
            // Check if one column has 5 marks
            for (const bool& b : col_bools) {
                col_win = (b || col_win);
            }
            done = (col_win || row_win);
            return done;
        }
        else {
            return done;
        }
    }
    void boolToString() {
        for (size_t row = 0; row < marked.size(); row++) {
            for (size_t col = 0; col < marked[0].size(); col++) {
                std::cout << marked[row][col] << " ";
            }
            std::cout << "\n";
        }
    }
    void toString() {
        for (size_t row = 0; row < marked.size(); row++) {
            for (size_t col = 0; col < marked[0].size(); col++) {
                std::cout << board[row][col] << " ";
            }
            std::cout << "\n";
        }

    }
    int get_value() {
        int value = 0;
        for (int row = 0; row < marked.size(); row++) {
            for (int col = 0; col < marked[row].size(); col++) {
                if (!marked[row][col]) {
                    value = value + std::stoi(board[row][col]);
                }
            }
        }
        return value; 
    }
    void set_winning_round(int r) {
        if (!winning_round_set) {
            winning_round = r;
            winning_round_set = true;
        }
    }
    int get_winning_round() { return winning_round; }
    void set_last_value(int r) { 
        if (!last_value_set) {
            last_value = r;
            last_value_set = true;
        }
    
    }
    int get_last_value() { return last_value; }
};
std::vector<std::string> read_file(std::string str);
std::vector<std::string> splitString(std::string str, char delimiter);
int main() {
    std::string filename = "input.txt";
    std::vector<std::string> data = read_file(filename);
    std::vector<BingoBoard> boards; 
    std::vector<std::string> turns;
    //Get the turns
    std::regex integerRegex("\\d+");
    std::sregex_iterator currentMatch(data[0].begin(), data[0].end(), integerRegex);
    std::sregex_iterator lastMatch;
    while (currentMatch != lastMatch) {
        std::smatch match = *currentMatch;
        std::string matchedInteger = match.str();
        turns.push_back(matchedInteger);
        // Move to the next match
        ++currentMatch;
    }
    // Create the boards
    for (size_t i = 1; i < data.size(); i++) {
        if (data[i] == "") {
            std::vector<std::vector<std::string>> new_board;
            int j = i + 1;
            while (j < data.size() && data[j] != ""  ) {
                std::regex integerRegex("\\d+");
                std::sregex_iterator currentMatch(data[j].begin(), data[j].end(), integerRegex);
                std::sregex_iterator lastMatch;
                std::vector<std::string> new_row;
                while (currentMatch != lastMatch) {
                    std::smatch match = *currentMatch;
                    std::string matchedInteger = match.str();
                    new_row.push_back(matchedInteger);
                    // Move to the next match
                    ++currentMatch;
                }
                new_board.push_back(new_row);
                j=j+1;
            }
            BingoBoard board(new_board);
            boards.push_back(board);
        }
    }
    int round = 1;
    for (std::string& turn : turns) {
        //std::cout << "We draw number " << turn << " at round " << round << "\n";
        for (BingoBoard& board : boards) {
            board.check_number(turn);
            if (board.won()) {
                board.set_winning_round(round);
                board.set_last_value(std::stoi(turn));
            }
        }
        round++;
    }
    BingoBoard last_board = boards[1];;
    int temp_last = INT_MAX;

    for (BingoBoard& board : boards) {
        //std::cout << "Board:  " << "\n";
        //board.toString();
        //std::cout << "won at round " << board.get_winning_round() << "\n";
        if (last_board.get_winning_round() <  board.get_winning_round()) {
            last_board = board;
        }
    }
    std::cout << "Board \n"; 
    last_board.toString();
    std::cout << "is the last board to win with score: " << last_board.get_value() * last_board.get_last_value();
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
            // Process the line here
        input.push_back(line);
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
