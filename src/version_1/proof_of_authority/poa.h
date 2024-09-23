#ifndef BLOCKCHAIN_A2_POA_H
#define BLOCKCHAIN_A2_POA_H

#include "../../distributed_ledger/inventory.h"
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

class PoA {
public:
    // Constructor
    PoA() = default;

    // Add a validator
    void add_validator(const Inventory& validator);

    // Remove a validator
    void remove_validator(const Inventory& validator);

    // Check if an inventory is an authorized validator
    [[nodiscard]] bool is_validator(const Inventory& validator) const;

    // List all validator inventories
    void list_validators() const;

private:
    std::vector<Inventory> validators; // List of authorized validator inventories
};

#endif //BLOCKCHAIN_A2_POA_H
