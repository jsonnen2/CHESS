// terminal_chess.cpp
#include <iostream>
#include "chess.h"

using namespace std;
using namespace CHESS;

// Runs immediatly when calling the function.
void Start() {
    string fen_board = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
}
// Runs immediatly after player gives their move
void Update(string move) {

}

int main() {
    fen_string fen;
    fen.print_board();
}
