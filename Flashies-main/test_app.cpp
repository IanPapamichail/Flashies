#define CATCH_CONFIG_MAIN  // Let Catch2 provide the main() function, which is needed to run the tests
#include "catch.hpp"

#include "User.h"  // Including the User class for testing
#include "Deck.h"  // Including the Deck class for testing
#include "Card.h"  // Including the Card class for testing

// Function prototypes to ensure the correct functions are accessible for testing
void loadUsers(const std::string &file);  // Function to load users from a file
void saveUsers(const std::string &file);  // Function to save users to a file
void loadDecks(const std::string &file);  // Function to load decks from a file
void saveDecks(const std::string &file);  // Function to save decks to a file

// External references to global variables holding users and decks
extern std::vector<User> gUsers;  // Global vector of users to be tested
extern std::vector<Deck> gDecks;  // Global vector of decks to be tested

/**
 * Test loadUsers and saveUsers
 */
TEST_CASE("loadUsers and saveUsers", "[main][users]") {
    // 1) Clear out the global gUsers vector to start with an empty state
    gUsers.clear();

    // 2) Add some test users to the global gUsers vector
    gUsers.push_back(User("testuser", "pass123"));
    gUsers.push_back(User("nick", "2003"));

    // 3) Save the users to a test file
    const std::string testUsersFile = "test_users.json";  // Define the test file name
    saveUsers(testUsersFile);  // Call saveUsers to write the users to the file

    // 4) Clear the gUsers vector to ensure we are starting fresh before loading
    gUsers.clear();
    REQUIRE(gUsers.empty());  // Verify the gUsers vector is now empty

    // 5) Load the users from the test file
    loadUsers(testUsersFile);  // Call loadUsers to load the test data back into gUsers
    REQUIRE(gUsers.size() == 2);  // Verify there are now two users in gUsers

    // Check that the fields of the first user are correct
    REQUIRE(gUsers[0].username == "testuser");
    REQUIRE(gUsers[0].password == "pass123");

    // Check that the fields of the second user are correct
    REQUIRE(gUsers[1].username == "nick");
    REQUIRE(gUsers[1].password == "2003");

    // Optional: Cleanup the test file (if desired)
    // std::remove(testUsersFile.c_str());  // Uncomment if you want to delete the test file
}

/**
 * Test loadDecks and saveDecks
 */
TEST_CASE("loadDecks and saveDecks", "[main][decks]") {
    // 1) Clear out the global gDecks vector to start with an empty state
    gDecks.clear();

    // 2) Add a deck with some cards
    Deck d1(101, "TestDeck");  // Create a deck with ID 101 and name "TestDeck"
    d1.addCard(Card(201, "Q201", "A201"));  // Add the first card to the deck
    d1.addCard(Card(202, "Q202", "A202"));  // Add the second card to the deck
    gDecks.push_back(d1);  // Add the deck to the global gDecks vector

    // Another deck
    Deck d2(102, "AnotherDeck");  // Create a second deck with ID 102 and name "AnotherDeck"
    d2.addCard(Card(301, "Q301", "A301"));  // Add a single card to the second deck
    gDecks.push_back(d2);  // Add the second deck to the global gDecks vector

    // 3) Save the decks to a test file
    const std::string testDecksFile = "test_decks.json";  // Define the test file name
    saveDecks(testDecksFile);  // Call saveDecks to write the decks to the file

    // 4) Clear the gDecks vector again to ensure we're starting fresh before loading
    gDecks.clear();
    REQUIRE(gDecks.empty());  // Verify the gDecks vector is now empty

    // 5) Load the decks from the test file
    loadDecks(testDecksFile);  // Call loadDecks to load the test data back into gDecks
    REQUIRE(gDecks.size() == 2);  // Verify there are now two decks in gDecks

    // Check the first deck's ID and name
    REQUIRE(gDecks[0].id == 101);
    REQUIRE(gDecks[0].name == "TestDeck");
    // Verify the first deck has two cards
    REQUIRE(gDecks[0].cards.size() == 2);
    REQUIRE(gDecks[0].cards[0].id == 201);
    REQUIRE(gDecks[0].cards[0].question == "Q201");
    REQUIRE(gDecks[0].cards[0].answer == "A201");

    // Check the second deck's ID and name
    REQUIRE(gDecks[1].id == 102);
    REQUIRE(gDecks[1].name == "AnotherDeck");
    // Verify the second deck has one card
    REQUIRE(gDecks[1].cards.size() == 1);
    REQUIRE(gDecks[1].cards[0].id == 301);
    REQUIRE(gDecks[1].cards[0].question == "Q301");
    REQUIRE(gDecks[1].cards[0].answer == "A301");

    // Optional: Cleanup the test file (if desired)
    // std::remove(testDecksFile.c_str());  // Uncomment if you want to delete the test file
}