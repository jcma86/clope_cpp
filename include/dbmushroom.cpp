#include "dbmushroom.hpp"

bool DBMushroom::isOpen() const { return data_stream.is_open(); }

void DBMushroom::loadDataFromFile(const string filename_path) {
  file_path = filename_path;

  // If stream was already open, close before opening again
  if (data_stream.is_open()) data_stream.close();
  data_stream.open(file_path);

  if (!data_stream.is_open()) {
    cout << "Failed to open file " << file_path << "\n";
    exit(1);
  }
}

bool DBMushroom::next(Transaction& tx) {
  string line;
  // Readtx from db
  getline(data_stream, line);

  if (line.empty()) return false;

  // Store the transaction
  tx.seTransactionData(line);
  return true;
}

// The DB includes (after pre-processing) the cluster ID at the end of each
// transaction
void DBMushroom::update(Transaction& tx, int new_cluster_index) {
  streamoff pos_old;
  char cluster_id[6];

  snprintf(cluster_id, sizeof(cluster_id), "%.5d\n", new_cluster_index + 1);

  pos_old = data_stream.tellg();
  data_stream.seekp(-sizeof(cluster_id), ios_base::cur);
  data_stream.write(cluster_id, sizeof(cluster_id) - 1);
  data_stream.seekg(pos_old, ios_base::beg);
}

void DBMushroom::toStart() {
  data_stream.clear();
  data_stream.seekg(0);
  // loadDataFromFile(file_path);
}
