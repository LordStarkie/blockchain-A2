#include "poa.h"

// add to validator list
void PoA::add_validator(const Inventory& validator) {
    // Check if it exists
    if (std::find(validators.begin(), validators.end(), validator) == validators.end()) {
        validators.push_back(validator);
        std::cout << "Validator added." << std::endl;
    } else {
        std::cout << "Validator already exists." << std::endl;
    }
}

// remove from validator list
void PoA::remove_validator(const Inventory& validator) {
    auto it = std::remove(validators.begin(), validators.end(), validator);
    if (it != validators.end()) {
        validators.erase(it, validators.end());
        std::cout << "Validator removed." << std::endl;
    } else {
        std::cout << "Validator not found." << std::endl;
    }
}

// check if inventory is a validator
bool PoA::is_validator(const Inventory& validator) const {
    return std::find(validators.begin(), validators.end(), validator) != validators.end();
}

// list all validators
void PoA::list_validators() const {
    std::cout << "Validators: ";
    for (const auto& validator : validators) {
        std::cout << validator.get_inventory_name() << " ";
    }
    std::cout << std::endl;
}
