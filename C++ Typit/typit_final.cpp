#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <random>
#include <chrono>
#include <limits> // for numeric_limits

#ifdef _WIN32
#include <windows.h> // for system("cls") on Windows
#else
#include <unistd.h> // for system("clear") on Unix/Linux
#endif

using namespace std;

// Define a structure to store player scores
struct Score {
    string playerName;
    int score;
    double wordsPerMinute;
};

// Function prototypes
void typingPractice();
void playTypingGame(vector<string> &words, const string &difficulty);
vector<string> readParagraphs(const string &filename);
void calculateStats(const string &original, const string &typed, chrono::duration<double> elapsed);
void displayScoreCard(const vector<Score> &scoreCard);
void saveScore(const Score &score);
void displayMainMenu();

// Global variable to store scores
vector<Score> scoreCard;

// Function to read paragraphs from a file
vector<string> readParagraphs(const string &filename) {
    ifstream file(filename);
    vector<string> paragraphs;
    string line;

    if (file.is_open()) {
        while (getline(file, line)) {
            if (!line.empty()) {
                paragraphs.push_back(line);
            }
        }
        file.close();
    } else {
        cout << "Unable to open file: " << filename << endl;
    }

    return paragraphs;
}

// Function to calculate typing accuracy and words per minute
void calculateStats(const string &original, const string &typed, chrono::duration<double> elapsed) {
    int correctChars = 0;
    int totalChars = original.length();

    for (size_t i = 0; i < typed.length(); ++i) {
        if (i < original.length() && typed[i] == original[i]) {
            correctChars++;
        }
    }

    double accuracy = static_cast<double>(correctChars) / totalChars * 100.0;
    double wordsPerMinute = (totalChars / 5.0) / (elapsed.count() / 60.0);

    cout << "\nAccuracy: " << accuracy << "%\n";
    cout << "Words Per Minute: " << wordsPerMinute << " WPM\n";
}

// Function for typing practice
void typingPractice() {
    srand(static_cast<unsigned int>(time(nullptr)));
    vector<string> paragraphs = readParagraphs("words.txt");

    if (paragraphs.empty()) {
        cout << "No valid paragraphs available for typing practice." << endl;
        return;
    }

    // Select a random paragraph for typing practice
    int randomIndex = rand() % paragraphs.size();
    string originalParagraph = paragraphs[randomIndex];

    cout << "\n=== Typing Practice ===\n";
    cout << "Type the following paragraph:\n\n";
    cout << originalParagraph << "\n\n";

    auto start = chrono::high_resolution_clock::now();

    cout << "Your typing:\n";
    string typedParagraph;
    getline(cin, typedParagraph);

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;

    // Calculate and display typing statistics
    calculateStats(originalParagraph, typedParagraph, elapsed);

    // Wait for Enter key before clearing the screen
    cout << "Press Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Function for playing the typing game
void playTypingGame(vector<string> &words, const string &difficulty) {
    cout << "\n=== Typing Game (Level: " << difficulty << ") ===\n";
    cout << "Enter your name: ";
    string playerName;
    getline(cin, playerName);

    cout << "Press Enter to start...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Wait for the user to press Enter

    time_t startTime = time(nullptr);
    int score = 0;

    // Adjust file name based on difficulty level
    string fileName;
    if (difficulty == "easy") {
        fileName = "easy.txt";
    } else if (difficulty == "medium") {
        fileName = "medium.txt";
    } else if (difficulty == "hard") {
        fileName = "hard.txt";
    } else {
        cout << "Invalid difficulty level. Exiting the game." << endl;
        return;
    }

    vector<string> levelWords = readParagraphs(fileName);

    // Check if words are available for the selected difficulty level
    if (levelWords.empty()) {
        cout << "No valid words available for the selected difficulty level." << endl;
        return;
    }

    shuffle(levelWords.begin(), levelWords.end(), default_random_engine(time(nullptr))); // Shuffle the words

    // Iterate through the words for typing
    for (const string &word : levelWords) {
        cout << "\nType the word: " << word << endl;
        string userInput;
        getline(cin, userInput);

        if (userInput == word) {
            cout << "Correct!" << endl;
            score++;
        } else {
            cout << "Wrong! The correct word was: " << word << endl;
        }
    }

    time_t endTime = time(nullptr);
    double elapsedTime = difftime(endTime, startTime);

    cout << "\nGame Over! Your score: " << score << "/" << levelWords.size() << endl;
    cout << "Time taken: " << elapsedTime << " seconds" << endl;
    cout << "Words per minute: " << static_cast<int>((score / elapsedTime) * 60) << endl;
cout << "Press Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    // Save the score to the Score Card
    Score playerScore;
    playerScore.playerName = playerName;
    playerScore.score = score;
    playerScore.wordsPerMinute = static_cast<int>((score / elapsedTime) * 60);
    scoreCard.push_back(playerScore);
    saveScore(playerScore);
}

// Function to display the score card
void displayScoreCard(const vector<Score> &scoreCard) {
    cout << "\n=== Score Card ===\n";
    cout << "---------------------------------------------\n";
    cout << "Player\t\tScore\tWPM\n";
    cout << "---------------------------------------------\n";

    for (const Score &score : scoreCard) {
        cout << score.playerName << "\t\t" << score.score << "\t" << score.wordsPerMinute << "\n";
    }

    cout << "---------------------------------------------\n";
cout << "Press Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Function to save the score to a file
void saveScore(const Score &score) {
    ofstream file("score_card.txt", ios::app | ios::out); // Add ios::out flag

    if (file.is_open()) {
        file << score.playerName << " " << score.score << " " << score.wordsPerMinute << "\n";
        file.close();
    } else {
        cout << "Unable to save score." << endl;
    }
}

// Function to display the main menu
void displayMainMenu() {
    // Clear the console screen
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    // Display the main menu in a box style with center alignment
    cout << "\n\n";
    cout << "======================================\n";
    cout << "|           Typing Game Menu         |\n";
    cout << "======================================\n";
    cout << "|  1. Practice Typing                |\n";
    cout << "|  2. Play Typing Game               |\n";
    cout << "|      - *easy                       |\n";
    cout << "|      - *medium                     |\n";
    cout << "|      - *hard                       |\n";
    cout << "|  3. Score Card                     |\n";
    cout << "|  4. Quit                           |\n";
    cout << "======================================\n";
    cout << "Enter your choice (1-4): ";
}

int main() {
    int choice;
    vector<string> words = readParagraphs("word_list.txt");
    vector<string> paragraphs = readParagraphs("words.txt");

    do {
        displayMainMenu();
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer

        switch (choice) {
            case 1:
                typingPractice();
                break;
            case 2:
                if (!words.empty()) {
                    cout << "Select difficulty level (easy, medium, hard): ";
                    string difficultyLevel;
                    getline(cin, difficultyLevel);
                    playTypingGame(words, difficultyLevel);
                } else {
                    cout << "No valid words available for the typing game." << endl;
                }
                break;
            case 3:
                displayScoreCard(scoreCard);
                break;
            case 4:
                cout << "Exiting the Game!!! Goodbye!" << endl;
                break;
            default:
                cout << "Invalid choice. Please enter a number between 1 and 4." << endl;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                break;
        }

    } while (choice != 4);

    return 0;
}