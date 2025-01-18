#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "json.hpp"
#include "User.h"
#include "Deck.h"
#include "Card.h"
using namespace std;
using json = nlohmann::json;

// Global variables for storing user data, decks, and feedback email
vector<User> gUsers;
vector<Deck> gDecks;
string feedbackEmail = "papamichailnickolas@gmail.com";

// Function prototypes for various parts of the application
void loadUsers(const string &file);
void saveUsers(const string &file);
void loadDecks(const string &file);
void saveDecks(const string &file);

User* login();
void signUp();
void mainMenu(User &user);

void studyMode();    // Function to handle active recall + spaced repetition
void viewProgress(); // Function to summarize mastery
void manageDecks(); // Function to add/edit/delete decks/cards
void showFeedback(); // Function to show your email for feedback

int main() {
    // Seed random number generator for card selection
    std::srand(std::time(nullptr));

    // Load users and decks from files
    loadUsers("users.json");
    loadDecks("decks.json");

    while(true) {
        cout << "\n=== FLASHCARDS SPACED APP ===\n";
        cout << "1) Login\n";
        cout << "2) Sign Up\n";
        cout << "3) Exit\n";
        cout << "Choice: ";
        int choice;
        cin >> choice;  // Take user input for the action

        // Handle invalid input
        if(!cin) {
            cin.clear();
            cin.ignore(9999, '\n');
            continue;
        }

        // Action based on user input
        switch(choice) {
            case 1: {
                User* u = login();  // Call login function to authenticate the user
                if(u) mainMenu(*u);  // If login is successful, go to main menu
                break;
            }
            case 2:
                signUp();  // Sign up new user if they don't exist
                break;
            case 3:
                cout << "Goodbye!\n";  // Exit the program
                return 0;
            default:
                cout << "Invalid!\n";  // Handle invalid menu choice
        }
    }
}

// ---------- JSON LOAD/SAVE ----------
// Function to load users from a JSON file
void loadUsers(const string &file) {
    ifstream ifs(file);  // Open the file to read user data
    if(!ifs.is_open()) {
        cerr << "Could not open " << file << ". Starting empty...\n";
        return;
    }
    json j;
    ifs >> j;  // Read the JSON data
    ifs.close();

    // Populate the global gUsers vector with users from the JSON data
    for(const auto &uj : j["users"]) {
        gUsers.push_back(User::fromJSON(uj));  // Deserialize user data into User objects
    }
}

// Function to save users to a JSON file
void saveUsers(const string &file) {
    json j;
    j["users"] = json::array();  // Create a new JSON array for users
    for(const auto &u : gUsers) {
        j["users"].push_back(u.toJSON());  // Serialize each user and add it to the array
    }
    ofstream ofs(file);
    if(ofs.is_open()) {
        ofs << j.dump(4);  // Save the users JSON array to the file with 4 spaces indentation
        ofs.close();
    }
}

// Function to load decks from a JSON file
void loadDecks(const string &file) {
    ifstream ifs(file);  // Open the file to read deck data
    if(!ifs.is_open()) {
        cerr << "Could not open " << file << ". Starting empty...\n";
        return;
    }
    json j;
    ifs >> j;  // Read the JSON data
    ifs.close();

    // Populate the global gDecks vector with decks from the JSON data
    for(const auto &dj : j["decks"]) {
        gDecks.push_back(Deck::fromJSON(dj));  // Deserialize deck data into Deck objects
    }
}

// Function to save decks to a JSON file
void saveDecks(const string &file) {
    json j;
    j["decks"] = json::array();  // Create a new JSON array for decks
    for(const auto &d : gDecks) {
        j["decks"].push_back(d.toJSON());  // Serialize each deck and add it to the array
    }
    ofstream ofs(file);
    if(ofs.is_open()) {
        ofs << j.dump(4);  // Save the decks JSON array to the file with 4 spaces indentation
        ofs.close();
    }
}

// ---------- AUTH ----------
// Function to handle user login
User* login() {
    cout << "\n== Login ==\nUsername: ";
    string uname;
    cin >> uname;
    cout << "Password: ";
    string pass;
    cin >> pass;

    // Check if the provided username and password match any user
    for(auto &u : gUsers) {
        if(u.username == uname && u.password == pass) {
            cout << "Welcome, " << uname << "!\n";
            return &u;  // Return the user object if login is successful
        }
    }
    cout << "Invalid credentials.\n";  // Print error if no match
    return nullptr;
}

// Function to handle new user registration
void signUp() {
    cout << "\n== Sign Up ==\nUsername: ";
    string uname;
    cin >> uname;

    // Check if the username is already taken
    for(const auto &u : gUsers) {
        if(u.username == uname) {
            cout << "Username already taken!\n";
            return;
        }
    }

    cout << "Password: ";
    string pass;
    cin >> pass;

    gUsers.push_back(User(uname, pass));  // Add the new user to the global users list
    saveUsers("users.json");  // Save updated users to the file
    cout << "Sign Up successful!\n";
}

// ---------- MAIN MENU ----------
// Function to display the main menu and handle user actions
void mainMenu(User &user) {
    while(true) {
        cout << "\n=== Main Menu (User: " << user.username << ") ===\n";
        cout << "1) Study Mode\n";
        cout << "2) View Progress\n";
        cout << "3) Manage Decks\n";
        cout << "4) Feedback\n";
        cout << "5) Logout\n";
        cout << "Choice: ";
        int c;
        cin >> c;

        // Handle different actions based on user input
        switch(c) {
            case 1:
                studyMode();  // Call study mode function
                break;
            case 2:
                viewProgress();  // Call progress tracking function
                break;
            case 3:
                manageDecks();  // Call deck management function
                break;
            case 4:
                showFeedback();  // Show feedback contact info
                break;
            case 5:
                cout << "Logging out...\n";
                return;  // Logout and return to the login screen
            default:
                cout << "Invalid!\n";  // Handle invalid input
        }
    }
}

