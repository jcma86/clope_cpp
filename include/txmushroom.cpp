#include "txmushroom.hpp"

// Num of characters to identify in what cluster is the transaction
#define CLUSTER_ID_LEN 5

// type ('e' or 'p') + 22 items separated by commas + end_line charater
#define TX_STR_LEN 46

// item (letter from 'a' to 'z') + separator (',')
#define ITEM_LENGTH 2

const int TxMushroom::clusterIndex() const {
  // The cluster ID is the last 5 characters (added after pre-processing data)
  char number[CLUSTER_ID_LEN + 1];
  raw_tx.copy(number, CLUSTER_ID_LEN, TX_STR_LEN);
  number[CLUSTER_ID_LEN] = '\n';
  return atoi(number) - 1;
}

void TxMushroom::seTransactionData(const std::string& raw_tx_data) {
  char value;
  raw_tx = raw_tx_data;

  for (int i = 0; i < kItemCount; i++) {
    // Skipt the first item (mushrom type 'e' or 'p')
    value = raw_tx[(i + 1) * ITEM_LENGTH];
    if (value == '?' || (value >= 'a' && value <= 'z')) {
      items[i] = value;
      continue;
    }

    cout << "Mushroom values should consist only items in [a..z] and '?' "
            "(for ignored/missing items).\n";
    exit(2);
  }
}
