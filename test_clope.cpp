#include <fstream>
#include <iostream>
#include <string>

#include "./include/clope.hpp"
#include "./include/dbmushroom.hpp"

// Process DB? (add cluster ID to each transaction)
#define PROC_DB true

using namespace std;
int main() {
  // Original Mushroom database
  string db_file = "../data/mushrooms_data.txt";
  // Processed database
  string db_file_processed = "../data/mushrooms_data_processed.txt";

  ifstream in_file;
  in_file.open(db_file);

  if (!in_file.is_open()) {
    cout << "Failed to open file " << db_file << "\n";
    exit(1);
  }

  // Pre-processed data
  // Appends to the end of each transaction the cluster ID
  // An alternative is to use hash maps to track what transactions belong to
  // which cluster but requires more memory and more control of adding and
  // removing transactions.
  if (PROC_DB) {
    ofstream out_file;
    out_file.open(db_file_processed, ios_base::trunc);
    if (!out_file.is_open()) {
      cout << "Failed to created file mushrooms_data_processed.txt\n";
      exit(2);
    }

    string tx;
    in_file >> tx;
    while (!tx.empty()) {
      // Initially all data belongs to cluster 0
      tx.append(",00000\n");
      out_file << tx;
      tx.clear();
      in_file >> tx;
    }

    out_file.close();
    in_file.close();
  }

  Clope<DBMushroom> clope_algorithm;

  clope_algorithm.setRepulsion(2.6);
  // Now we work with the pre-processed data
  if (!clope_algorithm.loadDataFromFile(db_file_processed)) {
    cout << "Failed to open file " << db_file_processed << "\n";
    exit(1);
  }

  clope_algorithm.run();

  return 0;
}