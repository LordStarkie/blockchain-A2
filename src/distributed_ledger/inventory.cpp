#include "inventory.h"
#include <iostream>

// Inventory Constructor
Inventory::Inventory(Location loc) : location(loc) {}

// Add Block Method
void Inventory::add_block(const Transaction& transaction) {
    int index = blocks.size(); // Current size for the new block index
    std::string prev_hash = (index != 0) ? get_last_block_hash() : "0"; // Default for the first block

    // Create a new block with the transaction
    Block new_block(transaction);
    new_block.set_previous_hash(prev_hash); // Set the previous hash
    new_block.set_index(index); // Set the index

    blocks.push_back(new_block); // Add the new block
}

std::string Inventory::get_last_block_hash() const {
    if (!blocks.empty()) {
        return blocks.back().get_current_hash(); // Return the current hash of the last block
    }
    return "0"; // Return "0" if there are no blocks
}

// Print Blocks Method
void Inventory::print_blocks() const {
    for (const auto& block : blocks) {
        std::cout << "Block Index: " << block.get_index() << "\n"
                  << "Timestamp: " << block.get_timestamp() << "\n"
                  << "Item ID: " << block.get_transaction().get_item_id() << "\n"
                  << "Quantity: " << block.get_transaction().get_item_qty() << "\n"
                  << "Price: " << block.get_transaction().get_item_price() << "\n"
                  << "Location: " << static_cast<int>(block.get_transaction().get_location()) << "\n"
                  << "Previous Hash: " << block.get_previous_hash() << "\n"
                  << "Current Hash: " << block.get_current_hash() << "\n"
                  << std::endl;
    }
}
