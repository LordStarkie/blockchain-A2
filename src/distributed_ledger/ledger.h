#ifndef BLOCKCHAIN_A2_LEDGER_H
#define BLOCKCHAIN_A2_LEDGER_H

#include <vector>
#include <iostream>
#include "inventory.h"

class Ledger {
public:
    // Mock data
    static Ledger create_mock_ledger();
    void add_inventory(const Inventory& inventory);
    void print_all_inventories() const;

private:
    std::vector<Inventory> inventories; // List of inventories
};


#endif //BLOCKCHAIN_A2_LEDGER_H
