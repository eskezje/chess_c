#include "board.h"
#include <string.h>

int square_index(uint8_t rank, uint8_t file) {
    return (RANK_SHIFT*rank + file);
}

struct Chess_move notation_to_sqidx(const char *chess_move)  {
    uint8_t file0 = tolower(chess_move[0]) - 'a';
    uint8_t rank0 = chess_move[1] - '1';

    uint8_t file1 = tolower(chess_move[2]) - 'a';
    uint8_t rank1 = chess_move[3] - '1';

    struct Tuple_piece from_pcs = {file0, rank0};
    struct Tuple_piece to_pcs = {file1, rank1};

    struct Chess_move current_move = {from_pcs, to_pcs};

    return current_move;
}

void set_pawns(struct GameState *game) {
    for (int file = 0; file < 8; file++) {
        game -> board[square_index(1, file)] = PAWN;
    }

    for (int file = 0; file < 8; file++) {
        game -> board[square_index(6, file)] = -PAWN;
    }
}

void set_rest_white(struct GameState *game) {
    game -> board[square_index(0, 0)] = ROOK;
    game -> board[square_index(0, 7)] = ROOK;

    game -> board[square_index(0, 1)] = KNIGHT;
    game -> board[square_index(0, 6)] = KNIGHT;

    game -> board[square_index(0, 2)] = BISHOP;
    game -> board[square_index(0, 5)] = BISHOP;

    game -> board[square_index(0, 3)] = QUEEN;
    game -> board[square_index(0, 4)] = KING;
}

void set_rest_black(struct GameState *game) {
    game -> board[square_index(7, 0)] = -ROOK;
    game -> board[square_index(7, 7)] = -ROOK;

    game -> board[square_index(7, 1)] = -KNIGHT;
    game -> board[square_index(7, 6)] = -KNIGHT;

    game -> board[square_index(7, 2)] = -BISHOP;
    game -> board[square_index(7, 5)] = -BISHOP;

    game -> board[square_index(7, 3)] = -QUEEN;
    game -> board[square_index(7, 4)] = -KING;
}

char* piece_symbol(int8_t piece) {
    // unicode chess symbols as strings
    static char* white_pieces[] = {
        " ",
        "♟",
        "♞",
        "♝",
        "♜",
        "♛",
        "♚" 
    };
    
    static char* pieces[] = {
        " ",
        "♙",
        "♘",
        "♗",
        "♖",
        "♕",
        "♔"
    };
    
    int8_t abs_piece = abs(piece);
    
    if (piece > 0) {
        return white_pieces[abs_piece];
    } else if (piece < 0) {
        return pieces[abs_piece];
    } else {
        return " ";
    }
}

void print_board(struct GameState *game, char show_t) {
    printf("\n");
    
    if (show_t == 'n') {
        printf("  +---+---+---+---+---+---+---+---+\n");
        for (int i = 7; i >= 0; i--) {
            printf("%d |", (i+1));
            for (int k = 0; k < 8; k++) {
                printf(" %2d|", game -> board[square_index(i, k)]);
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
                printf(" %s |", piece_symbol(game -> board[square_index(i, k)]));
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

void move_piece(struct GameState *game) {
    int has_moved = 0;
    char chess_notation[16];

    while (has_moved < 1) {
        printf("Enter your move in chess notation (e.g., e2e4): ");
        if (!fgets(chess_notation, sizeof(chess_notation), stdin)) {
            printf("Error reading input.\n");
            continue;
        }
        size_t len = strcspn(chess_notation, "\n");
        if (chess_notation[len] == '\n') {
            chess_notation[len] = '\0';
        } else {
            int c; while ((c = getchar()) != '\n' && c != EOF) {}
            printf("Invalid notation. Use format: e2e4\n");
            continue;
        }
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
        uint8_t from_file = move.from.file;
        uint8_t from_rank = move.from.rank;
        int from_square = square_index(from_rank, from_file);
        
        // Check if source coordinates are valid - uint8_t is always >= 0
        if (from_rank > 7 || from_file > 7) {
            printf("Invalid source position. Try again.\n");
            continue;
        }
        
        // get piece at the source position
        int8_t p = game -> board[from_square];
        
        if (p == EMPTY) {
            printf("No piece at %c%c\n", chess_notation[0], chess_notation[1]);
            continue;
        }
        
        if ((p > 0 && game -> current_player < 0) || (p < 0 && game -> current_player > 0)) {
            printf("Not your piece to move!\n");
            continue;
        }
        
        if (abs(p) == PAWN) {
            if (move_pawn(game, move))    {
                has_moved = 1;
            }
        }
        else if (abs(p) == KNIGHT) {
            if (move_knight(game, move)) {
                has_moved = 1;
            }
        }
        else if (abs(p) == BISHOP) {
            if (move_bishop(game, move)) {
                has_moved = 1;
            }
        }
        else if (abs(p) == ROOK) {
            if (move_rook(game, move)){
                has_moved = 1;
            }
        }
        else if (abs(p) == QUEEN) {
            if (move_queen(game, move)) {
                has_moved = 1;
            }
        }
        else if (abs(p) == KING) {
            if (move_king(game, move)){
                has_moved = 1;
            }
            
        }
    }
}

void advance_round(struct GameState *game) {
    game -> current_player = -game -> current_player;
    printf("%s to move\n", game -> current_player > 0 ? "White" : "Black");
}