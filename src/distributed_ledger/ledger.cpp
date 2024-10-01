#include "ledger.h"

// add inventory to ledger
void Ledger::add_inventory(const Inventory& inventory) {
    inventories.push_back(inventory);
}

// get list of inventories
const std::vector<Inventory>& Ledger::get_inventories() const {
    return inventories;
}

// create mock ledger
Ledger Ledger::create_mock_ledger() {
    Ledger ledger;

    // Define the locations for inventories
    std::vector<Location> locations = {Location::A, Location::B, Location::C, Location::D};
    std::vector<Inventory> inventories;

    // Create inventories using a loop
    for (const auto& loc : locations) {
        std::string loc_str = location_to_string(loc);
        printf("Creating Inventory %s\n", loc_str.c_str());
        Inventory inventory(loc);
        inventories.push_back(inventory);
    }

    printf("\n");

    // Define sample transactions
    std::vector<Transaction> transactions = {
            Transaction("item1", 15, 23, Location::A),
            Transaction("item2", 109, 576, Location::B),
            Transaction("item3", 25, 234, Location::C)
    };

    // Add transactions to each inventory using nested loops
    for (auto& inventory : inventories) {
        for (const auto& tx : transactions) {
            inventory.add_block(tx);
        }
    }

    // Add all inventories to the ledger
    for (const auto& inventory : inventories) {
        ledger.add_inventory(inventory);
    }

    return ledger;
}

// add a transaction to all inventories
void Ledger::add_transaction_to_all_inventories(const Transaction &transaction) {
    for (auto &inventory : inventories) {
        inventory.add_block(transaction);
        std::cout << "Added transaction to inventory: " << inventory.get_inventory_name() << std::endl;
    }

    printf("\n");

}

// print ledger details
void Ledger::print_all_inventories() const {
    for (size_t i = 0; i < inventories.size(); ++i) {
        std::cout << "Inventory " << i + 1 << ":\n";
        inventories[i].print_blocks();
        std::cout << std::endl;
    }
}
