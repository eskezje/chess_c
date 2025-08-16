# CHESS GAME TODO LIST
### Got AI to make this list for me, just so that I know what things i need to do.

## Critical Features (Required for proper chess)
- [x] **Check Detection** - Detect when a king is under attack
- [x] **Prevent illegal moves that leave king in check** - Players cannot make moves that put/leave their own king in check
- [ ] **Checkmate Detection** - Game should end when king is in check and has no legal moves
- [ ] **Stalemate Detection** - Game should end in draw when player has no legal moves but king is not in check

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

## Code Quality & Features
- [x] **Basic input validation** - Chess notation parsing and validation
- [x] **Board representation** - 0x88 board with piece symbols
- [x] **Turn management** - Alternating player turns
- [ ] **Input validation improvements** - Better error handling for edge cases
- [ ] **Save/Load game** - Ability to save and resume games
- [ ] **Undo move** - Allow players to take back moves
- [ ] **Move suggestions/hints** - Show legal moves for a piece
- [ ] **Game timer** - Add chess clocks for timed games

## Bug Fixes & Improvements
- [ ] **Memory management** - Ensure no memory leaks
- [ ] **Code documentation** - Add comments explaining complex logic
- [x] **Unit tests** - Create tests for piece movement and game logic
- [ ] **Refactor repetitive code** - Clean up similar patterns in piece movement functions
- [ ] **Fix game loop** - Currently infinite loop, needs proper game end conditions

## Nice to Have
- [ ] **AI opponent** - Simple computer player
- [ ] **Different board themes** - Alternative visual representations
- [ ] **PGN format support** - Import/export games in standard notation
- [ ] **Network play** - Play against remote opponents
- [ ] **Move animation** - Smooth visual piece movement

---
**Priority Order:**
1. ~~Check detection (most critical)~~ ✅ COMPLETED
2. Checkmate/Stalemate detection ⬅️ **NEXT PRIORITY**
3. Castling, en passant, pawn promotion
4. Game state tracking
5. Everything else

## Current Status:
- ✅ Basic chess game is playable
- ✅ All piece movements implemented correctly
- ✅ Check detection and prevention working
- ⚠️ Game doesn't end (infinite loop in main)
- ⚠️ Missing endgame conditions (checkmate/stalemate)

maybe try and use this for ui later?
https://www.nicbarker.com/clay