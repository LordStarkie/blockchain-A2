#include "src/distributed_ledger/ledger.h"
#include "src/version_1/proof_of_authority/poa.h"
#include "src/version_1/dsa/dsa_main.h"
#include "src/hash/hash.h"

int main() {
    // create a blockchain system with 4 inventories
    Ledger ledger = Ledger::create_mock_ledger();
    ledger.print_all_inventories();

    // create a proof of authority manager
    PoA poa;
    // designate 3/4 inventories as validators
    std::vector<Inventory> inventories = ledger.get_inventories();

    // add inventories A,B,C as validators
    poa.add_validator(inventories[0]);
    poa.add_validator(inventories[1]);
    poa.add_validator(inventories[2]);
    poa.list_validators();

    Transaction proposed_transaction("item4", 100, 100, Location::A);
    poa.propose_block(proposed_transaction, proposed_transaction.get_location());

    // remove inventories as validators
    poa.remove_validator(inventories[0]);
    poa.remove_validator(inventories[1]);
    poa.remove_validator(inventories[2]);

    return 0;
}
