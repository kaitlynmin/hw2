#include "clothing.h"
#include "util.h"
#include <sstream>
#include <iomanip>

Clothing::Clothing(const std::string category, const std::string name, double price, int qty, std::string size, std::string brand) :
  Product::Product(category, name, price, qty),
  _size(size),
  _brand(brand)
{
}

Clothing::~Clothing() {}

std::string Clothing::getSize() const {
  return _size;
}

std::string Clothing::getBrand() const {
  return _brand;
}

std::set<std::string> Clothing::keywords() const {
  std::set<std::string> upper_keywords = parseStringToWords(getBrand());
  std::set<std::string> lower_keywords;
  for (std::string str: upper_keywords) {
      std::string lower = convToLower(str);
      lower_keywords.insert(lower);
  }
  return lower_keywords;
}

std::string Clothing::displayString() const {
  std::ostringstream oss;
  oss << Product::getName() << "\n";
  oss << "Size: " << getSize() << " Brand: " << getBrand() << "\n";
  oss << std::fixed << std::setprecision(2) << Product::getPrice() << " ";
  oss << Product::getQty() << " left.";
  return oss.str();
}


void Clothing::dump(std::ostream& os) const {
  Product::dump(os);
  os << this->getSize() << "\n" << this->getBrand() << "\n";
}