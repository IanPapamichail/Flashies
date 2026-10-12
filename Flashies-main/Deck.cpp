#include "Deck.h"   // Include the header file for the Deck class to define the class and methods
#include <algorithm> // Include the algorithm library for functions like std::remove_if

using json = nlohmann::json; // Use nlohmann's JSON library for working with JSON data

// Constructor for the Deck class. Initializes the deck's ID and name.
Deck::Deck(int id, const std::string &name) : id(id), name(name) {}

// Method to add a card to the deck
void Deck::addCard(const Card &card) {
    cards.push_back(card); // Add the card to the deck's list of cards
}

// Method to remove a card from the deck by its ID
void Deck::removeCard(int cardId) {
    cards.erase(std::remove_if(cards.begin(), cards.end(),
        [cardId](const Card &c){ return c.id == cardId; }), // Remove card with matching ID
        cards.end()); // End iterator, erases the card if found
}

// Method to edit a card's question and answer based on the card's ID
void Deck::editCard(int cardId, const std::string &newQ, const std::string &newA) {
    for (auto &c : cards) {  // Iterate through all cards in the deck
        if (c.id == cardId) { // Check if the current card has the matching ID
            c.question = newQ;  // Update the question of the card
            c.answer = newA;     // Update the answer of the card
            break;               // Exit the loop after editing the card
        }
    }
}

// Convert the Deck object to a JSON object to save or transfer data
json Deck::toJSON() const {
    json j;                 // Create a new JSON object
    j["id"] = id;           // Add the deck's ID to the JSON object
    j["name"] = name;       // Add the deck's name to the JSON object
    j["cards"] = json::array();  // Create an empty array for the cards

    // Iterate through the cards in the deck and add each card's JSON representation
    for (auto &c : cards) {
        j["cards"].push_back(c.toJSON()); // Add each card's JSON data to the "cards" array
    }

    return j; // Return the final JSON object representing the entire deck
}

// Deserialize a Deck object from a JSON object
Deck Deck::fromJSON(const json &j) {
    // Create a Deck object from the ID and name found in the JSON object
    Deck d(j.at("id").get<int>(), j.at("name").get<std::string>());

    // Iterate through each card in the "cards" array and add it to the deck
    for (auto &c : j.at("cards")) {
        d.cards.push_back(Card::fromJSON(c));  // Use Card's fromJSON method to create a Card object from JSON
    }

    return d;  // Return the Deck object created from the JSON data
}