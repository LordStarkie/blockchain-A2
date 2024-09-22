#include "block.h"
#include "../hash/hash.h"
#include <sstream>
#include <iomanip>
#include <utility>

// Hash function for transaction
std::string hash_block(const Block& block) {
    unsigned char* digest = nullptr;
    unsigned int digest_len = 0;

    // Concatenate data for hashing
    std::string data = std::to_string(block.get_index()) +
                       block.get_previous_hash() +
                       block.get_transaction().get_item_id() +
                       std::to_string(block.get_transaction().get_item_qty()) +
                       std::to_string(block.get_transaction().get_item_price()) +
                       std::to_string(block.get_timestamp());

    // SHA-256
    digest_message(reinterpret_cast<const unsigned char*>(data.c_str()), data.size(),
                   &digest, &digest_len);

    // Convert digest to hex string
    std::ostringstream hexStream;
    for (unsigned int i = 0; i < digest_len; ++i) {
        hexStream << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(digest[i]);
    }

    OPENSSL_free(digest); // Free memory
    return hexStream.str(); // Return hash as string
}

// Block Constructor
Block::Block(Transaction  txn)
        : transaction(std::move(txn)), previous_hash("0") { // Default previous hash for a new block
    timestamp = std::time(nullptr); // Set current time
    current_hash = hash_block(*this); // Calculate current hash immediately
}

// Getter methods
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

// Setters
void Block::set_previous_hash(const std::string& prev_hash) {
    previous_hash = prev_hash;
}

void Block::set_index(int idx) {
    index = idx;
}
