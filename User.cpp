#include "User.h"  // Include the header file for the User class (this should define the User class and its methods)
using json = nlohmann::json;  // Alias for nlohmann's JSON library, allowing us to easily use the 'json' type

// Constructor for the User class that initializes the username and password of the user
User::User(const std::string &u, const std::string &p) 
    : username(u), password(p) {}  // Use member initializer list to initialize username and password

// Convert the User object into a JSON object for serialization (e.g., saving to a file or transferring)
json User::toJSON() const {
    return json{  // Create a new JSON object
        {"username", username},  // Add the 'username' field to the JSON
        {"password", password}   // Add the 'password' field to the JSON
    };
}

// Convert a JSON object back into a User object (deserialization)
User User::fromJSON(const json &j) {
    return User(  // Create a new User object using the data from the JSON
        j.at("username").get<std::string>(),  // Get the 'username' field from JSON and convert it to a string
        j.at("password").get<std::string>()   // Get the 'password' field from JSON and convert it to a string
    );
}