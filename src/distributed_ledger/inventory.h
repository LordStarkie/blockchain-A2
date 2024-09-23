#ifndef BLOCKCHAIN_A2_INVENTORY_H
#define BLOCKCHAIN_A2_INVENTORY_H

#include <vector>
#include "block.h"
#include "transaction.h"

class Inventory {
public:
    explicit Inventory(Location loc);

    void add_block(const Transaction& transaction);
    void print_blocks() const;
    [[nodiscard]] std::string get_last_block_hash() const;

    std::string hash_block(const Block& block);

private:
    Location location;
    std::vector<Block> blocks;
};

#endif //BLOCKCHAIN_A2_INVENTORY_H
