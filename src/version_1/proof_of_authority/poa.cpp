#include "poa.h"

// add to validator list
void PoA::add_validator(const Inventory& validator) {
    // stores location and public key
    auto *v = new Validator(validator.get_location());
    // stores location and private key
    auto *p = new Private_Keys(validator.get_location());
    // generate keys
    generate_key_pair(&v->p, &v->q, &v->g, &v->v, &p->d);

    // add to lists
    validators.push_back(*v);
    private_keys.push_back(*p);
}

// remove from validator list
void PoA::remove_validator(const Inventory& validator) {
    Location loc = validator.get_location();

    // Remove from validators list
    auto it = std::remove_if(validators.begin(), validators.end(), [loc](const Validator& v) {
        return v.location == loc;
    });
    if (it != validators.end()) {
        validators.erase(it, validators.end());
        std::cout << "Validator removed." << std::endl;
    } else {
        std::cout << "Validator not found." << std::endl;
    }

    // Remove from private keys list
    auto it_priv = std::remove_if(private_keys.begin(), private_keys.end(), [loc](const Private_Keys& p) {
        return p.location == loc;
    });
    if (it_priv != private_keys.end()) {
        private_keys.erase(it_priv, private_keys.end());
        std::cout << "Private key removed." << std::endl;
    } else {
        std::cout << "Private key not found." << std::endl;
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

// list all validators
void PoA::list_validators() const {
    std::cout << "Validators: " << std::endl;
    for (const auto& v : validators) {
        std::cout << v.location << " - Public Key: " << v.p << "," << v.q << ","
        << v.v << "," << v.g << std::endl;
    }
}

