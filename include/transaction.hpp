#ifndef TRANSACTION_HPP
#define TRANSACTION_HPP

#include <array>
#include <string>

using namespace std;

// Base class for transaction handlers implementation
// (In this test, txmushroom.hpp is used)
class Transaction {
 private:
  // Num of items per transaction
  static const int kItemCount = 22;

  array<char, kItemCount> items;

  // Stores the transaction as read from db (line of text with items separated
  // by commas)
  string raw_tx;

 public:
  // Pair of item/prop and its value
  typedef pair<int, char> item_type;

  // Requires implementation of hash generating function done by using
  // operator() implementation
  struct hash;

  inline item_type item(int i) const;

  // Returns the number of items in the transaction
  inline const int size() const;

  // Resturns the type of transaction
  inline const char getType() const;

  // Validates if the value for the given transaction item is valid. For the
  // mushroom database, the values are between 'a' and 'z'
  inline bool isValid(int i) const;

  const int clusterIndex() const;

  void seTransactionData(const string& raw_tx);
};

#endif
