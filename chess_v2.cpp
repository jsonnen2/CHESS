#include <iostream>
#include <string.h>
#include "chess.h"
#include <map>
#include <math.h>
#include <stack>
#include <queue>
#include <stdint.h>

using namespace std;
using namespace CHESS;

char* fen_get_charBoard(char core64[]);

class Board {

public:
    char* charBoard;
    char* player;
    char* castling_rights;
    char* enPassant;
    char* fifty_move_draw_rule;
    char* fullmove_count;
// THIS OBJECT INITIALIZATION EDITS YOUR STRING INPUT!!!
    Board(char* core64) {
        this-> player = strtok(NULL, " ");
        this-> castling_rights = strtok(NULL, " ");
        this-> enPassant = strtok(NULL, " ");
        this-> fifty_move_draw_rule = strtok(NULL, " ");
        this-> fullmove_count = strtok(NULL, " ");
        this-> charBoard = fen_get_charBoard(core64);
    }
    void print_board() {
        for (int i = 0; i < 64; ++i) {
            // newline every row
            if (i % 8 == 0 && i != 0) cout << endl;
            cout << charBoard[i] << ' ';
        }
        cout << endl;
    }
    // Define an evaluation function 
    double evaluation() {
        // Sum of piece values.
        std::map<char, double> piece_value = {
            {'p', -1}, {'P', 1},
            {'n', -3}, {'N', 3},
            {'b', -3}, {'B', 3},
            {'r', -5}, {'R', 5},
            {'q', -9}, {'Q', 9},
            {'k', -200}, {'K', 200},
        };
        double point_sum = 0;
        for(int i=0; i<64; ++i) {
            point_sum += piece_value.find(charBoard[i])->second;
        }
        cout << "Evaluation: " << point_sum << endl;
        return point_sum;
    }
};

class Bitboard {
// I will use fully public methods for now
public: 
    uint64_t white_pieces;
    uint64_t black_pieces;
    
    uint64_t pawn;
    uint64_t knight;
    uint64_t bishop;
    uint64_t rook;
    uint64_t queen;
    uint64_t king;
    
// another initialization method using fen string
// I think I can only have 1 initialization tho
    Bitboard(char* charBoard) {
        // build white and black pieces
        for(int i = 0; i < 64; ++i) {
            this-> white_pieces <<= 1;
            this-> black_pieces <<= 1;

            this-> pawn <<= 1;
            this-> knight <<= 1;
            this-> bishop <<= 1;
            this-> rook <<= 1;
            this-> queen <<= 1;
            this-> king <<= 1;

            if (charBoard[i] != '.') {
                if (isupper(charBoard[i])) 
                    this-> white_pieces += 1;
                else 
                    this-> black_pieces += 1;

                if (charBoard[i] == 'p' || charBoard[i] == 'P') 
                    this-> pawn += 1;
                else if (charBoard[i] == 'n' || charBoard[i] == 'N') 
                    this-> knight += 1;
                else if (charBoard[i] == 'b' || charBoard[i] == 'B') 
                    this-> bishop += 1;
                else if (charBoard[i] == 'r' || charBoard[i] == 'R') 
                    this-> rook += 1;
                else if (charBoard[i] == 'q' || charBoard[i] == 'Q') 
                    this-> queen += 1;
                else if (charBoard[i] == 'k' || charBoard[i] == 'K') 
                    this-> king += 1;
            }
        }
    }
    // return the (color?) and piece on a given square
    char get_piece_on_square(int square) {

    }
};
// Should be a look-up into my storage of all legal moves
bool is_legal_bitmove(uint16_t bitmove) {

}
// fen string to charBoard (easy to print)
char* fen_get_charBoard(char core64[]) {
    strtok(core64, "/");
    char* board = new char[64];

    int index = 0;
    while (core64 != NULL) {
        for (int i = 0; i < strlen(core64); ++i) {
            if (isdigit(core64[i])) {
                int max = core64[i] - '0'; //??
                for (int j = 0; j < max; ++j) {
                    board[index] = '.';
                    ++index;
                }
            }
            else {
                board[index] = core64[i];
                ++index;
            }
        }
        core64 = strtok(NULL, "/");
    }
    return board;
}
// Enter any int and return internal binary representation.
// Boolean to print to console or not.
void binary(uint64_t n) {
    std::stack<int> stack;

    while(n > 0) {
        stack.push(n%2);
        n = n/2;
    }
    while (stack.size() < 64) {
        stack.push(0);
    }
    int i = 63;
    while (stack.empty() == false) {
        if (i%8 == 7) cout << endl;
        cout << stack.top();
        stack.pop();
        --i;
    }
    cout << endl;
}

int main() {
    // TODO: throw errors for illegal fen strings
    // check that each "row" contains 8 characters
    const char fen[] = "rnbqkbnr/pppppppp/8/8/P7/8/1PPPPPPP/RNBQKBNR b KQkq - 0 1";

// Copy fen string, then tokenize by " "
    char core64[100];
    strcpy(core64, fen);
    strtok(core64, " ");

// board object using charBoard
    Board chessboard = Board(core64);
    chessboard.print_board();
    chessboard.evaluation();

    Bitboard bitboardss = Bitboard(chessboard.charBoard);

}
