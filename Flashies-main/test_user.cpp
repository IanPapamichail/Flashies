#define CATCH_CONFIG_MAIN  // Let Catch2 provide the main() function for running the tests
#include "catch.hpp"       // Include the Catch2 header to enable the testing framework

#include "User.h"          // Include the User class header to be tested

// Test case for testing the constructor of the User class
TEST_CASE("User constructor sets fields", "[user]") {
    // Create a User object with a username and password
    User u("testuser", "abcd1234");

    // Verify that the username and password were correctly set
    REQUIRE(u.username == "testuser");  // Check if the username matches the expected value
    REQUIRE(u.password == "abcd1234");  // Check if the password matches the expected value
}

// Test case for testing the serialization and deserialization methods
TEST_CASE("User toJSON and fromJSON", "[user]") {
    // Create a User object
    User original("nick", "2003");

    // Serialize the original User object into JSON
    auto j = original.toJSON();
    
    // Verify that the JSON representation contains the correct data
    REQUIRE(j["username"] == "nick");  // Check if the "username" in JSON is correct
    REQUIRE(j["password"] == "2003");  // Check if the "password" in JSON is correct

    // Deserialize the JSON back into a User object
    User copy = User::fromJSON(j);

    // Verify that the deserialized User object has the same values
    REQUIRE(copy.username == "nick");   // Check if the username of the deserialized object is correct
    REQUIRE(copy.password == "2003");   // Check if the password of the deserialized object is correct
}