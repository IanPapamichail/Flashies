#ifndef CARD_H
#define CARD_H

#include <string>       // Include the string library to use std::string
#include <chrono>       // Include the chrono library to work with time durations and timestamps
#include "json.hpp"     // Include the nlohmann/json library for handling JSON serialization

// Define the Card class which represents a flashcard for the spaced repetition system
class Card {
public:
    int id;  // Unique identifier for each card
    std::string question;  // The question part of the flashcard
    std::string answer;    // The answer part of the flashcard
    int interval;          // Days until the card should be reviewed again
    int masteryLevel;      // Tracks how well the user knows the card (0 to 5, for example)
    std::string dueDate;   // ISO 8601 formatted string to store the next due date for the card review

    // Constructor to initialize the card with an id, question, and answer
    Card(int id, const std::string &q, const std::string &a);

    // Spaced repetition & active recall:
    // This method updates the card's interval and mastery level based on whether the user answered correctly
    // If the answer is correct, masteryLevel increases and the interval doubles.
    // If the answer is incorrect, masteryLevel is reset to 0 and the interval is reset to 1.
    // It also recalculates the 'dueDate' based on the updated interval.
    void updateInterval(bool correct);

    // Check if the card is due for review. This is done by comparing the current date to the 'dueDate'.
    bool isDue() const;

    // Serialize the card's data into JSON format. This is useful for saving the card data to a file.
    nlohmann::json toJSON() const;

    // Deserialize card data from JSON format. This is useful for loading card data from a file.
    static Card fromJSON(const nlohmann::json &j);

private:
    // Private method to calculate the next due date based on the current interval
    // It returns a string representing the calculated due date (ISO 8601 format)
    std::string calculateNextDueDate() const;
};

#endif  // This ends the preprocessor directive to close the #ifndef CARD_H