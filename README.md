```
                _____ _                      ______             _
     _/|       / ____| |                    |  ____|           (_)                 |\_ 
    // o\     | |    | |__   ___  ___ ___   | |__   _ __   __ _ _ _ __   ___      /X \\
    || ._)    | |    | '_ \ / _ \/ __/ __|  |  __| | '_ \ / _` | | '_ \ / _ \    (_. ||
    //__\     | |____| | | |  __/\__ \__ \  | |____| | | | (_| | | | | |  __/     /__\\
    )___(      \_____|_| |_|\___||___/___/  |______|_| |_|\__, |_|_| |_|\___|     )___(
                                                           __/ |
                                                          |___/              
```
# C++ Chess Engine with Alpha-Beta Pruning

This project is a complete implementation of a chess engine written in modern C++. It includes the full set of chess rules, an object-oriented structure for piece behavior, and an AI system using the Alpha-Beta pruning algorithm to efficiently evaluate moves. A command-line interface allows the user to play against the AI or simulate games between two AIs. The project also includes a suite of unit tests.

## Features

- Complete implementation of chess rules:
  - Legal movement for all pieces
  - Castling
  - En passant
  - Promotion
  - Check and checkmate detection
- Object-oriented design with inheritance for each piece type
- Alpha-Beta pruning algorithm for AI move evaluation
- Move rating and prioritization system
- Legal move generation with king safety and pin detection
- Comprehensive unit tests

## Requirements

- C++20-compatible compiler (e.g., GCC 11+, Clang 13+)
- `make` build tool

## Usage

1. Clone the repository or copy the project files to your local machine.
2. Open a terminal and navigate to the project directory.

To build the project, run:

```bash
make
```
This will compile the program and create an executable file named `chess` (`chess.exe` on Windows).

To run the program:

```bash
./chess      # On Linux/macOS
chess.exe    # On Windows
```

To clean the files generated during compilation, run:

```bash
make clean
```

## File Structure
```
.
├── build/                   # Directory for object files (generated automatically)
│
├── include/                 # Directory containing header files
│   └── AlfaBeta.h           # Declaration of the Alpha-Beta pruning class
│   └── Board.h              # Declaration of the Board class
│   └── Game.h               # Declaration of the Game class
│   └── Piece.h              # Declaration of the base Piece class and derived classes (`Pawn`, `Rook`, `Knight`, etc.)
│   └── Types.h              # Declarations of core enums, custom types (`PositionSet`, `Actions`, `Action`, etc.), and utility structures
│
├── src/                     # Directory containing source files
│   └── AlfaBeta.cpp         # Implementation of the Alpha-Beta pruning algorithm for AI decision-making
│   └── Board.cpp            # Manages the game state, move execution, validation, and board evaluation
│   └── Game.cpp             # Controls the game flow and handles input/output logic
│   └── main.cpp             # Main entry point of the application
│   └── Piece.cpp            # Implementation of the base Piece class and all derived piece types
│   └── Types.cpp            # Implementation of core enums, custom types, and helper structures
│
├── tests/                   # Directory containing unit tests
│   └── Board_unittest.cpp   # Tests for board initialization, evaluation, and various move types (e.g., check, castling, en passant)
│   └── Piece_unittest.cpp   # Tests for individual piece movement logic (`Pawn`, `Rook`, `Knight`, etc.)
│
├── CMakeLists.txt           # Configuration file for building with Google Test
├── Makefile                 # Script for building the project
└── Chess.exe                # Executable file (generated after compilation)
```

## License

This project is licensed under the MIT License. See the LICENSE file for details.

## Contributing

Feel free to contribute by opening issues or submitting pull requests. Your contributions are welcome!