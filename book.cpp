#include "book.h"
#include "util.h"
#include "product.h"
#include <sstream>
#include <iomanip>
#include <set>

Book::Book(const std::string category, const std::string name, double price, int qty, std::string ISBN, std::string author) :
  Product::Product(category, name, price, qty),
  _isbn(ISBN),
  _author(author)
{
}

Book::~Book() {}

std::string Book::getISBN() const {
  return _isbn;
}

std::string Book::getAuthor() const {
  return _author;
}

std::set<std::string> Book::keywords() const {
  // call parseStringToWords() to create keywords
  // call convToLower() on returned keywords
  std::set<std::string> upper_keywords1 = parseStringToWords(getAuthor());
  std::set<std::string> upper_keywords2 = parseStringToWords(Product::getName());
  upper_keywords1.insert(upper_keywords2.begin(), upper_keywords2.end());
  std::set<std::string> lower_keywords;
  for (std::string str: upper_keywords1) {
      std::string lower = convToLower(str);
      lower_keywords.insert(lower);
  }
  lower_keywords.insert(getISBN());
  std::string name = Product::getName();
  bool isNumeric = true;
  for (char c: name) {
    if (!isdigit(c)) {
      isNumeric=false;
      break;
    }
  }
  if (isNumeric) {
    lower_keywords.insert(convToLower(name));
  }
  return lower_keywords;
}

std::string Book::displayString() const {
  // create string that containes product info
  std::ostringstream oss;
  oss << Product::getName() << "\n";
  oss << "Author: " << getAuthor() << " ISBN: " << getISBN() << "\n";
  oss << std::fixed << std::setprecision(2) << Product::getPrice() << " ";
  oss << Product::getQty() << " left.";
  return oss.str();
}

void Book::dump(std::ostream& os) const {
  Product::dump(os);
  os << this->getISBN() << "\n" << this->getAuthor() << "\n";
}