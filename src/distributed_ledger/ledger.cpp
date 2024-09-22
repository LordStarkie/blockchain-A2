#include "ledger.h"

// Add Inventory Method
void Ledger::add_inventory(const Inventory& inventory) {
    inventories.push_back(inventory); // Add the inventory to the list
}

// Print All Inventories Method
void Ledger::print_all_inventories() const {
    for (size_t i = 0; i < inventories.size(); ++i) {
        std::cout << "Inventory " << i + 1 << ":\n";
        inventories[i].print_blocks(); // Print blocks in the inventory
        std::cout << std::endl; // Add an extra newline for clarity
    }
}

// Method to create a mock ledger
Ledger Ledger::create_mock_ledger() {
    Ledger ledger;

    // Create four inventories: A, B, C, D
    Inventory inventoryA(Location::A);
    Inventory inventoryB(Location::B);
    Inventory inventoryC(Location::C);
    Inventory inventoryD(Location::D);

    // Create sample transactions
    Transaction trans_1("item1", 15, 23, Location::A);
    Transaction trans_2("item2", 109, 576, Location::B);
    Transaction trans_3("item3", 25, 234, Location::D);

    // Add three identical transactions to each inventory
    for (int i = 0; i < 3; ++i) {
        inventoryA.add_block(trans_1);
        inventoryB.add_block(trans_1);
        inventoryC.add_block(trans_1);
        inventoryD.add_block(trans_1);

        inventoryA.add_block(trans_2);
        inventoryB.add_block(trans_2);
        inventoryC.add_block(trans_2);
        inventoryD.add_block(trans_2);

        inventoryA.add_block(trans_3);
        inventoryB.add_block(trans_3);
        inventoryC.add_block(trans_3);
        inventoryD.add_block(trans_3);
    }

    // Add inventories to the ledger
    ledger.add_inventory(inventoryA);
    ledger.add_inventory(inventoryB);
    ledger.add_inventory(inventoryC);
    ledger.add_inventory(inventoryD);

    return ledger; // Return the populated ledger
}
