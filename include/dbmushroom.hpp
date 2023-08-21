#ifndef DBMUSHROOM_HPP
#define DBMUSHROOM_HPP

#include <stdio.h>

#include <fstream>
#include <iostream>
#include <string>

#include "database.hpp"
#include "txmushroom.hpp"

using namespace std;

class DBMushroom : public DataBase {
 private:
  string file_path;
  fstream data_stream;

 public:
  typedef TxMushroom Transaction;

  void loadDataFromFile(const string filename_path);
  bool isOpen() const;
  void toStart();
  bool next(Transaction& tx);
  void update(Transaction& current_tx, int new_cluster_index);
};

#endif