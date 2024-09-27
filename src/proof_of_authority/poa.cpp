#include "poa.h"
#include "../hash/hash.h"
#include "../dsa/dsa_key_generation.h"
#include "../dsa/dsa_signing.h"
#include "../dsa/dsa_verification.h"

// propose to add a block
bool PoA::propose_block(const Transaction& transaction, Location location, Ledger & ledger) {
    // signature parameters
    BIGNUM *r = BN_new();
    BIGNUM *s = BN_new();
    BIGNUM *m = BN_new();

    // prepare the data string
    std::string data = transaction.get_item_id() +
                       std::to_string(transaction.get_item_qty()) +
                       std::to_string(transaction.get_item_price()) +
                       std::to_string(transaction.get_location());

    // convert to SHA-256 to decimal
    unsigned char *digest = nullptr;
    unsigned int digest_len = 0;
    digest_message(reinterpret_cast<const unsigned char*>(data.c_str()), data.size(),
                   &digest, &digest_len);
    char *decimal_representation = nullptr;
    sha256_to_decimal(digest, digest_len, &decimal_representation);

    // set message for signing
    BN_dec2bn(&m, decimal_representation);

    // get keys
    auto *validator = get_validator(location);
    auto *private_key = get_private_key(location);

    if (validator && private_key) {
        // sign message
        key_sign(validator->p, validator->q, validator->g, private_key->d, &r, &s, &m);
    } else {
        std::cout << "Validator or private key not found." << std::endl;
        return false;
    }

    // all inventories verify
    if (key_verification(m, s, r, validator->g, validator->v, validator->p, validator->q)) {
        std::cout << "Message verification successful" << std::endl;\
        printf("\n");
        // add block to all inventories
        ledger.add_transaction_to_all_inventories(transaction);
        return true;
    } else {
        std::cout << "Message verification unsuccessful" << std::endl;
        printf("\n");
        return false;
    }

    // free memory
    OPENSSL_free(digest);
    OPENSSL_free(decimal_representation);
    BN_free(r);
    BN_free(s);
    BN_free(m);
    return false;
}

// add to validator list
void PoA::add_validator(const Inventory& validator) {
    // stores location and public key
    auto *v = new Validator(validator.get_location());
    // stores location and private key
    auto *p = new Private_Key(validator.get_location());
    // generate keys
    std::cout << "Adding " << validator.get_inventory_name() << " as validator" << std::endl;
    std::cout << "Generating keys for " << validator.get_inventory_name() << std::endl;

    generate_key_pair(&v->p, &v->q, &v->g, &v->v, &p->d);

    // add to lists
    validators.push_back(*v);
    private_keys.push_back(*p);
}

// remove from validator list and private key list
void PoA::remove_validator(const Inventory& validator) {
    Location loc = validator.get_location();

    auto it = std::remove_if(validators.begin(), validators.end(),
                             [loc](const Validator& v) {
        return v.location == loc;
    });
    if (it != validators.end()) {
        validators.erase(it, validators.end());
        std::cout << validator.get_inventory_name() << " removed as validator." << std::endl;
    } else {
        std::cout << validator.get_inventory_name() << " not found as validator." << std::endl;
    }

    auto it_priv = std::remove_if(private_keys.begin(), private_keys.end(),
                                  [loc](const Private_Key& p) {
        return p.location == loc;
    });
    if (it_priv != private_keys.end()) {
        private_keys.erase(it_priv, private_keys.end());
        std::cout << validator.get_inventory_name() << " private key removed." << std::endl;
    } else {
        std::cout << validator.get_inventory_name() << " private key not found." << std::endl;
    }
}

// check if inventory is a validator
bool PoA::is_validator(const Inventory& validator) const {
    Location loc = validator.get_location();
    // check if any validator has the same location
    for (const auto& v : validators) {
        if (v.location == loc) {
            return true;
        }
    }
    return false;
}

// return public keys by location
Validator* PoA::get_validator(Location location) {
    for (auto &validator : validators) {
        if (validator.location == location) {
            return &validator;
        }
    }
    return nullptr;
}

// return private keys by location
Private_Key *PoA::get_private_key(Location location) {
    for (auto &validator : private_keys) {
        if (validator.location == location) {
            return &validator;
        }
    }
    return nullptr;
}

// list all validators
void PoA::list_validators() const {
    std::cout << "Validators: " << std::endl;
    for (const auto& v : validators) {
        std::cout << v.location << " - Public Key: " << v.p << "," << v.q << ","
        << v.v << "," << v.g << std::endl;
    }
}