#include "board.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

static void reset_board(void) {
    memset(board, 0, sizeof(board));
}

int main(void) {
    int king_sq = square_index(4, 4);

    // pawn attack
    reset_board();
    board[king_sq] = KING;
    board[king_sq + 15] = -PAWN;
    assert(is_square_attacked(king_sq, -1));

    // knight attack
    reset_board();
    board[king_sq] = KING;
    board[king_sq + 33] = -KNIGHT;
    assert(is_square_attacked(king_sq, -1));

    // bishop attack
    reset_board();
    board[king_sq] = KING;
    board[king_sq + 17] = -BISHOP;
    assert(is_square_attacked(king_sq, -1));

    // rook attack
    reset_board();
    board[king_sq] = KING;
    board[king_sq + RANK_SHIFT] = -ROOK;
    assert(is_square_attacked(king_sq, -1));

    // queen attack
    reset_board();
    board[king_sq] = KING;
    board[king_sq + 1] = -QUEEN;
    assert(is_square_attacked(king_sq, -1));

    // king attack
    reset_board();
    board[king_sq] = KING;
    board[king_sq + 1] = -KING;
    assert(is_square_attacked(king_sq, -1));

    // move leaving king in check
    reset_board();
    board[king_sq] = KING;
    board[king_sq + 32] = -ROOK;
    board[king_sq + 16] = ROOK;
    current_player = 1;
    struct Chess_move mv = {{4,5}, {3,5}};
    assert(execute_move_piece(mv) == 0);
    assert(board[king_sq + 16] == ROOK);

    printf("All tests passed!\n");
    return 0;
}