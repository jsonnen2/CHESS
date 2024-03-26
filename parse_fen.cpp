// parse_fen.cpp
#include <iostream>
#include "chess.h"

using namespace std;
using namespace CHESS;

void print_line(string token) {
    //print operations
    for (string::size_type i = 0; i < token.size(); ++i) {
        if (isdigit(token[i])) {
            int max = token[i] - '0';
            for (int i = 0; i < max; ++i) {
                cout << ". ";
            }
        }
        else {
            cout << token[i];
            cout << " ";
        }
    }
    cout << endl;
}
void core64_print(string core64) {
    //loop for each token
        // foreach character c
            // if (isdigit(c)) print c num of .
            // else print(c)
    string delimiter = "/";
    size_t pos = 0;
    string token;
    // while not at the end of the string
    while((pos = core64.find(delimiter)) != string::npos) {
        token = core64.substr(0, pos);
        print_line(token);
        core64.erase(0, pos + delimiter.length());
    }
    print_line(core64);
}

void fen_string::print_board() {
    // TODO: define this fen string when creating the fen_string object
    string fen = "rnbq1rk1/4bppp/p2ppn2/1p6/3NP3/1BN4P/PPP2PP1/R1BQ1RK1 w - - 2 10";

    string core64 = fen.substr(0, fen.find(" "));
    string rest = fen.substr(fen.find(" ")+1, fen.length());

    core64_print(core64);
}
