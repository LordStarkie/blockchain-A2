#ifndef BLOCKCHAIN_A2_TRANSACTION_H
#define BLOCKCHAIN_A2_TRANSACTION_H

#include <string>

enum Location {
    A,
    B,
    C,
    D
};

class Transaction {
public:
    Transaction(std::string id, int qty, int price, Location loc);

    [[nodiscard]] const std::string& get_item_id() const;
    [[nodiscard]] int get_item_qty() const;
    [[nodiscard]] int get_item_price() const;
    [[nodiscard]] Location get_location() const;

private:
    std::string item_id;
    int item_qty;
    int item_price;
    Location location;
};

#endif //BLOCKCHAIN_A2_TRANSACTION_H
