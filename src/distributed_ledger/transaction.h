#ifndef BLOCKCHAIN_A2_TRANSACTION_H
#define BLOCKCHAIN_A2_TRANSACTION_H

#include <string>

enum Location {
    A,
    B,
    C,
    D
};

// Class to represent a transaction in the ledger
class Transaction {
public:
    // Constructor
    Transaction(std::string id, int qty, int price, Location loc);

    // Get methods
    const std::string& get_item_id() const;
    int get_item_qty() const;
    int get_item_price() const;
    Location get_location() const;

private:
    std::string item_id;
    int item_qty;
    int item_price;
    Location location;
};

#endif //BLOCKCHAIN_A2_TRANSACTION_H
