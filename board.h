#ifndef BOARD_H
#define BOARD_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <stdint.h>  

#define RANK_SHIFT 16

#define EMPTY   0
#define PAWN    1
#define KNIGHT  2
#define BISHOP  3
#define ROOK    4
#define QUEEN   5
#define KING    6

struct GameState {
    int8_t board[128];
    int8_t current_player;
};

struct Tuple_piece {
    uint8_t file;
    uint8_t rank;
};

struct Chess_move {
    struct Tuple_piece from;
    struct Tuple_piece to;
};

char* piece_symbol(int8_t piece);
int square_index(uint8_t rank, uint8_t file);
void set_pawns(struct GameState *game);
void set_rest_white(struct GameState *game);
void set_rest_black(struct GameState *game);
void print_board(struct GameState *game, char show_t);
void move_piece(struct GameState *game);
void advance_round(struct GameState *game);

int move_knight(struct GameState *game, struct Chess_move player_move);
int move_pawn(struct GameState *game, struct Chess_move player_move);
int move_rook(struct GameState *game, struct Chess_move player_move);
int move_bishop(struct GameState *game, struct Chess_move player_move);
int move_queen(struct GameState *game, struct Chess_move player_move);
int move_king(struct GameState *game, struct Chess_move player_move);

int checkmate_check(struct GameState *game, int8_t color);
int check_surrounding_free(struct GameState *game, int sq);
int execute_move_piece(struct GameState *game, struct Chess_move player_move);
int is_path_clear(struct GameState *game, struct Chess_move player_move);
int find_king(struct GameState *game, int8_t color);
int is_square_attacked(struct GameState *game, int sq, int8_t by_color);

struct Chess_move notation_to_sqidx(const char *chess_move);

#endif
