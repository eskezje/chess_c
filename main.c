#include "board.h"
#include <stdio.h>

int main() {
    set_pawns();
    set_rest_white();
    set_rest_black();
    
    printf("Chess Game Started\n");
    printf("%s to move\n", current_player > 0 ? "White" : "Black");
    
    while (1) {
        print_board('y');
        move_piece();
    }
    
    return 0;
}