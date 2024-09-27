#ifndef BLOCKCHAIN_A2_UI_H
#define BLOCKCHAIN_A2_UI_H

#include "src/distributed_ledger/ledger.h"
#include "src/proof_of_authority/poa.h"

#include <cstdio>

int startUI();
void error_callback(int error, const char* description);
void show_propose_block_window(PoA &poa, Ledger &ledger, bool &show_window, bool &propose_success,
                               std::string &propose_message);
void show_inventory_as_table(const Inventory &inventory, int& selected_inventory);

#endif //BLOCKCHAIN_A2_UI_H
