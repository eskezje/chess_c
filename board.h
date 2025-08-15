#ifndef BOARD_H
#define BOARD_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

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
void move_piece(void);
void move_knight(struct Chess_move player_move);
struct Tuple_piece {
    int file;
    int rank;
};

struct Chess_move {
    struct Tuple_piece from;
    struct Tuple_piece to;
};

struct Chess_move notation_to_sqidx(const char *chess_move);

#endif