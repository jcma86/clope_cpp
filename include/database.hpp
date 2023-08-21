#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <fstream>
#include <string>

#include "transaction.hpp"

using namespace std;

// Base class for database handlers implementation
// (In this test, dbmushroom.hpp is used)
class DataBase {
 private:
  string file_path;
  fstream data_stream;

 public:
  typedef Transaction Transaction;

  void loadDataFromFile(const string filename_path);
  bool isOpen() const;
  void toStart();
  bool next(Transaction& tx);
  void update(Transaction& current_tx, int new_cluster_index);
};

#endif