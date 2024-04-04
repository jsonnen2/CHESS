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

class Board {

public:
    char* charBoard;
    char* player;
    char* castling_rights;
    char* enPassant;
    char* fifty_move_draw_rule;
    char* fullmove_count;

    Board(char* charBoard, char* player, char* castling_rights, 
            char* enPassant, char* fifty_move_draw_rule, char* fullmove_count) {
        this->charBoard = charBoard;
        this->player = player;
        this->castling_rights = castling_rights;
        this->enPassant = enPassant;
        this->fifty_move_draw_rule = fifty_move_draw_rule;
        this->fullmove_count = fullmove_count;
    }
    void print_board() {
        for (int i = 0; i < 64; ++i) {
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
            {'k', -20}, {'K', 20},
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
    char* black_board;
    char* white_board;
    // 64 array of NULL or bitboards representing all legal moves for each piece.
    char* chessboard;
};
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

// The core64 is a pointer to the start of my fen string.
// strtok(fen, " ")
// was applied to fen before shoving it in this method. (and made a copy)
// THIS METHOD EDITS YOUR STRING!!!
Board create_board_obj(char* core64) {
    char* player = strtok(NULL, " ");
    char* castling_rights = strtok(NULL, " ");
    char* enPassant = strtok(NULL, " ");
    char* fifty_move_draw_rule = strtok(NULL, " ");
    char* fullmove_count = strtok(NULL, " ");
    char* charBoard = fen_get_charBoard(core64);
    
    Board board(charBoard, player, castling_rights, enPassant, fifty_move_draw_rule, fullmove_count);

    /* PRINT STATEMENTS */
    /*__________________*/
    // board.print_board();
    // cout << board.player << endl;
    // cout << board.castling_rights << endl;
    // cout << board.enPassant << endl;
    // cout << board.fifty_move_draw_rule << endl;
    // cout << board.fullmove_count << endl;

    return board;
}
// Enter any int and return internal binary representation.
// Boolean to print to console or not.
int binary(char n, bool verbose) {
    int result;
    // I think I just want a diff alg
    if (n < 0) {
        std::queue<int> queue;
        char running_total = (1 << 7);
        queue.push(1);
        for(int i = 6; i >= 0; --i) {
            if(running_total + (1 << i) <= n) {
                queue.push(1);
                running_total += (1 << i);
            }
            else queue.push(0);
        }
        // Read our queue
        result = 0;
        while (queue.empty() == false) {
            result *= 10;
            result += queue.front();
            queue.pop();
        }
    }
    else {
        std::stack<int> stack;
        while(n > 0) {
            stack.push(n%2);
            n = n/2;
        }
        result = 0;
        while (stack.empty() == false) {
            result *= 10;
            result += stack.top();
            stack.pop();
        }
    }    
    if (verbose) {
        printf("%08d", result);
        printf("\n");
    }
    return result;
}
// Takes in a pointer to a bitboard and zeros out all indices.
void zero_bitboard(char* bitboard) {
    for(int i = 0; i < sizeof(bitboard); ++i) {
        bitboard[i] = 0b00000000;
    }
}
// set bitboard to char c
void set_bitboard(char* bitboard, char c) {
    // TODO: make char c an optional param
    for(int i = 0; i < sizeof(bitboard); ++i) {
        bitboard[i] = c;
    }
}
void print_bitboard(char* bitboard) {
    for(int i = 0; i < sizeof(bitboard); ++i) {
        binary(bitboard[i], true);
    }
}
// Given current charBoard, update the bitboard given the pointer in memory
void generate_colored_bitboard(char* charBoard, char* bitboard, bool isWhite) {
    for(int i = 0; i < 8; ++i) {
        char row = 0;
        for(int j = 0; j<8; ++j) {
            row = row << 1;
            if (charBoard[8*i + j] != '.') {
                if (!(isWhite xor isupper(charBoard[8*i + j]))) {
                    row += 1;
                }
            }
            binary(row, true);
        }
        cout << i << ": ";
        printf("%08d", binary(row, false));
        cout << endl;
        bitboard[i] = row;
    }
}

int main() {
    // TODO: throw errors for illegal fen strings
    // check that each 'row' contains 8 characters
    const char fen[] = "rnbqkbnr/pppppppp/8/8/P7/8/1PPPPPPP/RNBQKBNR b KQkq - 0 1";

    // Copy pointer
    char core64[100];
    strcpy(core64, fen);
    strtok(core64, " ");

    Board chessboard = create_board_obj(core64);
    chessboard.print_board();
    chessboard.evaluation();

    char* bitboard = new char[8];
    set_bitboard(bitboard, 0b00000000);
    print_bitboard(bitboard);

    generate_colored_bitboard(chessboard.charBoard, bitboard, true);
    cout << "==================" << endl;
    print_bitboard(bitboard);

    cout << "==================" << endl;

    char c = 0b11100100;
    // c = c << 1;
    cout << c << endl;
    binary(c, true);
    binary('d', true);

    char d = 'd';
    cout << (int) c << endl;

    uint64_t foo = 0ULL;
    foo = static_cast<uint64_t>(1) << 63;
    cout << foo << endl;

    // Additional board representations.
        // given player turn, iterate through every square
        // for each player piece, generate legal moves
            // 64 array of bitboards
            // NULL for empty squares
            // start with 64 square * 64 bits of allocated memory
        // Two more bitboards: BlackPieces and WhitePieces        
}
