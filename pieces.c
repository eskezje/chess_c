#include "board.h"
#include <stdint.h>

int check_legal_moves(struct GameState *game, int8_t color) {
    // current implementation only sees if there is one legal move
    // can very easily change it to count all possible moves
    // but we want to spare a bit of the resources, keep it fast
    int total_legal_moves = 0;
    int pawn_moves[4];
    int bishop_dirs[4] = {17, -17, 15, -15};
    int knight_offsets[8] = {33, 31, 18, 14, -14, -18, -31, -33};
    int rook_dirs[4] = {1, -1, RANK_SHIFT, -RANK_SHIFT};
    int queen_dirs[8] = {1, -1, RANK_SHIFT, -RANK_SHIFT, 17, -17, 15, -15};


    for (int sq = 0; sq < 128; sq++) {
        if (sq & 0x88) continue;
        if (total_legal_moves) {
            break;
        }
        int8_t p = game -> board[sq];
        int8_t our_piece = color_of(p);
        if (our_piece == color) {
            // now we need to check if it can move
            int abs_piece = abs(p);
            switch (abs_piece) {
            case PAWN:
                if (color > 0) {    // white pawn
                    pawn_moves[0] = sq - RANK_SHIFT;        // forward 1
                    pawn_moves[1] = sq - 2*RANK_SHIFT;      // forward 2
                    pawn_moves[2] = sq - RANK_SHIFT + 1;    // capture right
                    pawn_moves[3] = sq - RANK_SHIFT - 1;    // capture left
                } else {            // black pawn
                    pawn_moves[0] = sq + RANK_SHIFT;        // forward 1
                    pawn_moves[1] = sq + 2*RANK_SHIFT;      // forward 2
                    pawn_moves[2] = sq + RANK_SHIFT + 1;    // capture right
                    pawn_moves[3] = sq + RANK_SHIFT - 1;    // capture left
                }
                
                for (int i = 0; i < 4; i++) {
                    int target_sq = pawn_moves[i];
                    if (!(target_sq & 0x88) && can_piece_move_to(game, sq, target_sq, p)) {
                        total_legal_moves += 1;
                        break;
                    }
                }
                break;
                case BISHOP:
                    for (int i = 0; i < 4; ++i)    {
                        int b_sq = sq + bishop_dirs[i];
                        while (!(b_sq & 0x88))  {
                            if (can_piece_move_to(game, sq, b_sq, p)) {
                                total_legal_moves += 1;
                                break;
                            }
                            b_sq += bishop_dirs[i];
                        }
                    }   
                    break;
                case KNIGHT:
                        for (int i = 0; i < 8; ++i) {
                            int n_sq = sq + knight_offsets[i];
                            if (!(n_sq & 0x88) && can_piece_move_to(game, sq, n_sq, p)) {
                                    total_legal_moves += 1;
                                    break;
                                }
                        }
                        break;
                case ROOK:
                        for (int i = 0; i < 4; ++i) {
                            int r_sq = sq + rook_dirs[i];
                            while (!(r_sq & 0x88)) {
                                if (can_piece_move_to(game, sq, r_sq, p)) {
                                    total_legal_moves += 1;
                                    break;
                                }
                                r_sq += rook_dirs[i];
                            }
                        }
                        break;
                case QUEEN:
                    for (int i = 0; i < 8; ++i) {
                        int q_sq = sq + queen_dirs[i];
                        while (!(q_sq & 0x88)) {
                            if (can_piece_move_to(game, sq, q_sq, p)) {
                                total_legal_moves += 1;
                                break;
                            }
                            q_sq += queen_dirs[i];
                        }
                    }
                    break;
            }
        }
    }

    return total_legal_moves;
}

