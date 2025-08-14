#include "board.h"

int main() {
    set_pawns();
    set_rest_white();
    set_rest_black();
    print_board('y');
    return 0;
}