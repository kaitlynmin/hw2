#include "mydatastore.h"
#include "util.h"
#include "book.h"
#include "clothing.h"
#include "movie.h"
#include "product.h"
#include <vector>
#include <string>
#include <iostream>
#include <ostream>
#include <string>
#include <unordered_map>
#include <queue>
#include <fstream>

MyDataStore::MyDataStore() {}
MyDataStore::MyDataStore(const std::vector<Product*>& p,
                         const std::vector<User*>& u,
                         std::unordered_map<std::string, std::queue<Product*>>& usercart,
                         std::unordered_map<std::string, std::set<Product*>>& keywordindex) :
  _products(p), _users(u), _usercarts(usercart), _keywordindex(keywordindex)
{}
MyDataStore::~MyDataStore() {}

void MyDataStore::addUser(User* u) {
  // adds a new user to the system
  // user pointer is added to the _users vector which stores all users in the database
  getUsers().push_back(u);
}

 void MyDataStore::buildKeywordIndex() {
  // keyword index maps keywords to every product that contains that keyword
  _keywordindex.clear();
  for (Product* product : getProducts()) {
      std::set<std::string> keywords = product->keywords();

    for(const std::string& keyword: keywords) {
      _keywordindex[keyword].insert(product);
    }
  }
} 

void MyDataStore::updateKeywordIndex(Product* p) {
  // updates the keyword index for any new products added
  std::set<std::string> keywords = p->keywords();

  for (const std::string& keyword : keywords) {
    _keywordindex[keyword].insert(p);
  }
}

void MyDataStore::addProduct(Product* p) {
  // adds a new product to the data store, given pointer is stored in the _products vector
  // products vector stores all the available products in database
  // maybe update keyword index so newly added product has relevant 
  getProducts().push_back(p);
  updateKeywordIndex(p);
}

std::vector<Product*> MyDataStore::search(std::vector<std::string>& terms, int type) {
  // can use intersection or union to find specific products
  // print products that match the search in ascending order
  // display via displayproducts() function in amazon.cpp
  // search should be case insensitive -> turn all entered keywords into lower containes

  for (size_t i = 0; i < terms.size(); ++i) {
      terms[i] = convToLower(terms[i]);
  }

  std::set<Product*> hitsSet;

  if (terms.empty()) {
    return std::vector<Product*>();
  }

  if (terms.size() == 1) {
    std::set<Product*> products = _keywordindex[terms[0]];
    std::vector<Product*> productList(products.begin(), products.end());
    _lastSearchResults = productList;
    return productList;
  }

  hitsSet = getKeywordIndex()[terms[0]];

  for (size_t i =1; i < terms.size(); ++i) {
    std::set<Product*> possibleResults;
    if (_keywordindex.find(terms[i]) != _keywordindex.end()) {
      possibleResults = _keywordindex[terms[i]];
      if (type == 0) {
        hitsSet = setIntersection(hitsSet, possibleResults);
      } else if (type == 1) {
        hitsSet = setUnion(hitsSet, possibleResults);
      }
    }
  }
  std::vector<Product*> hits(hitsSet.begin(), hitsSet.end());
  _lastSearchResults = hits;
  //_lastSearchResults.insert(_lastSearchResults.begin(), hits.begin(), hits.end());
  return hits;
  // if (terms.size() == 1) {
  //   std::string term = convToLower(terms[0]);
  //   if (_keywordindex.find(term) != _keywordindex.end()) {
  //     return std::vector<Product*>(_keywordindex[term].begin(), _keywordindex[term].end());
  //   } else {
  //     return std::vector<Product*>();
  //   }
  // }

  // for (size_t i=0; i < terms.size(); ++i) {
  //   terms[i] = convToLower(terms[i]);
  // }

  // std::set<Product*> hitsSet;

  // if (terms.empty()) {
  //   return std::vector<Product*>();
  // }

  // hitsSet = getKeywordIndex()[terms[0]];

  // if (terms.size() > 1) {
  // for (size_t i = 1; i < terms.size(); ++i) {
  //   std::set<Product*> possibleResults = _keywordindex[terms[i]];
  //   if (type == 0) {
  //     hitsSet = setIntersection(hitsSet, possibleResults);
  //   } else if (type == 1) {
  //     hitsSet = setUnion(hitsSet, possibleResults);
  //     }
  //   }
  // }
  // std::vector<Product*> hits(hitsSet.begin(), hitsSet.end());
  // getLastSearchResults() = hits;
  // return hits;
}


void MyDataStore::dump(std::ostream& ofile) {
// reproduce the database file from the current Products and User values
// write database contents to ofile
  ofile << "<products>" << std::endl;
  for (Product* product: getProducts()) {
    product->dump(ofile);
    delete product;
  }
  ofile << "</products>" << std::endl;

  ofile << std::fixed << "<users>" << std::endl;
  for(User* user : getUsers()) {
    // std::cout << "Balance before dump" << user->getBalance() << "\n";
    user->dump(ofile);
    // std::cout << "\n Balance after dump" << user->getBalance() << "\n";
    // std::stringstream ss;
    // ss << std::fixed << std::setprecision(2) << user->getBalance();
    // ofile << user->getName() << " " << ss.str() << " " << user->getType() << std::endl;
    delete user;
  }
  ofile << "</users>" << std::endl;

  getProducts().clear();
  getUsers().clear();
}


