# number-guessing-game
number guessing game in c++ using dsa
Key Components:
1. Binary Search Tree (BST):
Used to store the player's guesses and display them in sorted order.
Contains methods for inserting guesses and performing in-order traversal for sorted output.
2. Game State Management:
The GuessingGame class manages the game logic, tracks guesses, provides hints, maintains player statistics, and handles persistence (saving/loading data).
3. Difficulty Levels:
Offers three difficulty levels:
Easy: Numbers between 1–50.
Medium: Numbers between 1–100.
Hard: Numbers between 1–200.
Players can select their preferred difficulty before starting the game.
4. Hint System:
Players can use a hint once per game or after a cooldown of 1 minute.
The hint indicates whether the target number is higher or lower than the guess.
Includes a visual animation to enhance the user experience.
5. Guess History:
Uses a std::list to track the order of guesses.
Displays the history of guesses and sorts them using the BST.
6. Player Statistics:
Tracks:
Total games played.
Total attempts made across all games.
Best score (fewest guesses to win a game).
Saves and loads player data from a file to maintain persistent statistics across sessions.
7. Menu System:
Provides options to:
Start the game.
View instructions.
Check player statistics.
Change difficulty.
Exit the game.
8. Input Validation and Error Handling:
Ensures guesses are valid numbers within the correct range.
Handles invalid inputs gracefully.
Code Flow:
Loading Screen:

Simulates a loading process with a progress bar.
Main Menu:

Displays the menu and processes user input.
Game Loop:

The player guesses the target number within 5 attempts.
After each guess:
Feedback is given (correct, too high, or too low).
Hints are optionally offered with a cooldown mechanism.
If the player guesses correctly, the game records the result and updates statistics.
If all attempts are used, the game reveals the target number.
Guess Management:

The program records all guesses in a list for history.
Uses a BST to organize and display guesses in sorted order.
Persistence:

Player statistics and guess history are saved to files:
player_data.txt for statistics.
guess_history.txt for guess records.
Post-Game Options:

Players can choose to restart or return to the main menu.
Additional Features:
Cross-Platform Compatibility:

Supports both Windows (cls for clearing screen, _getch()) and Unix-like systems (clear).
Colorful and User-Friendly UI:

Utilizes ANSI escape codes for vibrant terminal output (colors, bold text, etc.).
Detailed Instructions:

Provides clear gameplay guidelines.
