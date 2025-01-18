#ifndef USER_H           // Check if USER_H is not already defined
#define USER_H           // Define USER_H to prevent multiple inclusions of this header file

#include <string>        // Include the string library to use the string class
#include "json.hpp"      // Include the json.hpp file from the nlohmann JSON library for serialization/deserialization

class User {            // Define the User class
public:
    std::string username;   // Public member variable to store the username as a string
    std::string password;   // Public member variable to store the password as a string

    // Constructor to initialize a User object with a username and password
    User(const std::string &u, const std::string &p);  

    // Method to convert the User object to a JSON object for serialization
    nlohmann::json toJSON() const;

    // Static method to create a User object from a JSON object for deserialization
    static User fromJSON(const nlohmann::json &j);
};

#endif                  // End of the ifndef preprocessor directive