#ifndef BLOCKCHAIN_A2_INVENTORY_H
#define BLOCKCHAIN_A2_INVENTORY_H

#include <vector>
#include "block.h"
#include "transaction.h"

// Class to represent an inventory
class Inventory {
public:
    // Constructor
    Inventory(Location loc);

    void add_block(const Transaction& transaction);
    void print_blocks() const;
    std::string get_last_block_hash() const;

private:
    Location location;
    std::vector<Block> blocks;
};

#endif //BLOCKCHAIN_A2_INVENTORY_H