// ---------- STUDY MODE (ACTIVE RECALL + SPACED REP) ----------
// Function to handle the study mode with active recall and spaced repetition
void studyMode() {
    vector<Card*> dueCards;
    // Gather all cards that are due for review
    for(auto &d : gDecks) {
        for(auto &c : d.cards) {
            if(c.isDue()) {
                dueCards.push_back(&c);  // Add due cards to the vector
            }
        }
    }
    if(dueCards.empty()) {
        cout << "No cards are due right now.\n";  // No due cards to study
        return;
    }

    // Pick a random due card
    int idx = rand() % dueCards.size();
    Card* card = dueCards[idx];

    cout << "\n--- Study Card ---\n";
    cout << "Q: " << card->question << "\n";
    cout << "[Press Enter to reveal answer]";
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    cin.get();
    cout << "A: " << card->answer << "\n";

    // Ask the user if they were correct
    cout << "Were you correct? (y/n): ";
    char c;
    cin >> c;
    bool correct = (c == 'y' || c == 'Y');
    card->updateInterval(correct);  // Update the interval and mastery level based on correctness

    saveDecks("decks.json");  // Save the deck data after updating the card
    cout << "Card updated! Interval=" << card->interval 
              << ", Mastery=" << card->masteryLevel
              << ", Next Due=" << card->dueDate << "\n";
}

// ---------- VIEW PROGRESS ----------
// Function to view user's progress in studying cards
void viewProgress() {
    int total = 0;
    int highMaster = 0;  // Cards with mastery level >= 3 are considered mastered
    // Calculate total cards and mastered cards
    for(const auto &d : gDecks) {
        total += d.cards.size();
        for(const auto &c : d.cards) {
            if(c.masteryLevel >= 3) {
                highMaster++;
            }
        }
    }
    cout << "You have " << total << " cards total, " 
              << highMaster << " mastered.\n";  // Display progress
}

// ---------- MANAGE DECKS (ADD/EDIT/DELETE) ----------
// Function to manage decks (add, edit, delete)
void manageDecks() {
    while(true) {
        cout << "\n=== Deck Management ===\n"
                  << "1) View Decks\n"
                  << "2) Add Deck\n"
                  << "3) Back\n"
                  << "Choice: ";
        int c;
        cin >> c;
        if(c == 1) {
            // Show existing decks
            for(auto &d : gDecks) {
                cout << "Deck ID=" << d.id << ", Name=" << d.name << "\n";
            }
            cout << "Enter deck ID to edit (0=cancel): ";
            int dID; 
            cin >> dID;
            if(dID == 0) continue;

            // Find the deck by ID
            auto it = find_if(gDecks.begin(), gDecks.end(), [dID](const Deck &dk){
                return dk.id == dID;
            });
            if(it == gDecks.end()) {
                cout << "Deck not found.\n";
                continue;
            }
            // Edit or Delete deck
            cout << "1) Edit, 2) Delete -> ";
            int c2;
            cin >> c2;
            if(c2 == 1) {
                // Edit deck name
                cout << "New deck name ('.' to skip): ";
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                string newName;
                getline(std::cin, newName);
                if(newName != ".") it->name = newName;
                // Edit cards within the deck
                while(true) {
                    cout << "\nDeck: " << it->name << "\n";
                    for(const auto &cc : it->cards) {
                        cout << "  CardID=" << cc.id 
                                  << ", Q=" << cc.question
                                  << ", A=" << cc.answer << "\n";
                    }
                    cout << "1) Add Card\n"
                              << "2) Edit Card\n"
                              << "3) Remove Card\n"
                              << "4) Done\n"
                              << "Choice: ";
                    int c3; 
                    cin >> c3;
                    if(c3 == 1) {
                        // Add card to deck
                        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        string q, a;
                        cout << "Question: ";
                        getline(std::cin, q);
                        cout << "Answer: ";
                        getline(std::cin, a);
                        int newID = std::rand() % 900000 + 100000;
                        Card newCard(newID, q, a);
                        it->addCard(newCard);
                    }
                    else if(c3 == 2) {
                        // Edit existing card
                        cout << "Card ID? ";
                        int cid; 
                        cin >> cid;
                        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        string nq, na;
                        cout << "New question ('.' skip): ";
                        getline(std::cin, nq);
                        cout << "New answer ('.' skip): ";
                        getline(std::cin, na);
                        for(auto &cd : it->cards) {
                            if(cd.id == cid) {
                                if(nq != ".") cd.question = nq;
                                if(na != ".") cd.answer = na;
                                break;
                            }
                        }
                    }
                    else if(c3 == 3) {
                        // Remove card
                        cout << "Card ID to remove: ";
                        int rcid; 
                        cin >> rcid;
                        it->removeCard(rcid);
                    }
                    else {
                        break;
                    }
                    saveDecks("decks.json");
                }
                saveDecks("decks.json");
            }
            else if(c2 == 2) {
                // Delete the deck
                gDecks.erase(it);
                saveDecks("decks.json");
                cout << "Deck removed.\n";
            }
        }
        else if(c == 2) {
            // Add a new deck
            cout << "Deck name: ";
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            string dn;
            getline(cin, dn);
            int newID = rand() % 900000 + 100000;
            Deck newDeck(newID, dn);
            gDecks.push_back(newDeck);
            saveDecks("decks.json");
            cout << "Deck added.\n";
        }
        else {
            break;
        }
    }
}

// ---------- FEEDBACK ----------
// Function to display feedback contact
void showFeedback() {
    cout << "For feedback, contact: " << feedbackEmail << "\n";
}