#ifndef BLOCKCHAIN_A2_POA_H
#define BLOCKCHAIN_A2_POA_H

#include "../../distributed_ledger/inventory.h"
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

class PoA {
public:
    PoA() = default;
    void add_validator(const Inventory& validator);
    void remove_validator(const Inventory& validator);
    [[nodiscard]] bool is_validator(const Inventory& validator) const;
    void list_validators() const;


private:
    std::vector<Inventory> validators;
};

#endif //BLOCKCHAIN_A2_POA_H
