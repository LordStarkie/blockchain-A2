#ifndef BLOCKCHAIN_A2_TRANSACTION_H
#define BLOCKCHAIN_A2_TRANSACTION_H

#include <string>

enum Location {
    A,
    B,
    C,
    D
};

static std::string location_to_string(Location location) {
    switch (location) {
        case Location::A: return "A";
        case Location::B: return "B";
        case Location::C: return "C";
        case Location::D: return "D";

        default: return "Unknown";
    }
}

class Transaction {
public:
    Transaction(std::string id, int qty, int price, Location loc);

    [[nodiscard]] const std::string& get_item_id() const;
    [[nodiscard]] int get_item_qty() const;
    [[nodiscard]] int get_item_price() const;
    [[nodiscard]] Location get_location() const;

    // equality operator
    bool operator==(const Transaction& other) const {
        return item_id == other.item_id &&
               item_qty == other.item_qty &&
               item_price == other.item_price &&
               location == other.location; // Compare location
    }

private:
    std::string item_id;
    int item_qty;
    int item_price;
    Location location;
};

#endif //BLOCKCHAIN_A2_TRANSACTION_H
