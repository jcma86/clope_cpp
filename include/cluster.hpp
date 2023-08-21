#ifndef CLUSTER_HPP
#define CLUSTER_HPP

#include <iostream>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

template <typename Transaction>
class Cluster {
 private:
  int cluster_id;
  int count;  // Number of transactions in cluster
  int width;  // Cluster width
  int size;   // Cluster size

  // Hash table of frequencies (histogram) of items in cluster
  unordered_map<typename Transaction::item_type, int,
                typename Transaction::hash>
      occ;

 public:
  // Constructors
  Cluster();
  Cluster(const Transaction &tx);

  // Returns if cluster is empty
  bool isEmpty() const;

  // Returns if cluster contains the item
  bool contains(const typename Transaction::item_type &item) const;

  const double deltaAdd(const Transaction &tx, const double r) const;

  // Add transaction to cluster
  void add(const Transaction &tx);

  // Remove transaction from cluster
  void remove(const Transaction &tx);
};

template <typename Transaction>
class ClusterList {
 private:
  vector<Cluster<Transaction> > clusters;
  double r;

 public:
  void setRepulsion(const double repulsion);

  // Finds the cluster with the maximum profit for the given transaction
  const int findProfitCluster(const Transaction &tx);

  // Adds transaction to the cluster that gives the max profit
  const int addTransaction(const Transaction &tx);

  // Removes transaction from the cluster
  const int replaceTransaction(const Transaction &tx);

  // Deletes empty clusters
  void deleteEmpty();
};

template <typename Transaction>
Cluster<Transaction>::Cluster() {
  count = 0;
  width = 0;
  size = 0;
}

template <typename Transaction>
Cluster<Transaction>::Cluster(const Transaction &tx) {
  count = 0;
  width = 0;
  size = 0;
  add(tx);
}

template <typename Transaction>
bool Cluster<Transaction>::isEmpty() const {
  return count == 0;
}

template <typename Transaction>
bool ::Cluster<Transaction>::contains(
    const typename Transaction::item_type &item) const {
  return occ.find(item) != occ.end();
}

template <typename Transaction>
void Cluster<Transaction>::add(const Transaction &tx) {
  count++;

  for (int i = 0; i < tx.size(); ++i) {
    if (tx.isValid(i)) {
      auto item = tx.item(i);
      if (!contains(item)) ++width;
      ++occ[item];
      ++size;
    }
  }
}

template <typename Transaction>
void Cluster<Transaction>::remove(const Transaction &tx) {
  count--;

  for (int i = 0; i < tx.size(); ++i) {
    if (tx.isValid(i)) {
      auto item = tx.item(i);
      if (occ[item] == 0) {
        // Remove item from freq/histogram and decrease the width of cluster
        occ.erase(item);
        --width;
      }
      --occ[item];
      --size;
    }
  }
}

// https://loginom.com/blog/clope
template <typename Transaction>
const double Cluster<Transaction>::deltaAdd(const Transaction &tx,
                                            const double r) const {
  int new_size = size;
  int new_width = width;

  for (int i = 0; i < tx.size(); ++i) {
    if (tx.isValid(i)) {
      auto item = tx.item(i);
      if (!contains(item)) new_width++;
      new_size++;
    }
  }

  if (count) {
    return new_size * (count + 1) / pow(new_width, r) -
           size * count / pow(width, r);
  } else {
    return new_size / pow(new_width, r);
  }
}

// https:// loginom.com/blog/clope
template <typename Transaction>
const int ClusterList<Transaction>::findProfitCluster(const Transaction &tx) {
  Cluster<Transaction> new_cluster;
  int index = 0;
  double delta;
  int best_cluster;
  double max_delta;

  best_cluster = clusters.size();
  max_delta = new_cluster.deltaAdd(tx, r);

  for (auto i_cluster = clusters.begin(); i_cluster != clusters.end();
       i_cluster++) {
    delta = i_cluster->deltaAdd(tx, r);
    if (delta >= max_delta) {
      max_delta = delta;
      best_cluster = index;
    }
    index++;
  }
  return best_cluster;
}

template <typename Transaction>
const int ClusterList<Transaction>::addTransaction(const Transaction &tx) {
  const int best_cluster = findProfitCluster(tx);

  if (best_cluster == clusters.size()) {
    clusters.emplace_back(tx);
  } else
    clusters[best_cluster].add(tx);

  return best_cluster;
}

template <typename Transaction>
const int ClusterList<Transaction>::replaceTransaction(const Transaction &tx) {
  const int old_cluster_index = tx.clusterIndex();

  clusters[old_cluster_index].remove(tx);

  return addTransaction(tx);
}

template <typename Transaction>
void ClusterList<Transaction>::deleteEmpty() {
  for (auto i_cluster = clusters.begin(); i_cluster != clusters.end();
       i_cluster++) {
    if (i_cluster->isEmpty()) {
      clusters.erase(i_cluster);
      i_cluster--;
    }
  }
}

template <typename Transaction>
void ClusterList<Transaction>::setRepulsion(const double repulsion) {
  r = repulsion;
}

#endif