#include "board.h"

int find_king(struct GameState *game, int8_t color) {
    for (int sq = 0; sq < 128; ++sq) {
        if (sq & 0x88) {
            continue;
        }
        if (game -> board[sq] == color * KING) {
            return sq;
        }
    }
    return -1;
}

int is_square_attacked(struct GameState *game, int sq, int8_t by_color) {
    int8_t piece;

    int pawn_offsets[2];
    if (by_color > 0) {
        pawn_offsets[0] = -17;
        pawn_offsets[1] = -15;
    } else {
        pawn_offsets[0] = 15;
        pawn_offsets[1] = 17;
    }
    for (int i = 0; i < 2; ++i) {
        int p_sq = sq + pawn_offsets[i];
        if (!(p_sq & 0x88) && game -> board[p_sq] == by_color * PAWN) {
            return 1;
        }
    }

    int knight_offsets[8] = {33, 31, 18, 14, -14, -18, -31, -33};
    for (int i = 0; i < 8; ++i) {
        int n_sq = sq + knight_offsets[i];
        if (!(n_sq & 0x88) && game -> board[n_sq] == by_color * KNIGHT) {
            return 1;
        }
    }

    int rook_dirs[4] = {1, -1, RANK_SHIFT, -RANK_SHIFT};
    for (int i = 0; i < 4; ++i) {
        int r_sq = sq + rook_dirs[i];
        while (!(r_sq & 0x88)) {
            piece = game -> board[r_sq];
            if (piece != EMPTY) {
                if (piece * by_color > 0 && (abs(piece) == ROOK || abs(piece) == QUEEN)) {
                    return 1;
                }
                break;
            }
            r_sq += rook_dirs[i];
        }
    }

    int bishop_dirs[4] = {17, -17, 15, -15};
    for (int i = 0; i < 4; ++i) {
        int b_sq = sq + bishop_dirs[i];
        while (!(b_sq & 0x88)) {
            piece = game -> board[b_sq];
            if (piece != EMPTY) {
                if (piece * by_color > 0 && (abs(piece) == BISHOP || abs(piece) == QUEEN)) {
                    return 1;
                }
                break;
            }
            b_sq += bishop_dirs[i];
        }
    }

    int king_offsets[8] = {1, -1, RANK_SHIFT, -RANK_SHIFT, 17, -17, 15, -15};
    for (int i = 0; i < 8; ++i) {
        int k_sq = sq + king_offsets[i];
        if (!(k_sq & 0x88) && game -> board[k_sq] == by_color * KING) {
            return 1;
        }
    }

    return 0;
}

int execute_move_piece(struct GameState *game, struct Chess_move player_move) {
    // extract source position
    uint8_t from_file = player_move.from.file;
    uint8_t from_rank = player_move.from.rank;
    int from_square = square_index(from_rank, from_file);
    
    // extract destination position
    uint8_t to_file = player_move.to.file;
    uint8_t to_rank = player_move.to.rank;
    int to_square = square_index(to_rank, to_file);
    
    // ensure the piece is actually moving
    if (from_square == to_square) {
        printf("You must move to a different square! Try again\n");
        return 0;
    }

    // check if destination is off the board
    if (to_square & 0x88) { 
        printf("You cannot move it to that square! Try again\n");
        return 0;
    }
    
    // get the piece at the source square
    int8_t piece = game -> board[from_square];
    int8_t target_piece = game -> board[to_square];
    
    if (target_piece != EMPTY && 
        ((piece > 0 && target_piece > 0) || (piece < 0 && target_piece < 0))) {
        printf("Cannot capture your own piece!\n");
        return 0;
    }


    game -> board[to_square] = piece;
    game -> board[from_square] = EMPTY;

    int king_sq = find_king(game, game -> current_player);
    if (king_sq != -1 && is_square_attacked(game, king_sq, -game -> current_player)) {
        game -> board[from_square] = piece;
        game -> board[to_square] = target_piece;
        printf("You cannot leave your king in check! Try again\n");
        return 0;
    }

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

int is_path_clear(struct GameState *game, struct Chess_move player_move) {
    // extract source position
    uint8_t from_file = player_move.from.file;
    uint8_t from_rank = player_move.from.rank;
    int from_square = square_index(from_rank, from_file);
    
    // extract destination position
    uint8_t to_file = player_move.to.file;
    uint8_t to_rank = player_move.to.rank;
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
        if (game -> board[current_square] != EMPTY) {
            printf("Path is blocked! Try again!\n");
            return 0;
        }
        
        // move to the next square
        current_square += step;
    }
    
    return 1;  // the path is clear
}

int move_knight(struct GameState *game, struct Chess_move player_move) {
    // flag for found move
    int found_move = 0;

    // extract source position
    uint8_t from_file = player_move.from.file;
    uint8_t from_rank = player_move.from.rank;
    int from_square = square_index(from_rank, from_file);
    
    // extract destination position
    uint8_t to_file = player_move.to.file;
    uint8_t to_rank = player_move.to.rank;
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
            found_move = execute_move_piece(game, player_move);
            break;
        } 
    }

    if (found_move) {
        advance_round(game);
    }

    if (!found_move) {
        printf("The move was not legal, try again\n");
    }
    return found_move;
}

