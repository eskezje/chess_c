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
        
        // Check for checkmate before allowing move
        if (checkmate_check(&game, game.current_player)) {
            printf("Checkmate! %s wins!\n", game.current_player > 0 ? "Black" : "White");
            break;
        }
        
        // Check if current player is in check
        int king_sq = find_king(&game, game.current_player);
        if (king_sq != -1 && is_square_attacked(&game, king_sq, -game.current_player)) {
            printf("Check!\n");
        }
        
        move_piece(&game);
    }

    return 0;
}