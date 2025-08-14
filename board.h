#ifndef BOARD_H
#define BOARD_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define EMPTY   0
#define PAWN    1
#define KNIGHT  2
#define BISHOP  3
#define ROOK    4
#define QUEEN   5
#define KING    6

extern int board[128];

int square_index(int rank, int file);
char piece_char(int piece);
void set_pawns();
void set_rest_white();
void set_rest_black();
void print_board(char show_t);

struct Tuple_piece {
    int idx;
    int rnk;
};

struct Chess_move {
    Tuple_piece from;
    Tuple_piece to;
};

#endif