int move_pawn(struct GameState *game, struct Chess_move player_move) {
    // flag for found move
    int found_move = 0;
    int move_len = 1;
    int distance = 0;
    int horizontal = 0;
    int capture = 0;

    // extract source position
    uint8_t from_file = player_move.from.file;
    uint8_t from_rank = player_move.from.rank;
    
    // extract destination position
    uint8_t to_file = player_move.to.file;
    uint8_t to_rank = player_move.to.rank;
    int to_square = square_index(to_rank, to_file);

    // blank pawns
    if (from_rank == 6 && game -> current_player < 0)    {
        move_len = 2;
    }
    // white pawns
    if (from_rank == 1 && game -> current_player > 0)    {
        move_len = 2;
    }

    distance = from_rank - to_rank;
    horizontal = from_file - to_file;

    if (abs(horizontal) == 1 && abs(distance) == 1)   {
        capture = 1;
    }

    if (abs(distance) == 2 && !is_path_clear(game, player_move))
    {
        return 0;
    }

    //black pawn
    if (game -> current_player < 0)  {
        if (distance > 0 &&                 // they are always moving downwards on the board
            capture == 0 &&                 // they are not allowed to move horizontally (yet)
            abs(distance) <= move_len &&    // they cant move further than what their move length is
            game -> board[to_square] == EMPTY)   {  // the square they are moving to needs to be empty, if moving only vertically
                found_move = execute_move_piece(game, player_move);
        }

        else if (capture == 1 && distance == 1 && game -> board[to_square] > 0)
        {
            found_move = execute_move_piece(game, player_move);
        }
        
    }

    //white pawn
    if (game -> current_player > 0)  {
        if (distance < 0 &&                 // they are always moving upwards on the board
            capture == 0 &&                 // they are not allowed to move horizontally (yet)
            abs(distance) <= move_len &&    // they cant move further than what their move length is
            game -> board[to_square] == EMPTY)   {  // the square they are moving to needs to be empty, if moving only vertically
                found_move = execute_move_piece(game, player_move);
        }

        else if (capture == 1 && distance == -1 && game -> board[to_square] < 0)
        {
            found_move = execute_move_piece(game, player_move);
        }
    }

    if (found_move)    {
        advance_round(game);
    }

    if (!found_move) {
        printf("The move was not legal, try again\n");
    }

    return found_move;
}

int move_rook(struct GameState *game, struct Chess_move player_move) {
    // flag for found move
    int found_move = 0;

    // extract source position
    uint8_t from_file = player_move.from.file;
    uint8_t from_rank = player_move.from.rank;
    
    // extract destination position
    uint8_t to_file = player_move.to.file;
    uint8_t to_rank = player_move.to.rank;

    int distance = from_rank - to_rank;
    int horizontal = from_file - to_file;

    if (distance && horizontal)
    {
        printf("You cannot move the rook diagonally, try again!");
        return 0;
    }

    if (!is_path_clear(game, player_move))
    {
        return 0;
    }

    found_move = execute_move_piece(game, player_move);

    if (found_move) {
        advance_round(game);
    }

    if (!found_move) {
        printf("The move was not legal, try again\n");
    }
    return found_move;
}

int move_bishop(struct GameState *game, struct Chess_move player_move) {
    // flag for found move
    int found_move = 0;

    // extract source position
    uint8_t from_file = player_move.from.file;
    uint8_t from_rank = player_move.from.rank;
    
    // extract destination position
    uint8_t to_file = player_move.to.file;
    uint8_t to_rank = player_move.to.rank;

    int distance = from_rank - to_rank;
    int horizontal = from_file - to_file;

    if (abs(distance) != abs(horizontal))
    {
        printf("You can only move the Bishop diagonally, try again!");
        return 0;
    }

    if (!is_path_clear(game, player_move))
    {
        return 0;
    }

    found_move = execute_move_piece(game, player_move);

    if (found_move) {
        advance_round(game);
    }

    if (!found_move) {
        printf("The move was not legal, try again\n");
    }
    return found_move;
}


int move_queen(struct GameState *game, struct Chess_move player_move) {
    // flag for found move
    int found_move = 0;

    // extract source position
    uint8_t from_file = player_move.from.file;
    uint8_t from_rank = player_move.from.rank;
    
    // extract destination position
    uint8_t to_file = player_move.to.file;
    uint8_t to_rank = player_move.to.rank;

    int distance = from_rank - to_rank;
    int horizontal = from_file - to_file;

    if ((distance && horizontal) && (abs(distance) != abs(horizontal))) {
        printf("Queen move was invalid! The Queen can only move horizontal, vertical or diagonally! Try again!\n");
        return 0;
    }

    if (!is_path_clear(game, player_move))
    {
        return 0;
    }

    found_move = execute_move_piece(game, player_move);

    if (found_move) {
        advance_round(game);
    }

    if (!found_move) {
        printf("The move was not legal, try again\n");
    }
    return found_move;
}

int move_king(struct GameState *game, struct Chess_move player_move) {
    // flag for found move
    int found_move = 0;

    // extract source position
    uint8_t from_file = player_move.from.file;
    uint8_t from_rank = player_move.from.rank;
    
    // extract destination position
    uint8_t to_file = player_move.to.file;
    uint8_t to_rank = player_move.to.rank;

    int distance = from_rank - to_rank;
    int horizontal = from_file - to_file;

    if (abs(distance) > 1 || abs(horizontal) > 1)  {
        printf("The king cant move more than 1 square in any direction! Try again\n");
        return 0;
    }

    if (!is_path_clear(game, player_move))
    {
        return 0;
    }

    found_move = execute_move_piece(game, player_move);

    if (found_move) {
        advance_round(game);
    }

    if (!found_move) {
        printf("The move was not legal, try again\n");
    }
    return found_move;
}