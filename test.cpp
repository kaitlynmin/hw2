#include "mydatastore.h"
#include "book.h"
#include <iostream>

int main() {
    // Create a MyDataStore instance
    MyDataStore dataStore;

    // Create a user with sufficient balance
    User* user = new User("user1", 10000.0, 0);
    dataStore.addUser(user);

    // Add a large number of products to the data store
    const int numProducts = 10000;
    for (int i = 0; i < numProducts; ++i) {
        std::string title = "Book " + std::to_string(i);
        dataStore.addProduct(new Book("book", title, 10.0, 100, "ISBN-" + std::to_string(i), "Author " + std::to_string(i)));
    }

    // Add all products to the user's cart
    for (int i = 0; i < numProducts; ++i) {
        dataStore.addToCart("user1", i + 1);
    }

    // Buy all products from the cart
    dataStore.buyCart("user1");

    // Display the user's remaining balance
    std::cout << "User's remaining balance: $" << user->getBalance() << std::endl;

    // Clean up allocated memory
    delete user;

    return 0;
}