int stalemate_check(struct GameState *game, int8_t color) {
    int king_sq = find_king(game, color);
    if (king_sq == -1) return 0; // no king is found 
    int sq_attack = is_square_attacked(game, king_sq, -color); // the king cannt be attacked to have a stalemate 
    int king_escape = can_king_escape(game, king_sq, color); // if the king can escape then there is no stalemate_check
    int any_legal_moves = check_legal_moves(game, color);
    // now we need to check if any other pieces of the color can move
    if (sq_attack == 0 &&
        king_escape == 0 &&
        any_legal_moves == 0) {
        return 1;   // then the king isnt attacked, it cant move and no other pieces has a legal move.
    }
    return 0;       // there was a legal move 
}

int checkmate_check(struct GameState *game, int8_t color) {
    int king_sq = find_king(game, color);
    if (king_sq == -1) return 0; // no king is found
    
    // first check if king is actually in check
    if (!is_square_attacked(game, king_sq, -color)) {
        return 0; // the king is not in check, so not checkmate
    }
    
    // check if king can move to safety
    if (can_king_escape(game, king_sq, color)) {
        return 0; // the king can escape
    }
    
    // check if any piece can block or capture the attacker
    if (can_defend_king(game, king_sq, color)) {
        return 0; // can defend
    }
    
    return 1; // CHECKMATE
}

int can_king_escape(struct GameState *game, int king_sq, int8_t color) {
    int king_moves[8] = {1, -1, RANK_SHIFT, -RANK_SHIFT, 17, -17, 15, -15};
    
    for (int i = 0; i < 8; i++) {
        int escape_sq = king_sq + king_moves[i];
        
        if (escape_sq & 0x88) continue;
        
        // check if square is empty or contains enemy piece
        int8_t target = game->board[escape_sq];
        if (target != EMPTY && target * color > 0) continue; // own piece
        
        // temp make the move to test if king would still be in check
        int8_t original_piece = game->board[escape_sq];
        game->board[escape_sq] = color * KING;
        game->board[king_sq] = EMPTY;
        
        int still_attacked = is_square_attacked(game, escape_sq, -color);
        
        // restore board
        game->board[king_sq] = color * KING;
        game->board[escape_sq] = original_piece;
        
        if (!still_attacked) {
            return 1; // found escape square
        }
    }
    return 0; // no escapes
}

int can_defend_king(struct GameState *game, int king_sq, int8_t color) {
    // for each square on the board check if any of our pieces can move there
    // to either block the attack or capture the attacker
    for (int sq = 0; sq < 128; sq++) {
        if (sq & 0x88) continue;
        
        // check if moving a piece to this square would help
        if (can_piece_defend_square(game, sq, king_sq, color)) {
            return 1; // we can defend the square
        }
    }
    return 0; // we cannot defend the square
}

int can_piece_defend_square(struct GameState *game, int target_sq, int king_sq, int8_t color) {
    // check if any of our pieces can move to the target square to defend the king
    for (int from_sq = 0; from_sq < 128; from_sq++) {
        if (from_sq & 0x88) continue;
        
        int8_t piece = game->board[from_sq];
        if (piece == EMPTY || piece * color <= 0 || abs(piece) == KING) continue;
        
        // check if this piece can legally move to the target square
        if (can_piece_move_to(game, from_sq, target_sq, piece)) {
            // temp make the move and check if king is still in check
            int8_t original_target = game->board[target_sq];
            game->board[target_sq] = piece;
            game->board[from_sq] = EMPTY;
            
            int king_still_attacked = is_square_attacked(game, king_sq, -color);
            
            // restore board
            game->board[from_sq] = piece;
            game->board[target_sq] = original_target;
            
            if (!king_still_attacked) {
                return 1; // WE SAVE THE KING
            }
        }
    }
    return 0;
}

