# CHESS GAME TODO LIST
### Got AI to make this list for me, just so that I know what things i need to do.

## Critical Features (Required for proper chess)
- [x] **Check Detection** - Detect when a king is under attack
- [x] **Prevent illegal moves that leave king in check** - Players cannot make moves that put/leave their own king in check
- [x] **Checkmate Detection** - Game should end when king is in check and has no legal moves
- [x] **Stalemate Detection** - Game should end in draw when player has no legal moves but king is not in check

## Important Chess Rules
- [ ] **Castling** - Special king+rook move (kingside and queenside)
- [ ] **En passant** - Special pawn capture rule
- [ ] **Pawn promotion** - Pawns reaching the end rank should promote to another piece
- [x] **Move validation for check** - All piece moves should be validated to ensure they don't leave king in check

## Basic Piece Movement (Completed)
- [x] **Pawn movement** - Forward movement and diagonal capture
- [x] **Knight movement** - L-shaped moves
- [x] **Bishop movement** - Diagonal moves
- [x] **Rook movement** - Horizontal and vertical moves
- [x] **Queen movement** - Combined rook and bishop moves
- [x] **King movement** - One square in any direction
- [x] **Path collision detection** - Pieces can't move through other pieces
- [x] **Capture logic** - Pieces can capture enemy pieces but not friendly pieces

## Game State & Tracking
- [ ] **Track piece movement history** - Needed for castling rules (king/rook hasn't moved)
- [ ] **Game status tracking** - Track if game is ongoing, checkmate, stalemate, or draw
- [ ] **Move history/notation** - Keep record of all moves played
- [ ] **50-move rule** - Draw after 50 moves without pawn move or capture
- [ ] **Threefold repetition** - Draw when same position occurs 3 times

## Code Quality & Improvements
- [x] **Basic input validation** - Chess notation parsing and validation
- [x] **Board representation** - 0x88 board with piece symbols
- [x] **Turn management** - Alternating player turns
- [x] **Data type optimization** - Using appropriate integer types (int8_t, uint8_t) for better memory usage
- [ ] **Input validation improvements** - Better error handling for edge cases
- [ ] **Save/Load game** - Ability to save and resume games
- [ ] **Undo move** - Allow players to take back moves
- [ ] **Move suggestions/hints** - Show legal moves for a piece
- [ ] **Game timer** - Add chess clocks for timed games
- [ ] **Code optimization** - Remove unnecessary `is_path_clear()` call in king movement
- [ ] **Variable naming clarity** - Consider renaming `no_legal_moves` to `legal_move_count`

## Bug Fixes & Potential Issues
- [x] **Memory management** - Fixed memory usage with appropriate sized types
- [ ] **Code documentation** - Add comments explaining complex logic
- [x] **Unit tests** - Create tests for piece movement and game logic
- [x] **C standard compliance** - Using proper function declarations (main(void) instead of main())
- [ ] **Refactor repetitive code** - Clean up similar patterns in piece movement functions
- [x] **Fix game loop** - Game now properly ends on checkmate
- [ ] **Edge case testing** - Test stalemate detection with actual stalemate positions
- [ ] **Input bounds checking** - Verify all coordinate validation is robust

## Nice to Have
- [ ] **AI opponent** - Simple computer player
- [ ] **Different board themes** - Alternative visual representations
- [ ] **PGN format support** - Import/export games in standard notation
- [ ] **Network play** - Play against remote opponents
- [ ] **Move animation** - Smooth visual piece movement

---
**Priority Order:**
1. ~~Check detection (most critical)~~ ✅ COMPLETED
2. ~~Checkmate detection~~ ✅ COMPLETED
3. ~~Stalemate detection~~ ✅ COMPLETED
4. **Castling, en passant, pawn promotion** ⬅️ **NEXT PRIORITY**
5. Game state tracking & move history
6. Code quality improvements
7. Everything else

## Current Status:
- ✅ **MAJOR MILESTONE**: All core chess rules implemented!
- ✅ Basic chess game is fully playable
- ✅ All piece movements implemented correctly
- ✅ Check detection and prevention working
- ✅ Checkmate detection implemented and working
- ✅ Stalemate detection implemented
- ✅ Game properly ends on checkmate and stalemate
- ✅ Code optimized with appropriate data types
- ✅ Unit tests functioning correctly
- ⚠️ **NEXT CRITICAL**: Implement special moves (castling, en passant, pawn promotion)
- ⚠️ Consider adding move history tracking for advanced rules
- 💡 **OPTIONAL**: Minor code optimizations and edge case improvements

## Testing Recommendations:
- [ ] Test actual stalemate positions to verify detection works
- [ ] Test edge cases with invalid inputs
- [ ] Test memory usage with longer games
- [ ] Verify all piece movement combinations work correctly

maybe try and use this for ui later?
https://www.nicbarker.com/clay
