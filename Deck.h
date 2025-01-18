#ifndef DECK_H  // Ensure the contents of this file are only included once during compilation
#define DECK_H   // Define DECK_H to avoid multiple inclusions

#include <string>     // Include string library to use std::string
#include <vector>     // Include vector library to use std::vector
#include "json.hpp"   // Include nlohmann/json library for JSON serialization/deserialization
#include "Card.h"     // Include Card.h to work with the Card class (each deck contains cards)

class Deck {   // Start of the Deck class definition
public:
    int id;  // The ID of the deck, typically unique
    std::string name;  // The name of the deck
    std::vector<Card> cards;  // A vector to store cards in the deck

    // Constructor that initializes the deck with an id and a name
    Deck(int id, const std::string &name);

    // Method to add a card to the deck
    void addCard(const Card &card);

    // Method to remove a card from the deck by its ID
    void removeCard(int cardId);

    // Method to edit an existing card in the deck by its ID (change its question and answer)
    void editCard(int cardId, const std::string &newQ, const std::string &newA);

    // Method to serialize the deck to JSON format
    nlohmann::json toJSON() const;

    // Static method to deserialize a deck from JSON format
    static Deck fromJSON(const nlohmann::json &j);
};

#endif  // End of the preprocessor directive to close the #ifndef DECK_H