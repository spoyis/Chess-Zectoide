# Zectoide Chess Engine
Zectoide is a bitboard based chess engine capable of generating pseudo-legal moves. The engine is built using Visual Studio 2022 and comes with a built-in graphical user interface (GUI) made with SDL 2.0. The SDL files are already included in the repository, so there's no need to install them separately.

# Getting Started
To run the engine, simply clone or download the repository and open the solution file (Zectoide.sln) in Visual Studio 2022. Build the solution and run the chess.exe file to start the engine.

The GUI allows you to play against the engine, the GUI (as of now) still allows the user to play illegal moves, but not the engine (probably xdd).

# Engine Architecture
Zectoide uses a bitboard representation of the chess board and employs bitboard operations to generate pseudo-legal moves. The engine uses the alpha-beta pruning algorithm with iterative deepening to search for the best move.

### Bitboards:
Bitboards are a data structure used in chess engines to efficiently represent the state of the chess board. Each bitboard is a 64-bit integer, with each bit corresponding to a square on the chessboard. A bit with a value of 1 represents the presence of a piece on that square, while a bit with a value of 0 represents an empty square. By using bitboards, it becomes possible to quickly generate moves, calculate attacks, and evaluate the state of the board, Luckily the chess board has an 8x8 configuration, therefore the 64 bit representation fits quite nicely.

### Alpha-Beta Pruning:
Alpha-Beta pruning is a commonly used extension of the minimax algorithm, which is used to find the optimal move in two-player games. It works by exploring the game tree, pruning branches that are not worth exploring, based on the assumption that the opponent will make the best move possible with a given heuristic value. This allows us to explore fewer nodes than pure minimax, leading to more efficient search and improved performance. these techniques are basically mandatory for a chess engine, since chess has a rather high branching factor of about 35-38 moves per position.

### Iterative Deepening:
Iterative Deepening is an algorithm used to improve the efficacy of the pruning. By running the search function many times and gradually increasing the search depth with each iteration and saving the best move found so far - Allowing the next iteration of the search function to start with the best move yet found in order to improve its pruning, it can be counter-intuitive, but the pruning gained by starting with a good move always improves on search times.

# Credits
Zectoide was developed by [spoyis] as a personal project. The engine was built with the help of various online resources and tutorials, including the Chess Programming Wiki and good samaritans on youtube and reddit.
