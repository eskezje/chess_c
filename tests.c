#include "board.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

static void reset_board(struct GameState *game) {
    memset(game -> board, 0, sizeof(game -> board));
}

int main(void) {
    struct GameState game;
    int king_sq = square_index(4, 4);

    // pawn attack
    reset_board(&game);
    game.board[king_sq] = KING;
    game.board[king_sq + 15] = -PAWN;
    assert(is_square_attacked(&game, king_sq, -1));

    // knight attack
    reset_board(&game);
    game.board[king_sq] = KING;
    game.board[king_sq + 33] = -KNIGHT;
    assert(is_square_attacked(&game, king_sq, -1));

    // bishop attack
    reset_board(&game);
    game.board[king_sq] = KING;
    game.board[king_sq + 17] = -BISHOP;
    assert(is_square_attacked(&game, king_sq, -1));

    // rook attack
    reset_board(&game);
    game.board[king_sq] = KING;
    game.board[king_sq + RANK_SHIFT] = -ROOK;
    assert(is_square_attacked(&game, king_sq, -1));

    // queen attack
    reset_board(&game);
    game.board[king_sq] = KING;
    game.board[king_sq + 1] = -QUEEN;
    assert(is_square_attacked(&game, king_sq, -1));

    // king attack
    reset_board(&game);
    game.board[king_sq] = KING;
    game.board[king_sq + 1] = -KING;
    assert(is_square_attacked(&game, king_sq, -1));

    // move leaving king in check
    reset_board(&game);
    game.board[king_sq] = KING;
    game.board[king_sq + 32] = -ROOK;
    game.board[king_sq + 16] = ROOK;
    game.current_player = 1;
    struct Chess_move mv = {{4,5}, {3,5}};
    assert(execute_move_piece(&game, mv) == 0);
    assert(game.board[king_sq + 16] == ROOK);

    printf("All tests passed!\n");
    return 0;
}