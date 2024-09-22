#ifndef BLOCKCHAIN_A2_BLOCK_H
#define BLOCKCHAIN_A2_BLOCK_H

#include <string>
#include <ctime>
#include "transaction.h"

// Class to represent a block in the ledger
class Block {
public:
    // Constructor
    Block(Transaction  txn);

    // Get methods
    [[nodiscard]] int get_index() const;
    [[nodiscard]] const std::string& get_previous_hash() const;
    [[nodiscard]] const Transaction& get_transaction() const;
    [[nodiscard]] std::time_t get_timestamp() const;
    [[nodiscard]] const std::string& get_current_hash() const;

    void set_index(int idx);
    void set_previous_hash(const std::string& prev_hash);

private:
    int index{};
    std::time_t timestamp;
    Transaction transaction;
    std::string previous_hash;
    std::string current_hash;
};

#endif //BLOCKCHAIN_A2_BLOCK_H
