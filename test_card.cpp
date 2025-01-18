#define CATCH_CONFIG_MAIN  // Let Catch2 provide the main function for running tests
#include "catch.hpp"       // Include Catch2 header for testing framework
#include "Card.h"          // Include the Card class header for testing
#include <string>          // Include string library for using std::string

/**
 * Basic tests for the Card class
 */
TEST_CASE("Card constructor sets defaults", "[card]") {
    // Create a card object with id=101, a question and an answer
    Card c(101, "Question?", "Answer?");

    // Check initial fields to ensure that the constructor is setting them correctly
    REQUIRE(c.id == 101);                        // Check if the id is correctly set to 101
    REQUIRE(c.question == "Question?");          // Check if the question is correctly set
    REQUIRE(c.answer == "Answer?");              // Check if the answer is correctly set
    REQUIRE(c.interval == 1);                    // Check if the default interval is 1
    REQUIRE(c.masteryLevel == 0);                // Check if the default mastery level is 0
    REQUIRE(c.dueDate == "2023-01-01T00:00:00Z"); // Check if the default due date is set correctly
}

TEST_CASE("Card updateInterval on correct answer", "[card]") {
    // Create a card object with id=102, a question and answer
    Card c(102, "Q2", "A2");

    // Initially, check the interval and mastery level values
    REQUIRE(c.interval == 1);                    // The default interval should be 1
    REQUIRE(c.masteryLevel == 0);                // The default mastery level should be 0

    // Mark the card as answered correctly
    c.updateInterval(true);
    // After a correct answer, the interval should double and mastery level should increase
    REQUIRE(c.interval == 2);                    // Interval should be doubled to 2
    REQUIRE(c.masteryLevel == 1);                // Mastery level should increase to 1

    // Mark the card as correct again
    c.updateInterval(true);
    // After another correct answer, the interval should again double and mastery level should increase
    REQUIRE(c.interval == 4);                    // Interval should be doubled again to 4
    REQUIRE(c.masteryLevel == 2);                // Mastery level should increase to 2
}

TEST_CASE("Card updateInterval on incorrect answer", "[card]") {
    // Create a card object with id=103, a question and answer
    Card c(103, "Q3", "A3");

    // Mark the card as answered correctly once
    c.updateInterval(true); // interval=2, mastery=1

    // Now mark the card as answered incorrectly
    c.updateInterval(false);
    // After an incorrect answer, the interval and mastery level should reset
    REQUIRE(c.interval == 1);                    // Interval should reset to 1
    REQUIRE(c.masteryLevel == 0);                // Mastery level should reset to 0
}

TEST_CASE("Card isDue() naive check", "[card]") {
    // Create a card object with id=104, a question and answer
    Card c(104, "Q4", "A4");

    // Your code always returns true, so this test checks that behavior
    REQUIRE(c.isDue() == true); // The method should always return true, as per the current code
}

TEST_CASE("Card toJSON and fromJSON", "[card]") {
    // Create a card object with id=105, a question and answer, and custom interval, mastery level, and due date
    Card original(105, "JSON Q", "JSON A");
    original.interval = 8;                      // Set a custom interval
    original.masteryLevel = 2;                  // Set a custom mastery level
    original.dueDate = "2023-12-31T23:59:59Z";  // Set a custom due date

    // Serialize the card to JSON format
    auto j = original.toJSON();
    // Check that the serialized JSON contains the correct values
    REQUIRE(j["id"] == 105);                    // Check if the id in the JSON matches
    REQUIRE(j["question"] == "JSON Q");         // Check if the question in the JSON matches
    REQUIRE(j["answer"] == "JSON A");           // Check if the answer in the JSON matches
    REQUIRE(j["interval"] == 8);                // Check if the interval in the JSON matches
    REQUIRE(j["masteryLevel"] == 2);            // Check if the mastery level in the JSON matches
    REQUIRE(j["dueDate"] == "2023-12-31T23:59:59Z"); // Check if the due date in the JSON matches

    // Deserialize the JSON back into a Card object
    Card copy = Card::fromJSON(j);
    // Check that the deserialized card matches the original card's values
    REQUIRE(copy.id == 105);                    // Check if the id matches
    REQUIRE(copy.question == "JSON Q");         // Check if the question matches
    REQUIRE(copy.answer == "JSON A");           // Check if the answer matches
    REQUIRE(copy.interval == 8);                // Check if the interval matches
    REQUIRE(copy.masteryLevel == 2);            // Check if the mastery level matches
    REQUIRE(copy.dueDate == "2023-12-31T23:59:59Z"); // Check if the due date matches
}