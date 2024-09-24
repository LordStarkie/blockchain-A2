#ifndef BLOCKCHAIN_A2_DSA_KEY_GENERATION_H
#define BLOCKCHAIN_A2_DSA_KEY_GENERATION_H
#include <openssl/types.h>
#include "../../distributed_ledger/transaction.h"

void generate_key_pair(BIGNUM **p, BIGNUM **q, BIGNUM **g, BIGNUM **v, BIGNUM **d);
bool generate_primes(BIGNUM **p, BIGNUM **q, int bits, int q_bits);
bool is_primitive_root(const BIGNUM *h, const BIGNUM *p, BN_CTX *ctx);
bool generate_g(const BIGNUM *p, const BIGNUM *q, BIGNUM **g, BIGNUM **h);
bool generate_d(const BIGNUM *q, BIGNUM **d);
bool generate_v(const BIGNUM *g, const BIGNUM *d, const BIGNUM *p, BIGNUM **v);

#endif //BLOCKCHAIN_A2_DSA_KEY_GENERATION_H
