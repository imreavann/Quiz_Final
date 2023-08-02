#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <conio.h>

using namespace std;

class Question {
private:
    string text;
    vector<string> options;
    char correctOption;

public:
    Question(const string& questionText, const vector<string>& questionOptions, char correctOption)
        : text(questionText), options(questionOptions), correctOption(correctOption) {}

    string getText() const {
        return text;
    }

    vector<string> getOptions() const {
        return options;
    }

    char getCorrectOption() const {
        return correctOption;
    }
};

class Player {
private:
    string name;
    int point;
    bool quizCompleted;

public:
    Player(const string& playerName) : name(playerName), point(0), quizCompleted(false) {}

    string getName() const {
        return name;
    }

    int playerPoints() const {
        return point;
    }

    void plusPoint() {
        point++;
    }

    void completeQuiz() {
        quizCompleted = true;
    }
};

class Quiz {
private:
    vector<Question> questions;
    vector<Player> players;
    string leaderboardFilename;
    string questionsFilename;
public:
    Quiz(const string& leaderboardFile, const string& questionsFile) : leaderboardFilename(leaderboardFile), questionsFilename(questionsFile) {
        loadLeaderboard();
        loadQuestions();
    }

    void addQuestion(const Question& question) {
        questions.push_back(question);
    }

    void addPlayer(const Player& player) {
        players.push_back(player);
    }

    void run() {
        int correctAnswers = 0;
        int incorrectAnswers = 0;
        int skippedQuestions = 0;

        system("cls");
        cout << endl;
        cout << endl;

        for (const auto& question : questions) {
            system("cls");
            cout << "\n\n\n\t\t\t\t\t\t   " << question.getText() << endl;

            int index = 0;
            for (const auto& option : question.getOptions()) {
                cout << "\t\t\t\t\t\t   " << static_cast<char>('A' + index) << ") " << option << endl;
                index++;
            }

            char playerAnswer;
            cout << "\n\n\n\nCavab (skiplemek ucun 's'): ";
            cin >> playerAnswer;

            if (toupper(playerAnswer) == question.getCorrectOption()) {
                cout << "Dogru!" << endl;
                correctAnswers++;
            }
            else if (toupper(playerAnswer) == 'S') {
                cout << "Sual skiplendi." << endl;
                skippedQuestions++;
            }
            else {
                cout << "Sehv! Dogru cavab -> " << question.getCorrectOption() << endl;
                incorrectAnswers++;
            }

            system("pause");
            cout << endl;
        }

        for (auto& player : players) {
            player.completeQuiz();
        }

        cout << "Result" << endl;
        cout << "Correct: " << correctAnswers << endl;
        cout << "Incorrect: " << incorrectAnswers << endl;
        cout << "Skipped: " << skippedQuestions << endl;
        system("pause");
        saveLeaderboard();
    }

    void displayLeaderboard() {
        system("cls");
        cout << endl;
        cout << endl;
        cout << R"(                      _      ______          _____  ______ _____  ____   ____          _____  _____  
                     | |    |  ____|   /\   |  __ \|  ____|  __ \|  _ \ / __ \   /\   |  __ \|  __ \ 
                     | |    | |__     /  \  | |  | | |__  | |__) | |_) | |  | | /  \  | |__) | |  | |
                     | |    |  __|   / /\ \ | |  | |  __| |  _  /|  _ <| |  | |/ /\ \ |  _  /| |  | |
                     | |____| |____ / ____ \| |__| | |____| | \ \| |_) | |__| / ____ \| | \ \| |__| |
                     |______|______/_/    \_\_____/|______|_|  \_\____/ \____/_/    \_\_|  \_\_____/ 
                                                                                 
                                                                                 )" << endl;
        sort(players.begin(), players.end(), [](const Player& a, const Player& b) {
            return a.playerPoints() > b.playerPoints();
            });

        for (const auto& player : players) {
            cout << "\t\t\t\t\t        " << player.getName() << ": " << player.playerPoints() << " points" << endl;
        }
    }

    void saveLeaderboard() {
        ofstream leaderboardFile(leaderboardFilename);

        if (!leaderboardFile) {
            cerr << "Save error" << endl;
            return;
        }

        for (const auto& player : players) {
            leaderboardFile << player.getName() << " " << player.playerPoints() << endl;
        }

        leaderboardFile.close();
    }

    void loadLeaderboard() {
        ifstream leaderboardFile(leaderboardFilename);

        if (!leaderboardFile) {
            cerr << "Load error" << endl;
            return;
        }

        string playerName;
        int playerScore;

        while (leaderboardFile >> playerName >> playerScore) {
            Player player(playerName);
            player.plusPoint();
            players.push_back(player);
        }

        leaderboardFile.close();
    }

    void saveQuestions() {
        ofstream questionsFile(questionsFilename);

        if (!questionsFile) {
            cerr << "Save error" << endl;
            return;
        }

        for (const auto& question : questions) {
            questionsFile << question.getText() << endl;
            for (const auto& option : question.getOptions()) {
                questionsFile << option << endl;
            }
            questionsFile << question.getCorrectOption() << endl;
        }

        questionsFile.close();
    }

    void loadQuestions() {
        ifstream questionsFile(questionsFilename);

        if (!questionsFile) {
            cerr << "Load error" << endl;
            return;
        }

        string questionText;
        string option;
        char correctOption;

        while (getline(questionsFile, questionText)) {
            vector<string> options;
            for (int i = 0; i < 3; i++) {
                getline(questionsFile, option);
                options.push_back(option);
            }
            questionsFile >> correctOption;
            questionsFile.ignore();
            questions.push_back(Question(questionText, options, correctOption));
        }

        questionsFile.close();
    }

    void removeLeaderboard() {
        players.clear();
        saveLeaderboard();
    }

    void removeQuestions() {
        questions.clear();
        saveQuestions();
    }

    bool isAdminPasswordCorrect() const {
        string adminPassword = "admin";
        string enteredPassword;

        cout << "Admin parol daxil et: ";
        cin >> enteredPassword;

        return enteredPassword == adminPassword;
    }
};

