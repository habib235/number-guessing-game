#include <iostream>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <list>
#include <memory>
#include <fstream>
#include <limits> // Needed for numeric_limits
#include <conio.h> // For _getch() to read a key press in Windows
#include <thread>  // For adding delay
#include <chrono>  // For adding delay

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

// Define ANSI color codes for terminal output
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define BOLD    "\033[1m"
#define UNDERLINE "\033[4m"
#define BG_RED   "\033[41m"
#define BG_GREEN "\033[42m"
#define BG_BLUE  "\033[44m"

// Node structure for Binary Search Tree (BST)
struct TreeNode {
    int guess;
    TreeNode* left;
    TreeNode* right;

    TreeNode(int guess) : guess(guess), left(nullptr), right(nullptr) {}
};

// Class for managing the Binary Search Tree
class BST {
private:
    TreeNode* root;

    // Helper function to insert a new guess into the BST
    TreeNode* insert(TreeNode* node, int guess) {
        if (!node) {
            return new TreeNode(guess);
        }

        if (guess < node->guess) {
            node->left = insert(node->left, guess);
        } else if (guess > node->guess) {
            node->right = insert(node->right, guess);
        }

        return node;
    }

    // Helper function to perform in-order traversal
    void inorder(TreeNode* node) {
        if (node) {
            inorder(node->left);
            cout << CYAN << node->guess << RESET << " "; // Print in Cyan for better visibility
            inorder(node->right);
        }
    }

public:
    BST() : root(nullptr) {}

    // Insert a guess into the BST
    void insert(int guess) {
        root = insert(root, guess);
    }

    // Print the guesses in ascending order
    void printInOrder() {
        inorder(root);
        cout << endl;
    }
};

// Class for managing the game and player history
class GuessingGame {
private:
    int targetNumber;
    int numGuesses;
    int maxAttempts;
    int difficultyLevel;
    BST bst;
    list<int> history;
    int totalGamesPlayed;
    int totalAttempts;
    int bestScore;
    time_t lastHintTime;  // To store the last time hint was used
    int remainingHintTime; 

public:
    GuessingGame() : numGuesses(0), totalGamesPlayed(0), totalAttempts(0), bestScore(INT_MAX), lastHintTime(0) {
        srand(time(0)); 
        loadPlayerData();  
    }

    void showHint(int guess) {
        // Provide a hint after a guess
        cout << "\nHint: ";
        if (guess < targetNumber) {
            cout << "The target number is higher than your guess." << endl;
        } else if (guess > targetNumber) {
            cout << "The target number is lower than your guess." << endl;
        } else {
            cout << "Congratulations! You've guessed the correct number." << endl;
        }
    }

    void showHintAnimation() {
        // Animation to simulate a "loading" effect for the hint
        cout << "\n" << BOLD << CYAN << "Fetching your hint..." << RESET << endl;
        this_thread::sleep_for(chrono::milliseconds(500));

        for (int i = 0; i <= 100; i++) {
            cout << "\r" << BOLD << CYAN << "Fetching your hint... " << i << "%" << RESET;
            this_thread::sleep_for(chrono::milliseconds(30));
        }

        cout << "\r" << string(30, ' ') << "\r"; // Clear the line after the animation
    }

    bool canUseHint() {
        time_t currentTime = time(nullptr);
        double secondsElapsed = difftime(currentTime, lastHintTime);
        if (secondsElapsed >= 60) {
            return true;
        }else {
        // Return remaining time until hint can be used
        remainingHintTime = 60 - static_cast<int>(secondsElapsed);
        return false;  // Can't use hint yet
    }
        
    }

