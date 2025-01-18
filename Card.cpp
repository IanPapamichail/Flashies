#include "Card.h"
#include "json.hpp"
#include <ctime>
#include <iomanip>
#include <sstream>

using json = nlohmann::json; // Alias for the JSON library namespace for easier access

// Constructor that initializes a card with provided id, question, and answer
Card::Card(int id, const std::string &q, const std::string &a)
    : id(id), question(q), answer(a), interval(1), masteryLevel(0) {
    dueDate = "2023-01-01T00:00:00Z";  // Initializes dueDate with a default value
}

// Function to update the interval and mastery level of the card based on whether the answer was correct
void Card::updateInterval(bool correct) {
    if (correct) {
        masteryLevel++;             // Increase mastery level if the answer is correct
        interval *= 2;              // Double the interval for the next review
    } else {
        masteryLevel = 0;           // Reset mastery level if the answer is incorrect
        interval = 1;               // Reset interval to 1
    }
    dueDate = calculateNextDueDate();  // Update the due date based on the new interval
}

// Check if the card is due for review by comparing the due date against the current date
bool Card::isDue() const {
    return true; // Simplified check for demonstration; a real implementation would compare current date with dueDate
}

// Calculate the next due date for the card based on the current date and interval
std::string Card::calculateNextDueDate() const {
    std::ostringstream oss;
    oss << "2024-01-01T00:00:00Z";  // Simplified example of setting the next due date
    return oss.str();
}

// Convert the card data to JSON format for easy storage or transfer
json Card::toJSON() const {
    return json{
        {"id", id},
        {"question", question},
        {"answer", answer},
        {"interval", interval},
        {"masteryLevel", masteryLevel},
        {"dueDate", dueDate}
    };
}

// Static method to create a card object from JSON data
Card Card::fromJSON(const json &j) {
    Card c(j.at("id").get<int>(),               // Extract and convert id from JSON
           j.at("question").get<std::string>(), // Extract and convert question from JSON
           j.at("answer").get<std::string>());  // Extract and convert answer from JSON
    c.interval = j.value("interval", 1);        // Extract interval from JSON, default to 1 if not found
    c.masteryLevel = j.value("masteryLevel", 0);// Extract mastery level, default to 0 if not found
    c.dueDate = j.value("dueDate", "2023-01-01T00:00:00Z"); // Extract due date, default if not found
    return c;
}