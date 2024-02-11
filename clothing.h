#ifndef CLOTHING_H
#define CLOTHING_H
#include "product.h"
#include <string>
#include <set>

class Clothing : public Product {
public:
  Clothing(const std::string category, const std::string name, double price, int qty, std::string size, std::string brand);
  virtual ~Clothing();

  std::set<std::string> keywords() const override;
  std::string displayString() const override;
  void dump(std::ostream& os) const override;

  std::string getSize() const;
  std::string getBrand() const;

private:
  std::string _size;
  std::string _brand;
};
#endif