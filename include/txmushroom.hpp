#ifndef TXMUSHROOM_H
#define TXMUSHROOM_H

#include <iostream>
#include <string>

#include "transaction.hpp"

using namespace std;
class TxMushroom : public Transaction {
 private:
  // Mushroom DB has 22 items/props per mushroom
  static const int kItemCount = 22;

  std::array<char, kItemCount> items;
  std::string raw_tx;

 public:
  typedef std::pair<int, char> item_type;
  struct hash;

  inline item_type item(int i) const;

  // Returns the size of the transaction
  inline const int size() const;

  // Resturns the type of mushrom ('e' or 'p')
  inline const char getType() const;

  // Validates the value for the mushroom property
  inline bool isValid(int i) const;

  const int clusterIndex() const;

  // Stores the transaction/mushroom as read from db
  void seTransactionData(const std::string& raw_tx_data);
};

struct TxMushroom::hash {
  // A very simple and common hash function implementation
  // The alphabet has 26 letter, multpiying the item position by a number >= 26
  // and adding the second we can obtain a unique hash for every different
  // transaction
  size_t operator()(const item_type& item) const {
    return item.first * 26 + item.second;
  }
};

const int TxMushroom::size() const { return items.size(); }

const char TxMushroom::getType() const { return raw_tx[0]; }

TxMushroom::item_type TxMushroom::item(int i) const {
  return item_type(i, items[i]);
}

bool TxMushroom::isValid(int i) const {
  return items[i] >= 'a' && items[i] <= 'z';
}

#endif