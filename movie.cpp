#include "movie.h"
#include "util.h"
#include "product.h"
#include <sstream>
#include <iomanip>

Movie::Movie(const std::string category, const std::string name, double price, int qty, std::string genre, std::string rating) :
  Product::Product(category, name, price, qty),
  _genre(genre),
  _rating(rating)
{
}

Movie::~Movie() {}

std::string Movie::getGenre() const {
  return _genre;
}

std::string Movie::getRating() const {
  return _rating;
}

std::set<std::string> Movie::keywords() const {
  std::set<std::string> upper_keywords1 = parseStringToWords(getGenre());
  std::set<std::string> upper_keywords2 = parseStringToWords(Product::getName());
  upper_keywords1.insert(upper_keywords2.begin(), upper_keywords2.end());
  std::set<std::string> lower_keywords;
  for (std::string str: upper_keywords1) {
      std::string lower = convToLower(str);
      lower_keywords.insert(lower);
  }
  return lower_keywords;
}

std::string Movie::displayString() const {
  std::ostringstream oss;
  oss << Product::getName() << "\n";
  oss << "Genre: " << getGenre() << " Rating: " << getRating() << "\n";
  oss << std::fixed << std::setprecision(2) << Product::getPrice() << " ";
  oss << Product::getQty() << " left.";
  return oss.str();
}

void Movie::dump(std::ostream& os) const {
  Product::dump(os);
  os << this->getGenre() << "\n" << this->getRating() << "\n";
}