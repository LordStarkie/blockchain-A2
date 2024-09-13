#include "digital_signature.h"

#include <iostream>
#include <openssl/dsa.h>
#include <openssl/rand.h>

// A.i)
void generateKeyPair()
{
    // inits DSA structure: https://docs.openssl.org/1.1.1/man3/DSA_new/#name
    DSA *dsa = DSA_new();

    constexpr int bits = 2048;
    // seed has to be longer than p,q
    constexpr unsigned int seed_len = 32;
    unsigned char seed[seed_len];

    // generate random seed: https://docs.openssl.org/1.1.1/man3/RAND_bytes/
    if (RAND_bytes(seed, seed_len) != 1) {
        throw std::invalid_argument("Error generating random seed.");
    }

    // generate keys: https://docs.openssl.org/1.1.1/man3/DSA_generate_parameters/
    DSA_generate_parameters_ex(dsa,bits,seed, seed_len, nullptr, nullptr,nullptr);
    DSA_generate_key(dsa);

    // free memory
    DSA_free(dsa);
}


