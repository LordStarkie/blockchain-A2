#include "block.h"
#include <utility>

// constructor
Block::Block(Transaction  txn)
        : transaction(std::move(txn)) {
    timestamp = std::time(nullptr);
}

// getters
int Block::get_index() const {
    return index;
}

const std::string& Block::get_previous_hash() const {
    return previous_hash;
}

const Transaction& Block::get_transaction() const {
    return transaction;
}

std::time_t Block::get_timestamp() const {
    return timestamp;
}

const std::string& Block::get_current_hash() const {
    return current_hash;
}

// setters
void Block::set_previous_hash(const std::string& prev_hash) {
    previous_hash = prev_hash;
}

void Block::set_current_hash(const std::string &cur_hash) {
    current_hash = cur_hash;
}

void Block::set_index(int idx) {
    index = idx;
}