Question createQuestion() {
    string questionText;
    vector<string> options;
    char correctOption;

    cin.ignore();

    system("cls");
    cout << endl;
    cout << endl;
    cout << R"(                         ____  _    _ ______  _____ _______ _____ ____  _   _  _____ 
                        / __ \| |  | |  ____|/ ____|__   __|_   _/ __ \| \ | |/ ____|
                       | |  | | |  | | |__  | (___    | |    | || |  | |  \| | (___  
                       | |  | | |  | |  __|  \___ \   | |    | || |  | | . ` |\___ \ 
                       | |__| | |__| | |____ ____) |  | |   _| || |__| | |\  |____) |
                        \___\_\\____/|______|_____/   |_|  |_____\____/|_| \_|_____/ )" << endl;
    cout << "\n\n\n\t\t\t\t\t   Suali yazin: ";
    getline(cin, questionText);

    for (char option = 'A'; option <= 'C'; option++) {
        string optionText;
        cout << "\t\t\t\t\t   " << option << ") ";
        getline(cin, optionText);
        options.push_back(optionText);
    }

    cout << "\t\t\t\t\t   Dogru cavabi yazin (A, B, C): ";
    cin >> correctOption;
    correctOption = toupper(correctOption);

    return Question(questionText, options, correctOption);
}

char getArrowKeyInput() {
    char input = _getch();

    if (input == 0 || input == -32) {
        input = _getch();
        switch (input) {
        case 72:
            return 85;
        case 80:
            return 68;
        }
    }

    return input;
}

int main() {
    string playerName;
    cout << "Adini daxil et: ";
    cin >> playerName;

    Quiz quiz("leaderboard.txt", "questions.txt");
    Player player(playerName);
    quiz.addPlayer(player);

    int choice = 1;
    int selectedOption = 1;
    int maxOptions = 5;

    while (choice != 0) {
        system("cls");
        cout << endl;
        cout << endl;
        cout << R"(                                                 ____  _    _ _____ ______
                                                / __ \| |  | |_   _|___  /
                                               | |  | | |  | | | |    / / 
                                               | |  | | |  | | | |   / /  
                                               | |__| | |__| |_| |_ / /__ 
                                                \___\_\\____/\_____/_____|
                            
                            )";
        cout << endl;
        cout << endl;
        cout << "\t\t\t\t\t\t      Create Quiz" << (selectedOption == 1 ? "  <" : "") << endl;
        cout << "\t\t\t\t\t\t      Start Quiz" << (selectedOption == 2 ? "  <" : "") << endl;
        cout << "\t\t\t\t\t\t      Leaderboard" << (selectedOption == 3 ? "  <" : "") << endl;
        cout << "\t\t\t\t\t\t      Admin Panel" << (selectedOption == 4 ? "  <" : "") << endl;
        cout << "\t\t\t\t\t\t      Exit" << (selectedOption == 0 ? "  <" : "") << endl;

        char input = getArrowKeyInput();
        if (input == 85) {
            selectedOption = (selectedOption + maxOptions - 1) % maxOptions;
        }
        else if (input == 68) { 
            selectedOption = (selectedOption + 1) % maxOptions;
        }
        else if (input == '\r') {
            choice = selectedOption;
            switch (choice) {
            case 1:
                quiz.addQuestion(createQuestion());
                quiz.saveQuestions();
                break;
            case 2:
                quiz.run();
                break;
            case 3:
                quiz.displayLeaderboard();
                cout << "Press any key to continue...";
                _getch();
                break;
            case 4:
                if (quiz.isAdminPasswordCorrect()) {
                    system("cls");
                    cout << endl;
                    cout << endl;
                    cout << R"(                                _____  __  __ _____ _   _     _____        _   _ ______ _      
                          /\   |  __ \|  \/  |_   _| \ | |   |  __ \ /\   | \ | |  ____| |     
                         /  \  | |  | | \  / | | | |  \| |   | |__) /  \  |  \| | |__  | |     
                        / /\ \ | |  | | |\/| | | | | . ` |   |  ___/ /\ \ | . ` |  __| | |     
                       / ____ \| |__| | |  | |_| |_| |\  |   | |  / ____ \| |\  | |____| |____ 
                      /_/    \_\_____/|_|  |_|_____|_| \_|   |_| /_/    \_\_| \_|______|______|
                                                                          
                                                                          )" << endl;
                    cout << "\t\t\t\t\t   1. Leaderboard.txt silmek" << endl;
                    cout << "\t\t\t\t\t   2. Suallar.txt silmek" << endl;
                    cout << "\t\t\t\t\t   0. Menuya qayitmaq" << endl;

                    int adminChoice;
                    cout << "Secim et: ";
                    cin >> adminChoice;

                    switch (adminChoice) {
                    case 1:
                        quiz.removeLeaderboard();
                        break;
                    case 2:
                        quiz.removeQuestions();
                        break;
                    case 0:
                        break;
                    default:
                        cout << "Sehv secim. Error" << endl;
                        break;
                    }
                    cout << "Press any key to continue...";
                    _getch();
                }
                else {
                    cout << "Admin panele giris mumkun olmadi. Tekrar yoxlayin..." << endl;
                    cout << "Press any key to continue...";
                    _getch();
                }
                break;
            case 0:
                quiz.saveLeaderboard();
                quiz.saveQuestions();
                cout << "Exiting..." << endl;
                break;
            }
        }
    }
}
