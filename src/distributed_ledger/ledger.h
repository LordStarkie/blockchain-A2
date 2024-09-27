#ifndef BLOCKCHAIN_A2_LEDGER_H
#define BLOCKCHAIN_A2_LEDGER_H

#include <vector>
#include <iostream>
#include "inventory.h"

class Ledger {
public:

    static Ledger create_mock_ledger();
    void add_inventory(const Inventory& inventory);
    void print_all_inventories() const;
    const std::vector<Inventory>& get_inventories() const;
    void add_transaction_to_all_inventories(const Transaction& transaction);

private:
    std::vector<Inventory> inventories;
};


#endif //BLOCKCHAIN_A2_LEDGER_H
