#include "mydatastore.h" // Assuming MyDataStore is defined in a header file
#include <iostream>
#include <sstream>

int main() {
  // Create a test user
  User* user = new User("Alice", 10000.00, 1);

  // Print user balance before dump
  std::cout << "User balance before dump: " << user->getBalance() << std::endl;

  // Open a temporary output stream for testing
  std::ostringstream ofile; // In-memory stream

  // Perform the dump
  user->dump(ofile);

  // Get the dumped content as a string
  std::string output = ofile.str();

  // Print the dumped content
  std::cout << "Dumped content:" << std::endl << output << std::endl;

  // Clean up memory
  delete user;

  return 0;
}