    void startGame() {
        numGuesses = 0;
        history.clear();
        bst = BST();

        targetNumber = rand() % maxAttempts + 1;
        lastHintTime = 0;  // Reset hint timer

        cout << BG_BLUE << WHITE << BOLD << "\nGuess the number between 1 and " << maxAttempts << "!" << RESET << endl;

        int guess;
        while (numGuesses < 5) {
            cout << UNDERLINE << "\nEnter your guess: " << RESET;

            // Input validation loop
            while (true) {
                if (cin >> guess) {
                    // Check if the input is within the valid range
                    if (guess >= 1 && guess <= maxAttempts) {
                        break; // Break if the guess is valid
                    } else {
                        cout << RED << "Please enter a number between 1 and " << maxAttempts << "." << RESET << endl;
                    }
                } else {
                    cout << RED << "Invalid input! Please enter a valid number between 1 and " << maxAttempts << "." << RESET << endl;
                    cin.clear();  // Clear the error flag
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore invalid input
                }
            }

            numGuesses++;
            history.push_back(guess);
            bst.insert(guess);
                    // Check if the guess is correct or not
        if (guess != targetNumber) {
            cout << RED << "Oops! You have entered the wrong number." << RESET << endl;

}
            // Provide hint after each guess or on request
            if (numGuesses < 5) {
                char useHint;
                if (canUseHint()) {
                    cout << "\nDo you want a hint? (y/n): ";
                    cin >> useHint;
                    if (useHint == 'y' || useHint == 'Y') {
                        showHintAnimation(); // Show hint animation
                        showHint(guess);     // Show the actual hint
                        lastHintTime = time(nullptr);  // Record the time of hint usage
                    }
                } else {
                    cout << RED << "You can only use the hint once per game or after 1 minute. Please wait  "<< remainingHintTime << " seconds."  << RESET << endl;
                }
            }

            if (guess == targetNumber) {
                cout << GREEN << "Congratulations! You guessed the number in " << numGuesses << " guesses." << RESET << endl;
                totalGamesPlayed++;
                totalAttempts += numGuesses;
                bestScore = min(bestScore, numGuesses);
                break;
            }
        }

        if (numGuesses >= 5 && targetNumber != history.back()) {
            cout << RED << "Game over! You've used all 5 attempts. The correct number was: " << targetNumber << RESET << endl;
        }
        showSortedGuesses();

        savePlayerData();
        saveGuessHistory();
    }

    void showHistory() {
        cout << CYAN << "\nYour guess history: " << RESET;
        for (const int& h : history) {
            cout << BOLD << h << RESET << " ";
        }
        cout << endl;
    }

    void showSortedGuesses() {
        cout << CYAN << "\nYour guesses in sorted order: " << RESET;
        bst.printInOrder();
    }

    void showStats() {
        cout << GREEN << "\nTotal games played: " << RESET << totalGamesPlayed << endl;
        cout << GREEN << "Total attempts: " << RESET << totalAttempts << endl;
        cout << GREEN << "Best score (fewest attempts): " << RESET << bestScore << endl;
    }

    void setDifficulty() {
        cout << BG_GREEN << WHITE << BOLD << "\nSelect difficulty level:" << RESET << endl;
        cout << "1. Easy (1-50)" << endl;
        cout << "2. Medium (1-100)" << endl;
        cout << "3. Hard (1-200)" << endl;
        cout << "Enter your choice: ";
        cin >> difficultyLevel;

        switch (difficultyLevel) {
            case 1: maxAttempts = 50; break;
            case 2: maxAttempts = 100; break;
            case 3: maxAttempts = 200; break;
            default:
                cout << RED << "Invalid choice, defaulting to Medium." << RESET << endl;
                maxAttempts = 100;
        }
    }

    void clearScreen() {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }

    void restartGame() {
        char choice;
        cout << "\nPress any key to restart or 'q' to quit to menu: ";
        choice = _getch();
        if (choice == 'q' || choice == 'Q') {
            clearScreen();
            return;
        } else {
            clearScreen();
            startGame();
        }
    }

    void savePlayerData() {
        ofstream outFile("player_data.txt");
        if (outFile) {
            outFile << totalGamesPlayed << endl;
            outFile << totalAttempts << endl;
            outFile << bestScore << endl;
        }
        outFile.close();
    }

    void loadPlayerData() {
        ifstream inFile("player_data.txt");
        if (inFile) {
            inFile >> totalGamesPlayed;
            inFile >> totalAttempts;
            inFile >> bestScore;
        }
        inFile.close();
    }