void MyDataStore::addToCart(const std::string& username, int hit_result_index) {
// from products that search function print, choose which index product you want to added
// if both valid, add to cart to user using index
// if not valid, print "Invalid request" and do nothing
  bool userExists = false;
  std::vector<User*> users = getUsers();
  for (User* user : users) {
    if (user->getName() == username) {
      userExists = true;
      break;
    }
  }
  if (!userExists) {
    std::cout << "Invalid request : user does not exist" << std::endl;
    return;
  }

  //std::vector<Product*> lastSearchResults = getLastSearchResults();
  if (hit_result_index < 1 ||hit_result_index > static_cast<int>(_lastSearchResults.size())) {
    std::cout << "Invalid request : hit index out of range" << hit_result_index << " & " << _lastSearchResults.size() << std::endl;
    return;
  }

  Product* productToAdd = _lastSearchResults[hit_result_index - 1];

  std::unordered_map<std::string, std::queue<Product*>>::iterator it = _usercarts.find(username);
  if (it == _usercarts.end()) {
    std::queue<Product*> newCart;
    newCart.push(productToAdd);
    _usercarts[username] = newCart;
  } else {
    it->second.push(productToAdd);
  }
  // std::queue<Product*> userCart = _usercarts[username];
  // userCart.push(productToAdd);
}


void MyDataStore::viewCart(const std::string& username) {
// display contents of users cart
// if username not valid, print "Invalid username"
// what do we do if there is nothing inside cart
  std::unordered_map<std::string, std::queue<Product*>>::iterator it = _usercarts.find(username);
  if (it == _usercarts.end()) {
    std::cout << "Invalid username" << std::endl;
    return;
  }
  std::queue<Product*> userCart = it->second;
  if (userCart.empty()) {
    return;
  }
  int itemCount = 1;
  while(!userCart.empty()) {
    Product* product = userCart.front();
    std::cout << "Item " << itemCount << std::endl;
    std::cout << product->displayString() << std::endl;
    userCart.pop();
    itemCount++;
  }
}

void MyDataStore::buyCart(const std::string& username) {
// iterate through each item in username's cart
// if item in stock and user has enough $, change in stock quantity and product price reduced from user's balance
// if item not in stock or too expensive, move to next item
// must iterate in order products were added to cart
  std::unordered_map<std::string, std::queue<Product*>>::iterator it = _usercarts.find(username);
  if (it == _usercarts.end()) {
    std::cout << "Invalid username" << std::endl;
    return;
  }
  std::queue<Product*>& userCart = it->second;

  User* user = nullptr;
  for(User* u : getUsers()) {
    if (u->getName() == username) {
      user = u;
      break;
    }
  }

  std::queue<Product*> tempCart;
  while (!userCart.empty()) {
    Product* product = userCart.front();
    if (product->getQty() > 0 && user->getBalance() >= product->getPrice()) {
      user->deductAmount(product->getPrice());
      product->subtractQty(1);
    } else {
      tempCart.push(product);
    }
    userCart.pop();
  }
  _usercarts[username] = userCart;
  while (!userCart.empty()) {
    userCart.pop();
  }
  while (!tempCart.empty()) {
    userCart.push(tempCart.front());
    tempCart.pop();
  }
}

// void MyDataStore::saveDataBase(const std::string& filename) {
// // implements the QUIT filename command
// // creates new version of the database and saves to filename
// // change quantities and user credit
//   std::ofstream outFile(filename);
//   if (!outFile.is_open()) {
//     return;
//   }

//   outFile << "<products>" << std::endl;
//   for (Product* product : getProducts()) {
//     outFile << product->getName() << std::endl;
//     outFile << product->getPrice() << std::endl;
//     outFile << product->getQty() << std::endl;

//     Book* book = dynamic_cast<Book*>(product);
//     if(book != nullptr) {
//       outFile << book->getISBN() << std::endl;
//       outFile << book->getAuthor() << std::endl;
//     } else {
//       Clothing* clothing = dynamic_cast<Clothing*>(product);
//       if (clothing != nullptr) {
//         outFile << clothing->getSize() << std::endl;
//         outFile << clothing->getBrand() << std::endl;
//       } else {
//         Movie* movie = dynamic_cast<Movie*>(product);
//         if (movie != nullptr) {
//           outFile << movie->getGenre() << std::endl;
//           outFile << movie->getRating() << std::endl;
//         }
//       }
//     }
//   }
//   outFile << "</products>" << std::endl;
//   outFile << "<users>" << std::endl;
//   for (User* user : getUsers()) {
//     outFile << user->getName() << " " << user->getBalance() << std::endl;
//   }
//   outFile << "</users>" << std::endl;
//   outFile.close();
// }