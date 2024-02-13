#ifndef MYDATASTORE_H
#define MYDATASTORE_H
#include "datastore.h"
#include "user.h"
#include "product.h"
#include <unordered_map>
#include <vector>
#include <queue>

class MyDataStore : public DataStore {
public:
  MyDataStore();
  MyDataStore(const std::vector<Product*>& p,
              const std::vector<User*>& u,
              std::unordered_map<std::string, std::queue<Product*>>& usercart,
              std::unordered_map<std::string, std::set<Product*>>& keywordindex);
  virtual~MyDataStore();

  void addUser(User* u) override;
  void buildKeywordIndex();
  void updateKeywordIndex(Product* p);
  void addProduct(Product* p) override;
  std::vector<Product*> search(std::vector<std::string>& terms, int type) override;
  void dump(std::ostream& ofile) override;
  void addToCart(const std::string& username, int hit_result_index);
  void viewCart(const std::string& username);
  void buyCart(const std::string& username);
  // void saveDataBase(const std::string& filename);
  std::vector<User*>& getUsers() {
    return _users;
  }
  std::vector<Product*>& getProducts() {
    return _products;
  }
  std::vector<Product*>& getLastSearchResults() {
    return _lastSearchResults;
  }
  std::unordered_map<std::string, std::set<Product*>>& getKeywordIndex() {
    return _keywordindex;
  }

private:
  std::vector<Product*> _products;
  std::vector<User*> _users;
  std::vector<Product*> _lastSearchResults;

public:
  std::unordered_map<std::string, std::queue<Product*>> _usercarts;
  // maps usernames to carts
  std::unordered_map<std::string, std::set<Product*>> _keywordindex;
  // maps keywords to products

};
#endif