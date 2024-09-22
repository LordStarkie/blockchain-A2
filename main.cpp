#include "src/distributed_ledger/ledger.h"
#include "src/version_1/dsa/dsa_main.h"
#include "src/hash/hash.h"

int main() {

    Ledger ledger = Ledger::create_mock_ledger();
    ledger.print_all_inventories();
    //run();
    //dsa();

    return 0;
}
