#define CATCH_CONFIG_MAIN  // This directive tells Catch2 to generate the main function for running the tests
#include "catch.hpp"       // Include Catch2 header for the testing framework
#include "Deck.h"          // Include the Deck class for testing
#include "Card.h"          // Include the Card class because Deck works with Cards

/**
 * Basic tests for the Deck class
 */

// TEST CASE 1: Testing the constructor of Deck
TEST_CASE("Deck constructor sets fields", "[deck]") {
    Deck d(1, "MyTestDeck");  // Create a Deck object with id=1 and name "MyTestDeck"

    // Test that the constructor sets the id correctly
    REQUIRE(d.id == 1);              // The deck should have id 1
    REQUIRE(d.name == "MyTestDeck"); // The deck should have name "MyTestDeck"
    REQUIRE(d.cards.size() == 0);    // The deck should not contain any cards initially
}

// TEST CASE 2: Testing addCard method
TEST_CASE("Deck addCard", "[deck]") {
    Deck d(2, "DeckWithCards");   // Create a Deck object with id=2 and name "DeckWithCards"
    REQUIRE(d.cards.empty());      // Initially, the deck should have no cards

    Card c1(101, "Q1", "A1");     // Create a Card with id=101, question "Q1", and answer "A1"
    d.addCard(c1);                 // Add the card to the deck
    REQUIRE(d.cards.size() == 1);  // The deck should now contain one card
    REQUIRE(d.cards[0].id == 101); // The first card should have id 101
    REQUIRE(d.cards[0].question == "Q1"); // The question of the first card should be "Q1"
    REQUIRE(d.cards[0].answer == "A1");   // The answer of the first card should be "A1"

    // Add another card
    Card c2(102, "Q2", "A2");     // Create another Card with id=102, question "Q2", and answer "A2"
    d.addCard(c2);                 // Add the second card to the deck
    REQUIRE(d.cards.size() == 2);  // The deck should now contain two cards
    REQUIRE(d.cards[1].id == 102); // The second card should have id 102
}

// TEST CASE 3: Testing removeCard method
TEST_CASE("Deck removeCard", "[deck]") {
    Deck d(3, "RemovableDeck");   // Create a Deck object with id=3 and name "RemovableDeck"
    d.addCard(Card(201, "Q201", "A201")); // Add a card with id=201
    d.addCard(Card(202, "Q202", "A202")); // Add a card with id=202
    d.addCard(Card(203, "Q203", "A203")); // Add a card with id=203

    REQUIRE(d.cards.size() == 3);  // The deck should contain 3 cards initially

    // Remove card #202
    d.removeCard(202);              // Remove the card with id=202
    REQUIRE(d.cards.size() == 2);    // The deck should now contain 2 cards
    REQUIRE(d.cards[0].id == 201);  // The first remaining card should have id=201
    REQUIRE(d.cards[1].id == 203);  // The second remaining card should have id=203

    // Attempt to remove a non-existent card with id=999
    d.removeCard(999);              // Trying to remove a card that does not exist
    REQUIRE(d.cards.size() == 2);    // The deck should still contain 2 cards (no change)
}

// TEST CASE 4: Testing editCard method
TEST_CASE("Deck editCard", "[deck]") {
    Deck d(4, "DeckEditor");  // Create a Deck object with id=4 and name "DeckEditor"
    d.addCard(Card(301, "OldQ", "OldA")); // Add a card with id=301, question "OldQ", and answer "OldA"

    REQUIRE(d.cards.size() == 1);  // The deck should contain 1 card initially
    REQUIRE(d.cards[0].question == "OldQ"); // The question should be "OldQ"
    REQUIRE(d.cards[0].answer == "OldA");   // The answer should be "OldA"

    // Edit existing card with id=301
    d.editCard(301, "NewQ", "NewA");  // Change the question to "NewQ" and the answer to "NewA"
    REQUIRE(d.cards[0].question == "NewQ"); // The question should be updated to "NewQ"
    REQUIRE(d.cards[0].answer == "NewA");   // The answer should be updated to "NewA"

    // Try editing a non-existent card with id=999
    d.editCard(999, "ShouldNotChange", "ShouldNotChange"); // Try to edit a card that doesn't exist
    REQUIRE(d.cards.size() == 1);    // The deck should still only have 1 card
    REQUIRE(d.cards[0].question == "NewQ"); // The question should still be "NewQ"
    REQUIRE(d.cards[0].answer == "NewA");   // The answer should still be "NewA"
}

// TEST CASE 5: Testing toJSON and fromJSON methods
TEST_CASE("Deck toJSON and fromJSON", "[deck]") {
    Deck original(5, "JSONDeck"); // Create a Deck object with id=5 and name "JSONDeck"
    original.addCard(Card(401, "Q401", "A401")); // Add a card with id=401, question "Q401", and answer "A401"
    original.addCard(Card(402, "Q402", "A402")); // Add another card with id=402, question "Q402", and answer "A402"

    // Serialize the original deck to JSON
    auto j = original.toJSON();

    // Perform assertions to verify the JSON content
    REQUIRE(j["id"] == 5);                  // Check if the id is correctly serialized
    REQUIRE(j["name"] == "JSONDeck");       // Check if the name is correctly serialized
    REQUIRE(j["cards"].size() == 2);        // Check if the number of cards is correctly serialized
    REQUIRE(j["cards"][0]["id"] == 401);   // Check if the first card's id is correctly serialized
    REQUIRE(j["cards"][0]["question"] == "Q401"); // Check if the first card's question is correctly serialized
    REQUIRE(j["cards"][0]["answer"] == "A401");   // Check if the first card's answer is correctly serialized

    // Deserialize the JSON back into a new Deck object
    Deck copy = Deck::fromJSON(j);
    // Verify that the deserialized deck matches the original deck
    REQUIRE(copy.id == 5);                  // Check if the id is correctly deserialized
    REQUIRE(copy.name == "JSONDeck");       // Check if the name is correctly deserialized
    REQUIRE(copy.cards.size() == 2);        // Check if the number of cards is correctly deserialized
    REQUIRE(copy.cards[0].id == 401);       // Check if the first card's id is correctly deserialized
    REQUIRE(copy.cards[0].question == "Q401"); // Check if the first card's question is correctly deserialized
    REQUIRE(copy.cards[0].answer == "A401");   // Check if the first card's answer is correctly deserialized
}