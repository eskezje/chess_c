#include "board.h"

int board[128];

int square_index(int rank, int file) {
    return (16*rank + file);
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
    if (show_t == 'n')
        for (int i = 7; i >= 0; i--) {
            printf("%d ", (i+1));
            for (int k = 0; k < 8; k++) {
                printf("%d ", board[square_index(i, k)]);
            }
            printf("\n");
        }
    else if (show_t == 'y')
        for (int i = 7; i >= 0; i--) {
            printf("%d ", (i+1));
            for (int k = 0; k < 8; k++) {
                printf("%c ", piece_char(board[square_index(i, k)]));
            }
            printf("\n");
        }
    else
        printf("Not a valid choice, use either y or n\n");
    printf("  a b c d e f g h\n");
}



void move_piece() {
    int has_moved = 0;
    int rank;
    int file;

    while (has_moved < 1) {
        printf("Enter the rank and the file of the piece you want to move in the notation: x, y \n");

        int ok = scanf(" %d , %d", &rank, &file);
        if (ok != 2) {
            printf("Invalid input format. Use: x, y\n");
            // flush the rest of the line
            int ch; while ((ch = getchar()) != '\n' && ch != EOF) {}
            continue;
        }

        if (rank < 0 || rank > 7 || file < 0 || file > 7) {
            printf("Out of bounds. Rank and file must be 0..7\n");
            continue;
        }

        int sq = square_index(rank, file);
        int p = board[sq];

        if (p == EMPTY) {
            printf("No piece on rank %d, file %d\n", rank, file);
            continue;
        }

        printf("You are trying to move piece %c on rank %d, and file %d\n",
               piece_char(p), rank, file);

        if (abs(p) == PAWN) {
            // pawn logic here
        }
        else if (abs(p) == KNIGHT) {
            // knight logic here
        }
        else if (abs(p) == BISHOP) {
            // bishop logic here
        }
        else if (abs(p) == ROOK) {
            // rook logic here
        }
        else if (abs(p) == QUEEN) {
            // queen logic here
        }
        else if (abs(p) == KING) {
            // king logic here
        }

        // set to 1 once a move actually succeeds
        // has_moved = 1;
    }
}