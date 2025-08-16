#include "board.h"

int move_knight(struct Chess_move player_move) {
    // flag for found move
    int found_move = 0;

    // extract source position
    int from_file = player_move.from.file;
    int from_rank = player_move.from.rank;
    int from_square = square_index(from_rank, from_file);
    
    // extract destination position
    int to_file = player_move.to.file;
    int to_rank = player_move.to.rank;
    int to_square = square_index(to_rank, to_file);
    
    // get the piece at the source square
    int piece = board[from_square];

    int knight_moves[8];

    //move up
    knight_moves[0] = from_square+(2*RANK_SHIFT)-1;
    knight_moves[1] = from_square+(2*RANK_SHIFT)+1;
    
    // move down
    knight_moves[2] = from_square-(2*RANK_SHIFT)-1;
    knight_moves[3] = from_square-(2*RANK_SHIFT)+1;    
    
    // move right
    knight_moves[4] = from_square+(1*RANK_SHIFT)+2;
    knight_moves[5] = from_square-(1*RANK_SHIFT)+2;
    
    // move left
    knight_moves[6] = from_square+(1*RANK_SHIFT)-2;
    knight_moves[7] = from_square-(1*RANK_SHIFT)-2;


    for (int i = 0; i<8; i++){
        if (to_square == knight_moves[i] && !(knight_moves[i] & 0x88)) {
            // make sure we dont go full on kamikaze
            int target_piece = board[to_square];
            if (target_piece != EMPTY && 
                ((piece > 0 && target_piece > 0) || (piece < 0 && target_piece < 0))) {
                printf("Cannot capture your own piece!\n");
                return 0;
            }
            
            move_actual_piece(player_move);
            found_move += 1;
            advance_round();
            break;
        } 
    }

    if (!found_move) {
        printf("The move was not legal, try again\n");
    }
    return found_move ? 1 : 0;
}

int move_pawn(struct Chess_move player_move) {
    // flag for found move
    int found_move = 0;
    int move_len = 1;
    int distance = 0;
    int horizontal = 0;

    // extract source position
    int from_file = player_move.from.file;
    int from_rank = player_move.from.rank;
    int from_square = square_index(from_rank, from_file);
    
    // extract destination position
    int to_file = player_move.to.file;
    int to_rank = player_move.to.rank;
    int to_square = square_index(to_rank, to_file);
    
    // get the piece at the source square
    int piece = board[from_square];

    // blank pawns
    if (from_rank == 6 && current_player < 0)    {
        move_len = 2;
    }
    // white pawns
    if (from_rank == 1 && current_player > 0)    {
        move_len = 2;
    }

    distance = from_rank - to_rank;
    horizontal = from_file - to_file;

    //black pawn
    if (current_player < 0)  {
        if (distance < 0 &&                 // they are always moving downwards on the board
            horizontal == 0 &&              // they are not allowed to move horizontally (yet)
            abs(distance) <= move_len &&    // they cant move further than what their move length is
            board[to_square] == EMPTY)   {  // the square they are moving to needs to be empty, if moving only vertically
                move_actual_piece(player_move);
                found_move += 1;
                advance_round();
        }
    }

    //black pawn
    if (current_player > 0)  {
        if (distance > 0 &&                 // they are always moving downwards on the board
            horizontal == 0 &&              // they are not allowed to move horizontally (yet)
            abs(distance) <= move_len &&    // they cant move further than what their move length is
            board[to_square] == EMPTY)   {  // the square they are moving to needs to be empty, if moving only vertically
                move_actual_piece(player_move);
                found_move += 1;
                advance_round();
        }
    }

    if (!found_move) {
        printf("The move was not legal, try again\n");
    }

    return found_move ? 1 : 0;
}

void print_has_move(struct Chess_move player_move)  {
    int from_file = player_move.from.file;
    int from_rank = player_move.from.rank;
    
    int to_file = player_move.to.file;
    int to_rank = player_move.to.rank;
    printf("The piece has been moved from %c%c to %c%c\n", 
        'a' + from_file, '1' + from_rank, 
        'a' + to_file, '1' + to_rank);
}

void move_actual_piece(struct Chess_move player_move)   {
    // extract source position
    int from_file = player_move.from.file;
    int from_rank = player_move.from.rank;
    int from_square = square_index(from_rank, from_file);
    
    // extract destination position
    int to_file = player_move.to.file;
    int to_rank = player_move.to.rank;
    int to_square = square_index(to_rank, to_file);
    
    // get the piece at the source square
    int piece = board[from_square];

    board[to_square] = piece;
    board[from_square] = EMPTY;
    print_has_move(player_move);
}