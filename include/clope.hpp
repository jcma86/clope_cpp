#ifndef CLOPE_HPP
#define CLOPE_HPP

#include <array>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

#include "cluster.hpp"

using namespace std;

// Using template to be able to work with different databses
// This test implements the DBMushroom class (dbmushroom.hpp)
template <typename DataBase>
class Clope {
 private:
  // Repulsion parameter
  double r;
  DataBase db;
  typename DataBase::Transaction current_tx;
  ClusterList<typename DataBase::Transaction> clusters;

 public:
  bool loadDataFromFile(const string filename);
  void setRepulsion(const double repulsion);

  int run();
};

template <typename T>
void printElement(T t, const int& width) {
  cout << right << setw(width) << setfill(' ') << t;
}

template <typename DataBase>
int Clope<DataBase>::run() {
  int old_cluster_index;
  int new_cluster_index;

  if (!db.isOpen()) {
    cout << "Failed to open data\n";
    exit(1);
  }

  // Phase 1 — initialization
  while (db.next(current_tx)) {
    new_cluster_index = clusters.addTransaction(current_tx);
    db.update(current_tx, new_cluster_index);
  }

  // Phase 2 — Iteration
  bool moved;
  do {
    db.toStart();
    moved = false;
    while (db.next(current_tx)) {
      old_cluster_index = current_tx.clusterIndex();
      new_cluster_index = clusters.replaceTransaction(current_tx);

      if (old_cluster_index != new_cluster_index) {
        db.update(current_tx, new_cluster_index);
        moved = true;
      }
    }
  } while (moved);

  clusters.deleteEmpty();

  // Print clusters info
  db.toStart();
  unordered_map<int, pair<int, int>> cluster_info;
  while (db.next(current_tx)) {
    int cluster_index = current_tx.clusterIndex();
    if (cluster_info.find(cluster_index) == cluster_info.end()) {
      char t = current_tx.getType();
      int e = t == 'e' ? 1 : 0;
      int p = t == 'p' ? 1 : 0;
      cluster_info[cluster_index] = make_pair(e, p);
    } else {
      char t = current_tx.getType();
      if (t == 'e')
        cluster_info[cluster_index].first++;
      else
        cluster_info[cluster_index].second++;
    }
  }
  map<int, pair<int, int>> ordered(cluster_info.begin(), cluster_info.end());
  cout << "-------------------------------";
  cout << endl;
  printElement("| Cluster |", 11);
  printElement("e    |", 10);
  printElement("p    |", 10);
  cout << endl;
  cout << "-------------------------------";
  cout << endl;
  for (auto it = ordered.begin(); it != ordered.end(); ++it) {
    printElement(it->first, 9);
    printElement(it->second.first, 10);
    printElement(it->second.second, 10);
    if (it->second.first != 0 && it->second.second != 0)
      printElement("<- Dirty", 9);
    cout << endl;
  }

  return 0;
}

template <typename DataBase>
bool Clope<DataBase>::loadDataFromFile(const string filename) {
  db.loadDataFromFile(filename);
  return db.isOpen();
}

template <typename DataBase>
void Clope<DataBase>::setRepulsion(const double repulsion) {
  r = repulsion;
  clusters.setRepulsion(repulsion);
}

#endif
