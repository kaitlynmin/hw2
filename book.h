// write derived book class from product class
// keywords() returns appropriate keywords
// displayString() creates string that contains the product info
// dump() outputs the database format of the product info
#ifndef BOOK_H
#define BOOK_H
#include "product.h"
#include "util.h"
#include <string>
#include <set>

class Book : public Product {
public:
  Book(const std::string category, const std::string name, double price, int qty, std::string ISBN, std::string author);
  virtual ~Book();

  std::set<std::string> keywords() const override;
  std::string displayString() const override;
  void dump(std::ostream& os) const override;

  std::string getISBN() const;
  std::string getAuthor() const;

private:
  std::string _isbn;
  std::string _author;
};
#endif