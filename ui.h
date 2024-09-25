#ifndef BLOCKCHAIN_A2_UI_H
#define BLOCKCHAIN_A2_UI_H

#include "src/distributed_ledger/ledger.h"
#include "src/version_1/proof_of_authority/poa.h"

#include <cstdio>

int startUI();
void error_callback(int error, const char* description);
void show_inventory_as_table(const Inventory & inventories);



#endif //BLOCKCHAIN_A2_UI_H