int can_piece_move_to(struct GameState *game, int from_sq, int to_sq, int8_t piece) {
    // check if piece can legally move from from_sq to to_sq
    int8_t target = game->board[to_sq];
    
    // we cant capture our own pieces
    if (target != EMPTY && target * piece > 0) return 0;
    
    int abs_piece = abs(piece);
    int from_rank = from_sq / RANK_SHIFT;
    int from_file = from_sq % RANK_SHIFT;
    int to_rank = to_sq / RANK_SHIFT;
    int to_file = to_sq % RANK_SHIFT;
    
    struct Chess_move test_move = {{from_file, from_rank}, {to_file, to_rank}};
    
    switch (abs_piece) {
        case PAWN:
            return is_valid_pawn_move(game, test_move, piece);
        case KNIGHT:
            return is_valid_knight_move(from_sq, to_sq);
        case BISHOP:
            return is_valid_bishop_move(from_sq, to_sq) && is_path_clear(game, test_move);
        case ROOK:
            return is_valid_rook_move(from_sq, to_sq) && is_path_clear(game, test_move);
        case QUEEN:
            return is_valid_queen_move(from_sq, to_sq) && is_path_clear(game, test_move);
    }
    return 0;
}

int is_valid_pawn_move(struct GameState *game, struct Chess_move move, int8_t piece) {
    int from_rank = move.from.rank;
    int to_rank = move.to.rank;
    int from_file = move.from.file;
    int to_file = move.to.file;
    int to_sq = square_index(to_rank, to_file);
    
    int distance = from_rank - to_rank;
    int horizontal = abs(from_file - to_file);
    
    if (piece > 0) { // white pawn
        if (distance < 0 && horizontal == 0 && game->board[to_sq] == EMPTY) {
            return (abs(distance) == 1) || (from_rank == 1 && abs(distance) == 2);
        }
        if (distance == -1 && horizontal == 1 && game->board[to_sq] < 0) {
            return 1; // capture
        }
    } else { // black pawn
        if (distance > 0 && horizontal == 0 && game->board[to_sq] == EMPTY) {
            return (abs(distance) == 1) || (from_rank == 6 && abs(distance) == 2);
        }
        if (distance == 1 && horizontal == 1 && game->board[to_sq] > 0) {
            return 1; // capture
        }
    }
    return 0;
}

int is_valid_knight_move(int from_sq, int to_sq) {
    int knight_moves[8] = {33, 31, 18, 14, -14, -18, -31, -33};
    for (int i = 0; i < 8; i++) {
        if (from_sq + knight_moves[i] == to_sq) {
            return !(to_sq & 0x88);
        }
    }
    return 0;
}

int is_valid_bishop_move(int from_sq, int to_sq) {
    int diff = to_sq - from_sq;
    return (abs(diff) % 17 == 0 || abs(diff) % 15 == 0);
}

int is_valid_rook_move(int from_sq, int to_sq) {
    int diff = to_sq - from_sq;
    return (diff % RANK_SHIFT == 0 || abs(diff) < RANK_SHIFT);
}

int is_valid_queen_move(int from_sq, int to_sq) {
    return is_valid_rook_move(from_sq, to_sq) || is_valid_bishop_move(from_sq, to_sq);
}

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
    int8_t no_attacks = 0; 

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
            no_attacks += 1;
        }
    }

    int knight_offsets[8] = {33, 31, 18, 14, -14, -18, -31, -33};
    for (int i = 0; i < 8; ++i) {
        int n_sq = sq + knight_offsets[i];
        if (!(n_sq & 0x88) && game -> board[n_sq] == by_color * KNIGHT) {
            no_attacks += 1;
        }
    }

    int rook_dirs[4] = {1, -1, RANK_SHIFT, -RANK_SHIFT};
    for (int i = 0; i < 4; ++i) {
        int r_sq = sq + rook_dirs[i];
        while (!(r_sq & 0x88)) {
            piece = game -> board[r_sq];
            if (piece != EMPTY) {
                if (piece * by_color > 0 && (abs(piece) == ROOK || abs(piece) == QUEEN)) {
                    no_attacks += 1;
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
                    no_attacks += 1;
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
            no_attacks += 1;
        }
    }

    return no_attacks;
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
            // this print was annoying
            // printf("Path is blocked! Try again!\n"); 
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
