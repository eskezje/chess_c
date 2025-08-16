#include "board.h"
#include <string.h>

int board[128];
int current_player = 1;

int square_index(int rank, int file) {
    return (RANK_SHIFT*rank + file);
}

struct Chess_move notation_to_sqidx(const char *chess_move)  {
    int file0 = tolower(chess_move[0]) - 'a';
    int rank0 = chess_move[1] - '1';

    int file1 = tolower(chess_move[2]) - 'a';
    int rank1 = chess_move[3] - '1';

    struct Tuple_piece from_pcs = {file0, rank0};
    struct Tuple_piece to_pcs = {file1, rank1};

    struct Chess_move current_move = {from_pcs, to_pcs};

    return current_move;
}

char piece_char(int piece) {
    static const char pieces[] = ".pnbrqk";

    char c = pieces[abs(piece)];

    return (piece > 0) ? toupper(c) : c;
}

void set_pawns() {
    for (int file = 0; file < 8; file++) {
        board[square_index(1, file)] = PAWN;
    }

    for (int file = 0; file < 8; file++) {
        board[square_index(6, file)] = -PAWN;
    }
}

void set_rest_white() {
    board[square_index(0, 0)] = ROOK;
    board[square_index(0, 7)] = ROOK;

    board[square_index(0, 1)] = KNIGHT;
    board[square_index(0, 6)] = KNIGHT;

    board[square_index(0, 2)] = BISHOP;
    board[square_index(0, 5)] = BISHOP;

    board[square_index(0, 3)] = QUEEN;
    board[square_index(0, 4)] = KING;
}

void set_rest_black() {
    board[square_index(7, 0)] = -ROOK;
    board[square_index(7, 7)] = -ROOK;

    board[square_index(7, 1)] = -KNIGHT;
    board[square_index(7, 6)] = -KNIGHT;

    board[square_index(7, 2)] = -BISHOP;
    board[square_index(7, 5)] = -BISHOP;

    board[square_index(7, 3)] = -QUEEN;
    board[square_index(7, 4)] = -KING;
}

void print_board(char show_t) {
    printf("\n");
    
    if (show_t == 'n') {
        printf("  +---+---+---+---+---+---+---+---+\n");
        for (int i = 7; i >= 0; i--) {
            printf("%d |", (i+1));
            for (int k = 0; k < 8; k++) {
                printf(" %2d|", board[square_index(i, k)]);
            }
            printf("\n");
            printf("  +---+---+---+---+---+---+---+---+\n");
        }
    }
    else if (show_t == 'y') {
        printf("  +---+---+---+---+---+---+---+---+\n");
        for (int i = 7; i >= 0; i--) {
            printf("%d |", (i+1));
            for (int k = 0; k < 8; k++) {
                printf(" %c |", piece_char(board[square_index(i, k)]));
            }
            printf("\n");
            printf("  +---+---+---+---+---+---+---+---+\n");
        }
    }
    else {
        printf("Not a valid choice, use either y or n\n");
        return;
    }
    
    printf("    a   b   c   d   e   f   g   h\n");
    printf("\n");
}

void move_piece() {
    int has_moved = 0;
    char chess_notation[5];

    while (has_moved < 1) {
        printf("Enter your move in chess notation (e.g., e2e4): ");
        scanf("%4s", chess_notation);
        printf("\n");
        int c; while ((c = getchar()) != '\n' && c != EOF) {}
        if (strcmp(chess_notation, "quit") == 0) {
            printf("Game ended.\n");
            exit(0);
        }

        if (strlen(chess_notation) != 4 || 
            chess_notation[0] < 'a' || chess_notation[0] > 'h' ||
            chess_notation[2] < 'a' || chess_notation[2] > 'h' ||
            chess_notation[1] < '1' || chess_notation[1] > '8' ||
            chess_notation[3] < '1' || chess_notation[3] > '8') {
            printf("Invalid notation. Use format: e2e4\n");
            continue;
        }
        
        // notation to coords
        struct Chess_move move = notation_to_sqidx(chess_notation);
        
        // getting source position
        int from_file = move.from.file;
        int from_rank = move.from.rank;
        int from_square = square_index(from_rank, from_file);
        
        // Check if source coordinates are valid
        if (from_rank < 0 || from_rank > 7 || from_file < 0 || from_file > 7) {
            printf("Invalid source position. Try again.\n");
            continue;
        }
        
        // get piece at the source position
        int p = board[from_square];
        
        if (p == EMPTY) {
            printf("No piece at %c%c\n", chess_notation[0], chess_notation[1]);
            continue;
        }
        
        if ((p > 0 && current_player < 0) || (p < 0 && current_player > 0)) {
            printf("Not your piece to move!\n");
            continue;
        }

        /* printf("You are trying to move piece %c from %c%c to %c%c\n",
               piece_char(p), chess_notation[0], chess_notation[1], 
               chess_notation[2], chess_notation[3]); */
        
        // Call the appropriate move function based on piece type
        if (abs(p) == PAWN) {
            if (move_pawn(move))    {
                has_moved = 1;
            }
        }
        else if (abs(p) == KNIGHT) {
            if (move_knight(move)) {
                has_moved = 1;
            }
        }
        else if (abs(p) == BISHOP) {
            if (move_bishop(move)) {
                has_moved = 1;
            }
        }
        else if (abs(p) == ROOK) {
            if (move_rook(move)){
                has_moved = 1;
            }
        }
        else if (abs(p) == QUEEN) {
            // queen logic here
            printf("Not implemented yet, try again");
            continue;
        }
        else if (abs(p) == KING) {
            // king logic here
            printf("Not implemented yet, try again");
            continue;
        }
    }
}

void advance_round() {
    current_player = -current_player;
    printf("%s to move\n", current_player > 0 ? "White" : "Black");
}