    void saveGuessHistory() {
        ofstream outFile("guess_history.txt");
        if (outFile) {
            for (const int& guess : history) {
                outFile << guess << " ";
            }
            outFile << endl;
        }
        outFile.close();
    }

    // New function to display instructions
    void showInstructions() {
        cout << "\n===== Game Instructions =====" << endl;
        cout << "1. The game will generate a random number between 1 and the maximum number based on the difficulty level." << endl;
        cout << "2. You have 5 attempts to guess the correct number." << endl;
        cout << "3. You can input numbers only, and they should be within the specified range." << endl;
        cout << "4. After each guess, the program will tell you if the guess is too high, too low, or correct." << endl;
        cout << "5. You can view your history of guesses and sorted guesses." << endl;
        cout << "6. You can view your stats such as total games played, total attempts, and your best score." << endl;
        cout << "Good luck and have fun!" << endl;
    }
};

void displayMenu() {
    cout << BG_BLUE << WHITE << BOLD << "\n===== Welcome to the Number Guessing Game =====" << RESET << endl;
    cout << "1. Start Game" << endl;
    cout << "2. Instructions" << endl;
    cout << "3. View Stats" << endl;
    cout << "4. Set Difficulty" << endl;
    cout << "5. Exit" << endl;
    cout << BG_BLUE << WHITE << "=============================================" << RESET << endl;
    cout << "Choose an option (1/2/3/4/5): ";
}

int getValidMenuChoice() {
    int choice;
    while (true) {
        if (cin >> choice && choice >= 1 && choice <= 5) {
            return choice;
        } else {
            cout << RED << "Invalid input! Please enter a valid number between 1 and 5." << RESET << endl;
            cin.clear();  // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore invalid input
        }
    }
}

// Function to simulate the advanced loading screen, now centered
void showLoadingScreen() {
    // Get the console window size
    #ifdef _WIN32
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        int consoleHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
        int consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    #else
        // Assuming Unix-like system, using typical terminal height and width
        int consoleHeight = 24;  // Approximate height
        int consoleWidth = 80;   // Approximate width
    #endif

    // Calculate the middle of the screen
    int middleRow = consoleHeight / 2;
    int middleColumn = (consoleWidth - 50) / 2; // Center the loading bar (50 chars wide)

    // Move cursor to the center
    cout << "\033[" << middleRow << ";" << middleColumn << "H"; // Move cursor to calculated position

    cout << BOLD << CYAN << "Loading..." << RESET << endl;
    cout << "\033[" << middleRow + 1 << ";" << middleColumn << "H"; // Move cursor down for progress bar

    for (int i = 0; i <= 100; i++) {
        cout << "\033[" << middleRow + 2 << ";" << middleColumn << "H"; // Reset cursor to same line
        cout << "["; 
        int progress = i / 2;  // Divide by 2 for smoother progress bar
        for (int j = 0; j < 50; j++) {
            if (j < progress)
                cout << GREEN << "#" << RESET;
            else
                cout << " ";
        }
        cout << "] " << i << "%" << flush;
        this_thread::sleep_for(chrono::milliseconds(50));
    }

    // Clear the screen after loading is done
    cout << "\033[2J";  // Clear screen
    cout << "\033[H";   // Move cursor to top-left
}

// Main program
int main() {
    GuessingGame game;
    int choice;

    showLoadingScreen(); // Show loading screen before menu appears

    while (true) {
        displayMenu();
        choice = getValidMenuChoice();

        switch (choice) {
            case 1:
            	game.clearScreen();
                game.setDifficulty();
                game.clearScreen();
                game.startGame();
                game.restartGame();
                game.clearScreen();
                break;
            case 2:
            	game.clearScreen();
                game.showInstructions(); // Now works correctly
                break;
            case 3:
            	game.clearScreen();
                game.showStats();
                break;
            case 4:
                game.setDifficulty();
                game.clearScreen();
                break;
            case 5:
                cout << BG_BLUE << WHITE << "Thank you for playing! Goodbye." << RESET << endl;
                return 0;
            default:
                // This case will never be reached since we handle valid input
                break;
        }
    }

    return 0;
}
