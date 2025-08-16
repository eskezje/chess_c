# CHESS GAME TODO LIST
### Got AI to make this list for me, just so that I know what things i need to do.

## Critical Features (Required for proper chess)
- [ ] **Check Detection** - Detect when a king is under attack
- [ ] **Prevent illegal moves that leave king in check** - Players cannot make moves that put/leave their own king in check
- [ ] **Checkmate Detection** - Game should end when king is in check and has no legal moves
- [ ] **Stalemate Detection** - Game should end in draw when player has no legal moves but king is not in check

## Important Chess Rules
- [ ] **Castling** - Special king+rook move (kingside and queenside)
- [ ] **En passant** - Special pawn capture rule
- [ ] **Pawn promotion** - Pawns reaching the end rank should promote to another piece
- [ ] **Move validation for check** - All piece moves should be validated to ensure they don't leave king in check

## Game State & Tracking
- [ ] **Track piece movement history** - Needed for castling rules (king/rook hasn't moved)
- [ ] **Game status tracking** - Track if game is ongoing, checkmate, stalemate, or draw
- [ ] **Move history/notation** - Keep record of all moves played
- [ ] **50-move rule** - Draw after 50 moves without pawn move or capture
- [ ] **Threefold repetition** - Draw when same position occurs 3 times

## Code Quality & Features
- [ ] **Input validation improvements** - Better error handling for edge cases
- [ ] **Save/Load game** - Ability to save and resume games
- [ ] **Undo move** - Allow players to take back moves
- [ ] **Move suggestions/hints** - Show legal moves for a piece
- [ ] **Game timer** - Add chess clocks for timed games

## Bug Fixes & Improvements
- [ ] **Memory management** - Ensure no memory leaks
- [ ] **Code documentation** - Add comments explaining complex logic
- [ ] **Unit tests** - Create tests for piece movement and game logic
- [ ] **Refactor repetitive code** - Clean up similar patterns in piece movement functions

## Nice to Have
- [ ] **AI opponent** - Simple computer player
- [ ] **Different board themes** - Alternative visual representations
- [ ] **PGN format support** - Import/export games in standard notation
- [ ] **Network play** - Play against remote opponents
- [ ] **Move animation** - Smooth visual piece movement

---
**Priority Order:**
1. Check detection (most critical)
2. Checkmate/Stalemate detection
3. Castling, en passant, pawn promotion
4. Game state tracking
5. Everything else

maybe try and use this for ui later?
https://www.nicbarker.com/clay