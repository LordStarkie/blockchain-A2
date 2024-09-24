#ifndef BLOCKCHAIN_A2_POA_H
#define BLOCKCHAIN_A2_POA_H

#include "../../distributed_ledger/inventory.h"
#include "../../version_1/dsa/dsa_key_generation.h"

#include "map"
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <memory>
#include <openssl/bn.h>

// stores location + public keys
struct Validator {
    Location location;
    BIGNUM *p;
    BIGNUM *q;
    BIGNUM *g;
    BIGNUM *v;

    // Constructor
    explicit Validator(Location loc) : location(loc) {
        p = BN_new();
        q = BN_new();
        g = BN_new();
        v = BN_new();
    }
};

struct Private_Keys {
    Location location;
    BIGNUM *d;

    // Constructor
    explicit Private_Keys(Location loc) : location(loc) {
        d = BN_new();
    }
};


class PoA {
public:
    PoA() = default;
    void add_validator(const Inventory& validator);
    void remove_validator(const Inventory& validator);
    [[nodiscard]] bool is_validator(const Inventory& validator) const;
    void list_validators() const;

private:
    std::vector<Validator> validators;
    std::vector<Private_Keys> private_keys;

};

#endif //BLOCKCHAIN_A2_POA_H
