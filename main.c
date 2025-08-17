#include "board.h"
#include <stdio.h>

int main(void) {
    struct GameState game = { .current_player = 1 };

    set_pawns(&game);
    set_rest_white(&game);
    set_rest_black(&game);

    printf("Chess Game Started\n");
    printf("%s to move\n", game.current_player > 0 ? "White" : "Black");

    while (1) {
        print_board(&game, 'y');
        move_piece(&game);
    }

    return 0;
}