#ifndef BLOCKCHAIN_A2_INVENTORY_H
#define BLOCKCHAIN_A2_INVENTORY_H

#include <vector>
#include <string>
#include "block.h"
#include "transaction.h"

class Inventory {
public:
    explicit Inventory(Location loc);

    void add_block(const Transaction& transaction);
    void print_blocks() const;
    [[nodiscard]] std::string get_last_block_hash() const;

    std::string hash_block(const Block& block);

    // equality operator
    bool operator==(const Inventory& other) const {
        return location == other.location && blocks == other.blocks;
    }

    // get inventory name
    [[nodiscard]] std::string get_inventory_name() const;
    // get location
    [[nodiscard]] Location get_location() const;
    // get blocks
    const std::vector<Block>& get_blocks() const;

private:
    Location location;
    std::vector<Block> blocks;
};

#endif //BLOCKCHAIN_A2_INVENTORY_H
