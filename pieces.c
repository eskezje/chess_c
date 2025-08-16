#include "board.h"

int execute_move_piece(struct Chess_move player_move) {
    // extract source position
    int from_file = player_move.from.file;
    int from_rank = player_move.from.rank;
    int from_square = square_index(from_rank, from_file);
    
    // extract destination position
    int to_file = player_move.to.file;
    int to_rank = player_move.to.rank;
    int to_square = square_index(to_rank, to_file);
    
    // check if destination is off the board
    if (to_square & 0x88) { 
        printf("You cannot move it to that square! Try again\n");
        return 0;
    }
    
    // get the piece at the source square
    int piece = board[from_square];
    int target_piece = board[to_square];
    
    if (target_piece != EMPTY && 
        ((piece > 0 && target_piece > 0) || (piece < 0 && target_piece < 0))) {
        printf("Cannot capture your own piece!\n");
        return 0;
    }

    board[to_square] = piece;
    board[from_square] = EMPTY;
    
    // check for capture
    if (target_piece != EMPTY) {
        printf("You captured the enemies piece!\nThe piece has been moved from %c%c to %c%c\n", 
            'a' + from_file, '1' + from_rank, 
            'a' + to_file, '1' + to_rank);
    } else {
        printf("The piece has been moved from %c%c to %c%c\n", 
            'a' + from_file, '1' + from_rank, 
            'a' + to_file, '1' + to_rank);
    }

    return 1;
}

int is_path_clear(struct Chess_move player_move) {
    // extract source position
    int from_file = player_move.from.file;
    int from_rank = player_move.from.rank;
    int from_square = square_index(from_rank, from_file);
    
    // extract destination position
    int to_file = player_move.to.file;
    int to_rank = player_move.to.rank;
    int to_square = square_index(to_rank, to_file);   
    
    int height = to_rank - from_rank;
    int length = to_file - from_file;

    // calculate step directions
    int step_rank = 0;
    int step_file = 0;
    
    if (height != 0) {
        step_rank = (height > 0) ? RANK_SHIFT : -RANK_SHIFT;
    }
    
    if (length != 0) {
        step_file = (length > 0) ? 1 : -1;
    }
    
    // calculate total step between squares
    int step = step_rank + step_file;
    
    // check each square in the path (excluding destination)
    int current_square = from_square + step;
    
    while (current_square != to_square) {
        // check if we are off the board
        if (current_square & 0x88) {
            printf("Path goes off the board! Try again!\n");
            return 0;
        }
        
        // check if it is occupied
        if (board[current_square] != EMPTY) {
            printf("Path is blocked! Try again!\n");
            return 0;
        }
        
        // move to the next square
        current_square += step;
    }
    
    return 1;  // the path is clear
}

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
            found_move = execute_move_piece(player_move);
            break;
        } 
    }

    if (found_move) {
        advance_round();
    }

    if (!found_move) {
        printf("The move was not legal, try again\n");
    }
    return found_move;
}

int move_pawn(struct Chess_move player_move) {
    // flag for found move
    int found_move = 0;
    int move_len = 1;
    int distance = 0;
    int horizontal = 0;
    int capture = 0;

    // extract source position
    int from_file = player_move.from.file;
    int from_rank = player_move.from.rank;
    
    // extract destination position
    int to_file = player_move.to.file;
    int to_rank = player_move.to.rank;
    int to_square = square_index(to_rank, to_file);

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

    if (abs(horizontal) == 1 && abs(distance) == 1)   {
        capture = 1;
    }

    if (abs(distance) == 2 && !is_path_clear(player_move))
    {
        return 0;
    }

    //black pawn
    if (current_player < 0)  {
        if (distance > 0 &&                 // they are always moving downwards on the board
            capture == 0 &&                 // they are not allowed to move horizontally (yet)
            abs(distance) <= move_len &&    // they cant move further than what their move length is
            board[to_square] == EMPTY)   {  // the square they are moving to needs to be empty, if moving only vertically
                found_move = execute_move_piece(player_move);
        }

        else if (capture == 1 && distance == 1 && board[to_square] > 0)
        {
            found_move = execute_move_piece(player_move);
        }
        
    }

    //white pawn
    if (current_player > 0)  {
        if (distance < 0 &&                 // they are always moving upwards on the board
            capture == 0 &&                 // they are not allowed to move horizontally (yet)
            abs(distance) <= move_len &&    // they cant move further than what their move length is
            board[to_square] == EMPTY)   {  // the square they are moving to needs to be empty, if moving only vertically
                found_move = execute_move_piece(player_move);
        }

        else if (capture == 1 && distance == -1 && board[to_square] < 0)
        {
            found_move = execute_move_piece(player_move);
        }
    }

    if (found_move)    {
        advance_round();
    }

    if (!found_move) {
        printf("The move was not legal, try again\n");
    }

    return found_move;
}

int move_rook(struct Chess_move player_move) {
    // flag for found move
    int found_move = 0;

    // extract source position
    int from_file = player_move.from.file;
    int from_rank = player_move.from.rank;
    
    // extract destination position
    int to_file = player_move.to.file;
    int to_rank = player_move.to.rank;

    int distance = from_rank - to_rank;
    int horizontal = from_file - to_file;

    if (distance && horizontal)
    {
        printf("You cannot move the rook diagonally, try again!");
        return 0;
    }

    if (!is_path_clear(player_move))
    {
        return 0;
    }

    found_move = execute_move_piece(player_move);

    if (found_move) {
        advance_round();
    }

    if (!found_move) {
        printf("The move was not legal, try again\n");
    }
    return found_move;
}

int move_bishop(struct Chess_move player_move) {
    // flag for found move
    int found_move = 0;

    // extract source position
    int from_file = player_move.from.file;
    int from_rank = player_move.from.rank;
    
    // extract destination position
    int to_file = player_move.to.file;
    int to_rank = player_move.to.rank;

    int distance = from_rank - to_rank;
    int horizontal = from_file - to_file;

    if (abs(distance) != abs(horizontal))
    {
        printf("You can only move the Bishop diagonally, try again!");
        return 0;
    }

    if (!is_path_clear(player_move))
    {
        return 0;
    }

    found_move = execute_move_piece(player_move);

    if (found_move) {
        advance_round();
    }

    if (!found_move) {
        printf("The move was not legal, try again\n");
    }
    return found_move;
}


int move_queen(struct Chess_move player_move) {
    // flag for found move
    int found_move = 0;

    // extract source position
    int from_file = player_move.from.file;
    int from_rank = player_move.from.rank;
    
    // extract destination position
    int to_file = player_move.to.file;
    int to_rank = player_move.to.rank;

    int distance = from_rank - to_rank;
    int horizontal = from_file - to_file;

    if ((distance && horizontal) && (abs(distance) != abs(horizontal))) {
        printf("Queen move was invalid! The Queen can only move horizontal, vertical or diagonally! Try again!\n");
        return 0;
    }

    if (!is_path_clear(player_move))
    {
        return 0;
    }

    found_move = execute_move_piece(player_move);

    if (found_move) {
        advance_round();
    }

    if (!found_move) {
        printf("The move was not legal, try again\n");
    }
    return found_move;
}