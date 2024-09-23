#include "inventory.h"
#include "../hash/hash.h"
#include <sstream>
#include <iomanip>
#include <iostream>

// constructor
Inventory::Inventory(Location loc) : location(loc) {}

// add block to inventory
void Inventory::add_block(const Transaction& transaction) {
    int index = blocks.size();
    // genesis block will have no prev hash
    std::string prev_hash = (index != 0) ? get_last_block_hash() : "0";

    // create block and set values
    Block new_block(transaction);
    new_block.set_index(index);
    new_block.set_previous_hash(prev_hash);
    new_block.set_current_hash(hash_block(new_block));

    // add to block to inventory
    blocks.push_back(new_block);
}

// get hash of last block in chain
std::string Inventory::get_last_block_hash() const {
    if (!blocks.empty()) {
        return blocks.back().get_current_hash();
    }
    return "0";
}

// concatenate and hash
std::string Inventory::hash_block(const Block &block) {
    unsigned char* digest = nullptr;
    unsigned int digest_len = 0;

    std::string data = std::to_string(block.get_index()) +
                       block.get_previous_hash() +
                       block.get_transaction().get_item_id() +
                       std::to_string(block.get_transaction().get_item_qty()) +
                       std::to_string(block.get_transaction().get_item_price()) +
                       std::to_string(block.get_transaction().get_location()) +
                       std::to_string(block.get_timestamp());

    // SHA-256
    digest_message(reinterpret_cast<const unsigned char*>(data.c_str()), data.size(),
                   &digest, &digest_len);

    // convert to string
    std::ostringstream hexStream;
    for (unsigned int i = 0; i < digest_len; ++i) {
        hexStream << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(digest[i]);
    }

    // free memory
    OPENSSL_free(digest);

    // return string
    return hexStream.str();
}


// print blocks
void Inventory::print_blocks() const {
    for (const auto& block : blocks) {
        std::cout << "Block Index: " << block.get_index() << "\n"
                  << "Timestamp: " << block.get_timestamp() << "\n"
                  << "Item ID: " << block.get_transaction().get_item_id() << "\n"
                  << "Quantity: " << block.get_transaction().get_item_qty() << "\n"
                  << "Price: " << block.get_transaction().get_item_price() << "\n"
                  << "Location: " << block.get_transaction().get_location() << "\n"
                  << "Previous Hash: " << block.get_previous_hash() << "\n"
                  << "Current Hash: " << block.get_current_hash() << "\n"
                  << std::endl;
    }
}



