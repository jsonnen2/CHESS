#include <iostream>
#include <stdint.h>
#include <string.h>
#include <vector>
#include <stack>
#include <math.h>
#include <windows.h>
using namespace std;
//  DATA STRUCTURES
struct FenString {
    char* fen;
};
struct StandardRep {
    char* char_board;
    char player_turn;
    char castling_rights;
    char en_passant;
    char fifty_move_draw_rule;
    char fullmove_count;
};
struct BitBoard {
    uint64_t white;
    uint64_t black;
    uint64_t pawn;
    uint64_t knight;
    uint64_t bishop;
    uint64_t rook;
    uint64_t queen;
    uint64_t king;
    char player_turn;
};
//  CONVERSION B/W DATA STRUCTURES
//            TODO TODO
// Research: other ways to initialize bits INSIDE THE COMPUTER
// 
// Directly on the micro controller
StandardRep standard_from_fen_string(FenString fen_string) {
    char *fen = fen_string.fen;
    char *char_board = new char[100];

    int array_count = 0;
    for (int i = 0; fen[i] != '\0'; ++i) {
        if (fen[i] == ' ') break;
        else if (fen[i] == '/') continue;
        else if (isdigit(fen[i])) {
            int max = fen[i] - '0'; //??
            for (int j = 0; j < max; ++j) {
                char_board[array_count] = '.';
                ++array_count;
            }
        }
        else {
            char_board[array_count] = fen[i];
            ++array_count;
        }
    }
    StandardRep b;
    b.char_board = char_board;
    strtok(fen, " ");
    b.player_turn = *strtok(NULL, " ");
    // TODO: fix castling_rights
    b.castling_rights = *strtok(NULL, " ");
    b.en_passant = *strtok(NULL, " ");
    b.fifty_move_draw_rule = *strtok(NULL, " ");
    b.fullmove_count = *strtok(NULL, " ");
    return b;
}
//  fails: reads only 1st character of the "special conditions" of the fen string
//  strtok(fen, " ") --- incoreect
//  *strtok(NULL, " ")
BitBoard bit_board_from_standard(StandardRep standard) {
    BitBoard bitboard;
    char *char_board = standard.char_board;

    if (standard.player_turn == 'w') bitboard.player_turn = -1;
    else bitboard.player_turn = 0;

    for (int i=0; i<64; ++i) {
        bitboard.white <<= 1; 
        bitboard.black <<= 1;
        bitboard.pawn <<= 1;
        bitboard.knight <<= 1;
        bitboard.bishop <<= 1;
        bitboard.rook <<= 1;
        bitboard.queen <<= 1;
        bitboard.king <<= 1;

        if (char_board[i] != '.') {
            if (isupper(char_board[i]))
                bitboard.white += 1;
            else 
                bitboard.black += 1;

            if (char_board[i] == 'p' || char_board[i] == 'P') 
                bitboard.pawn += 1;
            else if (char_board[i] == 'n' || char_board[i] == 'N') 
                bitboard.knight += 1;
            else if (char_board[i] == 'b' || char_board[i] == 'B') 
                bitboard.bishop += 1;
            else if (char_board[i] == 'r' || char_board[i] == 'R') 
                bitboard.rook += 1;
            else if (char_board[i] == 'q' || char_board[i] == 'Q') 
                bitboard.queen += 1;
            else if (char_board[i] == 'k' || char_board[i] == 'K') 
                bitboard.king += 1;
        }
    }
    return bitboard;
}
//  FAIL:   I don't initialize the fen string here.
// No where in memory is a "new" variable, set of bits created
// Created in my main method
StandardRep standard_from_bit_board(BitBoard b) {
    StandardRep standard;
    char *char_board = new char[65];
    int i = 63;
    for (uint64_t bitmask=1; bitmask != 0; bitmask <<= 1) {
        if (bitmask & b.white & b.pawn) 
            char_board[i] = 'P';
        else if (bitmask & b.black & b.pawn) 
            char_board[i] = 'p';
        else if (bitmask & b.white & b.rook) 
            char_board[i] = 'R';
        else if (bitmask & b.black & b.rook) 
            char_board[i] = 'r';
        else if (bitmask & b.white & b.knight) 
            char_board[i] = 'N';
        else if (bitmask & b.black & b.knight) 
            char_board[i] = 'n';
        else if (bitmask & b.white & b.bishop) 
            char_board[i] = 'B';
        else if (bitmask & b.black & b.bishop) 
            char_board[i] = 'b';
        else if (bitmask & b.white & b.king) 
            char_board[i] = 'K';
        else if (bitmask & b.black & b.king) 
            char_board[i] = 'k';
        else if (bitmask & b.white & b.queen) 
            char_board[i] = 'Q';
        else if (bitmask & b.black & b.queen) 
            char_board[i] = 'q';
        else 
            char_board[i] = '.';
        --i;
    }
    standard.char_board = char_board;
    // TODO: other board states
    return standard;
}
FenString fen_string_from_standard(StandardRep char_board) {
    FenString f;
    return f;
}
//  PRINT METHODS
void print_binary64(uint64_t n) {
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
void print_binary16(uint16_t n) {
    std::stack<int> stack;
    while(n > 0) {
        stack.push(n%2);
        n = n/2;
    }
    while (stack.size() < 16) {
        stack.push(0);
    }
    int i = 15;
    while (stack.empty() == false) {
        if (i == 5 || i == 9) cout;
        cout << stack.top();
        stack.pop();
        --i;
    }
    cout << endl;
}
void print_bitmove_vector_as_binary(vector<uint16_t> bitmoves) {
    for (int i = 0; i < bitmoves.size(); ++i) {
        print_binary16(bitmoves[i]);
    }
}
void print_bitmove_vector_as_int(vector<uint16_t> bitmoves) {
    uint16_t to_square = 0b1111110000000000;
    uint16_t special = 0b0000001111000000;
    uint16_t from_square = 0b0000000000111111;
    for (int i = 0; i < bitmoves.size(); ++i) {
        cout << ((bitmoves[i] & to_square) >> 10) << "\t";
        cout << ((bitmoves[i] & special) >> 6) << "\t";
        cout << (bitmoves[i] & from_square) << endl;
    }
}
void print_char_board(char* charBoard) {
    for (int i = 0; i < 64; ++i) {
        // newline every row
        if (i % 8 == 0 && i != 0) cout << endl;
        cout << charBoard[i] << ' ';
    }
    cout << endl;
}
void print_char_board_using_bitboard(BitBoard bitboard) {
    StandardRep standard = standard_from_bit_board(bitboard);
    char* char_board = standard.char_board;
    print_char_board(char_board);
}
void print_big_board(BitBoard bitboard) {
    struct AsciiPiece {
        char* art;
        int width;
        int height;
    };
    AsciiPiece rook;
    char rook_ascii_art[82] = " _ _ _ _ | | | | ||_______|\\__ ___ / |___|_|  |_|___|  |___|_| (_______)(_______)";
    rook.art = rook_ascii_art;
    rook.width = 9;
    rook.height = 9;

    AsciiPiece knight;
    char knight_ascii_art[205] = "     ,....,        ,::::::<        ,::/^\"``.       ,::/, `   e`.    ,::; |        '. ,::|  \\___,-.  c);::|     \\   '-' `::|  _.=`\\       ;:|.=` .=`\\       '|_.=`  __\\      `\\_..==`` /      ('--....--')   ";
    knight.art = knight_ascii_art;
    knight.width = 17;
    knight.height = 12;

    AsciiPiece queen;
    char queen_ascii_art[106] = "    .o'   .o   o   /\\    /\\  o\\`.'  \\__/  `'/ \\    '++'   /   \\         /     \\_.---._/      '(_____)'   ";
    queen.art = queen_ascii_art;
    queen.width = 15;
    queen.height = 7;

    AsciiPiece bishop;
    char bishop_ascii_art[157] = "    _0_        /  /\\      /;-/ /\\    /  / /  \\   \\  \\/   /    \\____./     <--===>     / _.=`\\    /=` _.=`\\   /.=`  __\\  /..==``   \\ (.'.__.-`.) ('--....-') ";
    bishop.art = bishop_ascii_art;
    bishop.width = 12;
    bishop.height = 13;
}
void print_goofy_ahh_board(BitBoard bitboard) {
    // INEFFICIENT: print one ascii character at a time.
    struct AsciiPiece {
        char* art;
        int width;
        int height;
    };
    AsciiPiece blank;
    char blank_ascii_art[5] = "    ";
    blank.art = blank_ascii_art;
    blank.width = 2;
    blank.height = 2;
    // Tweetie bird
    AsciiPiece tweetie;
    char pawn_ascii_art[91] = " .-'-.    /_ _  \\   \\@ @  /   (_> _)      `)(_      /((_`)_,  \\__(/-'  __|||__  ((__|__)) ";
    tweetie.art = pawn_ascii_art;
    tweetie.width = 10;
    tweetie.height = 9;
    // Derp Beaver
    AsciiPiece derp_beaver;
    char rook_ascii_art[151] = "     .---.         @ @   )        ^     |       [|]    | ##    /      |####  (       |####   \\| /   |####  / |.'   |###  _\\ ``\\   )##  /,,_/,,____#   ";
    derp_beaver.art = rook_ascii_art;
    derp_beaver.width = 15;
    derp_beaver.height = 10;
    // Marge Simpson
    AsciiPiece marge_simpson;
    char queen_ascii_art[253] = "           (####)          (#######)       (#########)      (#########)      (#########)      (#########)      (#########)      (#########)        (o)(o)(##)      ,_C     (##)     /___,   (##)        \\     (#)          |    |            OOOOOO         ";
    marge_simpson.art = queen_ascii_art;
    marge_simpson.width = 18;
    marge_simpson.height = 14;
    // Homer Simpson
    AsciiPiece homer;
    char king_ascii_art[121] = "   __&__      /     \\    |       |   |  (o)(o)   C   .---_)   | |.___|    |  \\__/     /_____\\    /_____/ \\  /         \\ ";
    homer.art = king_ascii_art;
    homer.width = 12;
    homer.height = 10;
    // Goofy Elk
    AsciiPiece goof_ah_elk;
    char knight_ascii_art[166] = "   ()             ))    ((      //      \\\\    | \\\\____// |  \\~/ ~    ~\\/~~/ (|    _/o  ~~   /  /     ,|   (~~~)__.-\\ |    ``~~    | |     |      | |     |      | |  ";
    goof_ah_elk.art = knight_ascii_art;
    goof_ah_elk.width = 15;
    goof_ah_elk.height = 11;
    // Dome Guy
    AsciiPiece dome_guy;
    char bishop_ascii_art[122] = "   ,,,,          ))))        .-'';___    / _.'`   `; | (_.--''-./ \\  | (.)(.)\\  ) \\      // [   `]'-'\\`]  `''`     ` ";
    dome_guy.art = bishop_ascii_art;
    dome_guy.width = 13;
    dome_guy.height = 9;


//          ,....,
//       ,::::::<
//      ,::/^\"``.
//     ,::/, `   e`.
//    ,::; |        '.
//    ,::|  \___,-.  c)
//    ;::|     \   '-'
//    ;::|      \
//    ;::|   _.=`\
//    `;:|.=` _.=`\
//      '|_.=`   __\
//      `\_..==`` /
//       .'.___.-'.
//      /          \
// jgs ('--......--')
//     /'--......--'\
//     `"--......--"


    // Indexes inside the box
    int input_row = 8, input_col = 8;
    int x_padding = 3, y_padding = 0;
    int max_art_width = max(max(max(tweetie.width, derp_beaver.width), max(marge_simpson.width, homer.width)), max(goof_ah_elk.width, dome_guy.width));
    int max_art_height = max(max(max(tweetie.height, derp_beaver.height), max(marge_simpson.height, homer.height)), max(goof_ah_elk.height, dome_guy.height));
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    for (int row=1; row<=input_row; ++row) {
        for (int y_box_idx=0; y_box_idx <= max_art_height + 2*y_padding; ++y_box_idx) {
            for (int col=1; col<=input_col; ++col) {
                SetConsoleTextAttribute(hConsole, 15);
                cout << '|';
                // FIND PIECE
                uint64_t bitmask = ((uint64_t)1<<63) >> (8*(row-1)) >> (col-1);
                AsciiPiece art = blank;
                if (bitboard.knight & bitmask) 
                    art = goof_ah_elk;
                if (bitboard.bishop & bitmask)
                    art = dome_guy;
                if (bitboard.rook & bitmask)
                    art = derp_beaver;
                if (bitboard.queen & bitmask)
                    art = marge_simpson;
                if (bitboard.pawn & bitmask)
                    art = tweetie;
                if (bitboard.king & bitmask)
                    art = homer;
                // SET INDICES
                int start_x = ((max_art_width + 2*x_padding) - art.width) / 2;
                int end_x = start_x + art.width;
                int start_y = ((max_art_height + 2*y_padding) - art.height) / 2;
                int end_y = start_y + art.height;
                // LOOP
                for (int x_box_idx=1; x_box_idx <= max_art_width + 2*x_padding; ++x_box_idx) {
                    if (y_box_idx == 0) {
                        cout << '-';
                        continue;
                    }
                    if (y_box_idx == 1 & x_box_idx == 1) {
                        int square_index = log2(bitmask);
                        printf("%02d", square_index);
                        continue;
                    }
                    if (y_box_idx == 1 & x_box_idx == 2) {
                        continue;
                    }
                    if ((x_box_idx <= start_x) | (x_box_idx > end_x)) {
                        cout << ' ';
                        continue;
                    }
                    if ((y_box_idx <= start_y) | (y_box_idx > end_y)) {
                        cout << ' ';
                        continue;
                    }
                    // SET COLOR
                    if (bitboard.white & bitmask)
                        SetConsoleTextAttribute(hConsole, 11);
                    if (bitboard.black & bitmask)
                        SetConsoleTextAttribute(hConsole, 14);

                    int art_index = (y_box_idx - start_y - 1) * art.width + (x_box_idx - start_x - 1);
                    cout << art.art[art_index];
                }
            }
            SetConsoleTextAttribute(hConsole, 15);
            cout << '|';
            cout << endl;
        }
    }
    // LAST ROW
    for (int col=1; col<=8; ++col) {
        cout << '|';
        for (int idx_x_box=1; idx_x_box <= max_art_width + 2*x_padding; ++idx_x_box) {
            cout << '-';
        }
    }
    // LAST CHARACTER
    cout << '|' << endl;;
}
//  HELPER FUNCTIONS --> GENERATE MOVELIST VECTOR
uint64_t powers_of_two(uint16_t pow) {
    // base ^ pow
    uint64_t result = 1;
    while (pow > 0) {
        result <<= 1;
        --pow;
    }
    return result;
}
int row(uint64_t n) {
    return floor(log2(n))/8 + 1;
}
int col(uint64_t n) {
    return 8 - ((int)floor(log2(n)) % 8);
}
uint64_t shift(uint64_t a, int n) {
    // Perform the << bitwise operation. Allows for negative input
    // Negative nums rightshift
    if (n < 0) return a >> (-n);
    else return a << n;
}
//  GENERATE MOVELIST VECTOR
vector<uint16_t> rook_move(BitBoard bitboard, uint64_t bitmask) {
    // Initialize
    vector<uint16_t> movelist;
    uint64_t my_board = (bitboard.player_turn & bitboard.white) | ((~bitboard.player_turn) & bitboard.black);
    uint64_t your_board = ((~bitboard.player_turn) & bitboard.white) | ((bitboard.player_turn) & bitboard.black);
    uint64_t pos = bitmask;

    // Move up
    while ((bitmask << 8) != 0) {
        bitmask <<= 8;
        if (my_board & bitmask) break;
        uint16_t move = ((uint16_t) log2(pos)) << 10;
        move += ((uint16_t) log2(bitmask));
        // if (capture)
        if (your_board & bitmask) {
            move += (1 << 8);
            movelist.push_back(move);
            break;
        }
        movelist.push_back(move);
    }
    bitmask = pos;
    // Move down
    while ((bitmask >> 8) != 0) {
        bitmask >>= 8;
        if (my_board & bitmask) break;
        uint16_t move = ((uint16_t) log2(pos)) << 10;
        move += ((uint16_t) log2(bitmask));
        if (your_board & bitmask) {
            move += (1 << 8);
            movelist.push_back(move);
            break;
        }
        movelist.push_back(move);
    }
    bitmask = pos;
    // Move right
    while (row(bitmask >> 1) == row(pos)) {
        bitmask >>= 1;
        if (my_board & bitmask) break;
        uint16_t move = ((uint16_t) log2(pos)) << 10;
        move += ((uint16_t) log2(bitmask));
        if (your_board & bitmask) {
            move += (1 << 8);
            movelist.push_back(move);
            break;
        }
        movelist.push_back(move);
    }
    bitmask = pos;
    // Move left
    while (row(bitmask << 1) == row(pos)) {
        bitmask <<= 1;
        if (my_board & bitmask) break;
        uint16_t move = ((uint16_t) log2(pos)) << 10;
        move += ((uint16_t) log2(bitmask));
        if (your_board & bitmask) {
            move += (1 << 8);
            movelist.push_back(move);
            break;
        }
        movelist.push_back(move);
    }
    return movelist;
}
vector<uint16_t> bishop_move(BitBoard bitboard, uint64_t bitmask) {
    // Initialize
    vector<uint16_t> movelist;
    uint64_t my_board = (bitboard.player_turn & bitboard.white) | ((~bitboard.player_turn) & bitboard.black);
    uint64_t your_board = ((~bitboard.player_turn) & bitboard.white) | ((bitboard.player_turn) & bitboard.black);
    uint64_t pos = bitmask;

    // Move up right
    while (((bitmask << 7) != 0) & 
            (col(bitmask << 7) > col(pos))) {
        bitmask <<= 7;
        if (my_board & bitmask) break;
        uint16_t move = ((uint16_t) log2(pos)) << 10;
        move += ((uint16_t) log2(bitmask));
        // if (capture)
        if (your_board & bitmask) {
            move += (1 << 8);
            movelist.push_back(move);
            break;
        }
        movelist.push_back(move);
    }
    bitmask = pos;
    // Move up left
    while (((bitmask << 9) != 0) & 
            (col(bitmask << 9) < col(pos))) {
        bitmask <<= 9;
        if (my_board & bitmask) break;
        uint16_t move = ((uint16_t) log2(pos)) << 10;
        move += ((uint16_t) log2(bitmask));
        if (your_board & bitmask) {
            move += (1 << 8);
            movelist.push_back(move);
            break;
        }
        movelist.push_back(move);
    }
    bitmask = pos;
    // Move down left
    while (((bitmask >> 7) != 0) & 
            (col(bitmask >> 7) < col(pos))) {
        bitmask >>= 7;
        if (my_board & bitmask) break;
        uint16_t move = ((uint16_t) log2(pos)) << 10;
        move += ((uint16_t) log2(bitmask));
        if (your_board & bitmask) {
            move += (1 << 8);
            movelist.push_back(move);
            break;
        }
        movelist.push_back(move);
    }
    bitmask = pos;
    // Move down right
    while (((bitmask >> 9) != 0) & 
            (col(bitmask >> 9) > col(pos))) {
        bitmask >>= 9;
        if (my_board & bitmask) break;
        uint16_t move = ((uint16_t) log2(pos)) << 10;
        move += ((uint16_t) log2(bitmask));
        if (your_board & bitmask) {
            move += (1 << 8);
            movelist.push_back(move);
            break;
        }
        movelist.push_back(move);
    }
    return movelist;
}
vector<uint16_t> queen_move(BitBoard bitboard, uint64_t bitmask) {
    vector<uint16_t> movelist;
    // Temperarily edit our bitboards to say our queen is a rook
    bitboard.rook += bitmask;
    movelist = rook_move(bitboard, bitmask);
    bitboard.rook -= bitmask;

    bitboard.bishop += bitmask;
    vector<uint16_t> foo = bishop_move(bitboard, bitmask);
    movelist.insert(movelist.end(), foo.begin(), foo.end());
    bitboard.bishop -= bitmask;

    return movelist;
}
vector<uint16_t> knight_move(BitBoard bitboard, uint64_t bitmask) {
    vector<uint16_t> movelist;
    uint64_t my_board = (bitboard.player_turn & bitboard.white) | ((~bitboard.player_turn) & bitboard.black);
    uint64_t your_board = ((~bitboard.player_turn) & bitboard.white) | ((bitboard.player_turn) & bitboard.black);
    uint64_t pos = bitmask;
    int possible_squares[] = {-17, -15, -10, -6, 6, 10, 15, 17};
    for (int i = 0; i < 8; ++i) {
        bitmask = shift(pos, possible_squares[i]);
        if ((bitmask != 0) & abs(col(pos) - col(bitmask)) <= 2) {
            if (my_board & bitmask) continue;
            uint16_t move = ((uint16_t) log2(pos)) << 10;
            move += ((uint16_t) log2(bitmask));
            // if (capture)
            if (your_board & bitmask) {
                move += (1 << 8);
                movelist.push_back(move);
                continue;
            }
            movelist.push_back(move);
        }
        bitmask = pos;
    }
    return movelist;
}
vector<uint16_t> king_move(BitBoard bitboard, uint64_t bitmask) {
    /*
        Missing castling, check, move restrictions based on check.
        I will have a fork before these move methods are called. 
    */
    vector<uint16_t> movelist;
    uint64_t my_board = (bitboard.player_turn & bitboard.white) | ((~bitboard.player_turn) & bitboard.black);
    uint64_t your_board = ((~bitboard.player_turn) & bitboard.white) | ((bitboard.player_turn) & bitboard.black);
    uint64_t pos = bitmask;
    int possible_squares[] = {-9, -8, -7, -1, 1, 7, 8, 9};
    for (int i = 0; i < 8; ++i) {
        bitmask = shift(pos, possible_squares[i]);
        if ((bitmask != 0) & abs(col(pos) - col(bitmask)) <= 1) {
            if (my_board & bitmask) continue;
            uint16_t move = ((uint16_t) log2(pos)) << 10;
            move += ((uint16_t) log2(bitmask));
            // if (capture)
            if (your_board & bitmask) {
                move += (1 << 8);
                movelist.push_back(move);
                continue;
            }
            movelist.push_back(move);
        }
        bitmask = pos;
    }
    return movelist;
}
vector<uint16_t> pawn_move(BitBoard bitboard, uint64_t bitmask) {
    vector<uint16_t> movelist;
    uint64_t my_board = (bitboard.player_turn & bitboard.white) | ((~bitboard.player_turn) & bitboard.black);
    uint64_t your_board = ((~bitboard.player_turn) & bitboard.white) | ((bitboard.player_turn) & bitboard.black);
    uint64_t pos = bitmask;
    // Definition of playerTURN where: White = 1 and Black = -1
    char new_player_turn = (-2 * bitboard.player_turn) - 1;
    // Forward one square
    bitmask = shift(pos, new_player_turn * 8);
    if (~(bitboard.white | bitboard.black) & bitmask) {
        uint16_t move = (uint16_t)(log2(pos)) << 10;
        move += (uint16_t)(log2(bitmask));
        movelist.push_back(move);
        // Forward two squares
        bitmask = shift(pos, new_player_turn * 16);  
        if ((~(bitboard.white | bitboard.black) & bitmask) &
            (~(((5*bitboard.player_turn) + 7) == row(pos)) + 1)) {
            uint16_t move = (uint16_t)(log2(pos)) << 10;
            move += (uint16_t)(log2(bitmask));
            movelist.push_back(move);
        }
    }
    // Now check for pawn captures (non EnPassant)
    bitmask = shift(pos, new_player_turn * 7);
    if ((your_board & bitmask) &
        (~(abs(col(pos) - col(bitmask)) <= 1) + 1)) {
            uint16_t move = (uint16_t)(log2(pos)) << 10;
            move += (1 << 8);
            move += (uint16_t)(log2(bitmask));
            movelist.push_back(move);
        }
    bitmask = shift(pos, new_player_turn * 9);
    if ((your_board & bitmask) &
        (~(abs(col(pos) - col(bitmask)) <= 1) + 1)) {
            uint16_t move = (uint16_t)(log2(pos)) << 10;
            move += (1 << 8);
            move += (uint16_t)(log2(bitmask));
            movelist.push_back(move);
        }

    // TODO: En Passant
    // TODO: piece promotion
    return movelist;
}
vector<uint16_t> directory_generate_legal_moves(BitBoard bitboard) {
    vector<uint16_t> bitmoves;
    uint64_t bitmask = 1;
    vector<uint16_t> new_moves;
    while (bitmask != 0) {
        // If there's no valid piece on the current square skip to next loop
        if ((bitboard.player_turn & (bitboard.white & bitmask)) | 
            (~bitboard.player_turn & (bitboard.black & bitmask))) {
            if (bitboard.rook & bitmask)
                new_moves = (rook_move(bitboard, bitmask));
            else if (bitboard.bishop & bitmask) 
                new_moves = (bishop_move(bitboard, bitmask));
            else if (bitboard.queen & bitmask) 
                new_moves = (queen_move(bitboard, bitmask));
            else if (bitboard.king & bitmask) 
                new_moves = (king_move(bitboard, bitmask));
            else if (bitboard.pawn & bitmask) 
                new_moves = (pawn_move(bitboard, bitmask));
            else if (bitboard.knight & bitmask) 
                new_moves = (knight_move(bitboard, bitmask));
            // Append this move to bitmoves
            bitmoves.insert(bitmoves.end(), new_moves.begin(), new_moves.end());
            new_moves.clear();
            bitmask = bitmask << 1;
            continue;
        }
        bitmask = bitmask << 1;
    }
    return bitmoves;
}
//  MAKE MOVE
BitBoard make_move(BitBoard bitboard, uint16_t move) {
    // TODO: update "other" conditions inside BitBoard struct
    // Ex: castling rights, en passant square, 50-move draw, player_turn
    // Unpack move datatype
    uint16_t begin = move >> 10;
    uint16_t special = (move & 0b0000001111000000) >> 6;
    uint16_t end = move & 0b0000000000111111;
    // Switch player_turn
    bitboard.player_turn = ~bitboard.player_turn;
    // Create bitmasks
    uint64_t from_bitmask = powers_of_two(begin);
    uint64_t to_bitmask = powers_of_two(end);
    // CASTLING
    // TODO: implement me!
    if (special == 0b0010) {
        cout << "kingside castle" << endl;
        return bitboard;
    }
    if (special == 0b0011) {
        cout << "queenside castle" << endl;
        return bitboard;
    }
    // EN PASSANT CAPTURE
    // TODO: implement me!
    if (special == 0b0101) {
        // en passant capture
        cout << "en passant capture" << endl;
        return bitboard;
    }
    // PAWN PROMOTION
    // TODO: debug!
    if (special & 0b1000) {
        cout << "pawn promotion" << endl;
        
        // Remove piece on from_square
        bitboard.pawn = (bitboard.pawn) & ~(from_bitmask);
        if (bitboard.white & from_bitmask) {
            bitboard.white = (bitboard.white) & ~(from_bitmask);
            bitboard.white = bitboard.white | to_bitmask;
        }
        else if (bitboard.black & from_bitmask) {
            bitboard.black = (bitboard.black) & ~(from_bitmask);
            bitboard.black = bitboard.black | to_bitmask;
        }
        else {
            cout << "ERROR: white and black bitboards do not contain a piece on square: " << from_bitmask << endl;
        }

        if (special == 0b1000) {
            bitboard.knight = bitboard.knight | to_bitmask;
        }
        if (special == 0b1001) {
            bitboard.bishop = bitboard.bishop | to_bitmask;
        }
        if (special == 0b1010) {
            bitboard.rook = bitboard.rook | to_bitmask;
        }
        if (special == 0b1011) {
            bitboard.queen = bitboard.queen | to_bitmask;
        }
        if (special == 0b1100) {
            bitboard.knight = bitboard.knight | to_bitmask;
        }
        if (special == 0b1101) {
            bitboard.bishop = bitboard.bishop | to_bitmask;
        }
        if (special == 0b1110) {
            bitboard.rook = bitboard.rook | to_bitmask;
        }
        if (special == 0b1111) {
            bitboard.queen = bitboard.queen | to_bitmask;
        }
        return bitboard;
    }
    // QUIET MOVE OR SIMPLE CAPTURE
    if ((special == 0b0000) | (special == 0b0100)) {
        // DELETE captured piece
        if (special == 0b0100) {
            bitboard.white = (bitboard.white) & ~(to_bitmask);
            bitboard.black = (bitboard.black) & ~(to_bitmask);
            bitboard.pawn = (bitboard.pawn) & ~(to_bitmask);
            bitboard.rook = (bitboard.rook) & ~(to_bitmask);
            bitboard.knight = (bitboard.knight) & ~(to_bitmask);
            bitboard.bishop = (bitboard.bishop) & ~(to_bitmask);
            bitboard.queen = (bitboard.queen) & ~(to_bitmask);
            bitboard.king = (bitboard.king) & ~(to_bitmask);
        }
        // DELETE on from_square
        // ADD to to_square
        if (bitboard.white & from_bitmask) {
            bitboard.white = (bitboard.white) & ~(from_bitmask);
            bitboard.white = bitboard.white | to_bitmask;
        }
        else if (bitboard.black & from_bitmask) {
            bitboard.black = (bitboard.black) & ~(from_bitmask);
            bitboard.black = bitboard.black | to_bitmask;
        }
        else {
            cout << "ERROR: white and black bitboards do not contain a piece on square: " << from_bitmask << endl;
        }
        // Search for the piece type of the piece I'm going to move
        if (bitboard.pawn & from_bitmask) {
            bitboard.pawn = (bitboard.pawn) & ~(from_bitmask);
            bitboard.pawn = bitboard.pawn | to_bitmask;
        }
        else if (bitboard.bishop & from_bitmask) {
            bitboard.bishop = (bitboard.bishop) & ~(from_bitmask);
            bitboard.bishop = bitboard.bishop | to_bitmask;
        }
        else if (bitboard.rook & from_bitmask) {
            bitboard.rook = (bitboard.rook) & ~(from_bitmask);
            bitboard.rook = bitboard.rook | to_bitmask;
        }
        else if (bitboard.knight & from_bitmask) {
            bitboard.knight = (bitboard.knight) & ~(from_bitmask);
            bitboard.knight = bitboard.knight | to_bitmask;
        }
        else if (bitboard.queen & from_bitmask) {
            bitboard.queen = (bitboard.queen) & ~(from_bitmask);
            bitboard.queen = bitboard.queen | to_bitmask;
        }
        else if (bitboard.king & from_bitmask) {
            bitboard.king = (bitboard.king) & ~(from_bitmask);
            bitboard.king = bitboard.king | to_bitmask;
        }
        else {
            cout << "ERROR: no piece bitboard contains a piece on square: " << from_bitmask << endl;
        }
    }
    return bitboard;
}
uint16_t take_user_input_binary16(BitBoard bitboard) {
    char* user_input;
    cin >> user_input;
    // Build our bitmove 
    int index = 0;
    uint16_t bitmove = 0;
    for (uint16_t bitmask=(1<<15); bitmask!=0; bitmask>>=1) {
        if (user_input[index] == '1') {
            bitmove += bitmask;
        }
        index++;
    }
    return bitmove;
}
uint16_t take_user_input_int(BitBoard bitboard) {
    // Convert char into binary 16
    // 0-63
    // Check for special conditions
    // Build bitmove 16 bit
    char* user_input;
    cin >> user_input;
    cout << user_input;

    // strtok(user_input, " ");
    // char from_square = *user_input;
    // char to_square = *strtok(NULL, " ");

   

    uint16_t bitmove;
    return bitmove;
}
void run(BitBoard bitboard) {
    // Make a move: e4
    bitboard = make_move(bitboard, 0b0010110000011011);
    print_goofy_ahh_board(bitboard);
    // bitboard = make_move(bitboard, 0b1101110000101111);
    // print_goofy_ahh_board(bitboard);
    // bitboard = make_move(bitboard, 0b0010100000011010);
    // print_goofy_ahh_board(bitboard);
    // bitboard = make_move(bitboard, 0b0010010000011000);
    // print_goofy_ahh_board(bitboard);
    take_user_input_int(bitboard);
}
//  MAIN METHOD
int main() {
    // char fen[] = "r3k2r/pbpn1p2/1p1ppq1p/3P2p1/2P5/2P1PNP1/P1Q2PP1/3RKB1R w Kkq - 3 14";
    char fen[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    // DATA TYPE ASSIGNMENT
    FenString fen_string;
    fen_string.fen = fen;
    StandardRep standard_board = standard_from_fen_string(fen_string);
    BitBoard bitboard = bit_board_from_standard(standard_board);

    take_user_input_binary16(bitboard);
    
}


// Then take a png capture of that area of powershell terminal and save it as a file on my computer
        // Have user input on the rightside
