#ifndef DIGITAL_SIGNATURE_H
#define DIGITAL_SIGNATURE_H
#include <openssl/types.h>

void generateKeyPair();
bool generate_primes(BIGNUM **p, BIGNUM **q, const int bits, const int q_bits);

void signRecord();
void verifyRecord();

#endif //DIGITAL_SIGNATURE_H
