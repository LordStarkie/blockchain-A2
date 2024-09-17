#ifndef DIGITAL_SIGNATURE_H
#define DIGITAL_SIGNATURE_H
#include <openssl/types.h>

void generateKeyPair();
bool generate_primes(BIGNUM **p, BIGNUM **q, int bits, int q_bits);
bool is_primitive_root(const BIGNUM *h, const BIGNUM *p, BN_CTX *ctx);
bool generate_g(const BIGNUM *p, const BIGNUM *q, BIGNUM **g, BIGNUM **h);
bool generate_d(const BIGNUM *q, BIGNUM **d);
bool generate_v(const BIGNUM *g, const BIGNUM *d, const BIGNUM *p, BIGNUM **v);

#endif //DIGITAL_SIGNATURE_H
