#include "transaction.h"

// constructors
Transaction::Transaction(std::string id, int qty, int price, Location loc)
        : item_id(std::move(id)), item_qty(qty), item_price(price), location(loc) {}

// getters
const std::string& Transaction::get_item_id() const {
    return item_id;
}

int Transaction::get_item_qty() const {
    return item_qty;
}

int Transaction::get_item_price() const {
    return item_price;
}

Location Transaction::get_location() const {
    return location;
}
