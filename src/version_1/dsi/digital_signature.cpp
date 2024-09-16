#include "digital_signature.h"

#include <iostream>
#include <openssl/bn.h>

void generateKeyPair() {
    BIGNUM *p = nullptr, *q = nullptr;
    char *p_str = nullptr, *q_str = nullptr;

    // Generate p and q with the specified bit lengths
    if (!generate_primes(&p, &q, 4028, 224)) {
        fprintf(stderr, "Prime generation failed\n");
        return;
    }

    // Convert p and q to decimal strings
    p_str = BN_bn2dec(p);
    q_str = BN_bn2dec(q);

    printf("Generated Large Prime p: %s\n", p_str);
    printf("Generated Smaller Prime q: %s\n", q_str);

    // Free memory
    BN_free(p);
    BN_free(q);
    OPENSSL_free(p_str);
    OPENSSL_free(q_str);
}

// Generate a pair of primes where p = 1 mod q and p,q is exactly p_bits,q_bits in size
bool generate_primes(BIGNUM **p, BIGNUM **q, const int p_bits, const int q_bits) {
    BN_CTX *ctx = BN_CTX_new();
    if (!ctx) return false;

    // Generate q
    *q = BN_new();
    if (!BN_generate_prime_ex(*q, q_bits, 0, NULL, NULL, NULL)) {
        fprintf(stderr, "Failed to generate q\n");
        BN_free(*q);
        BN_CTX_free(ctx);
        return false;
    }

    // generate p
    *p = BN_new();
    BIGNUM *tmp = BN_new();
    BIGNUM *rem = BN_new();

    // store 2*q
    BIGNUM *two_q = BN_new();

    // store 1
    BIGNUM *one = BN_new();
    BN_one(one);

    // copy and left shift to multiply by 2
    BN_copy(two_q, *q);
    BN_lshift1(two_q, two_q);

    bool found = false;
    while (!found) {
        // Generate random p with size p_bits
        if (!BN_rand(*p, p_bits, BN_RAND_TOP_ONE, BN_RAND_BOTTOM_ODD)) {
            fprintf(stderr, "Failed to generate random p\n");
            BN_free(*q);
            BN_free(*p);
            BN_CTX_free(ctx);
            return false;
        }

        // Adjust p so p = 1modq
        // rem = p % 2q
        BN_mod(rem, *p, two_q, ctx);
        // tmp = rem - 1
        BN_sub(tmp, rem, one);
        // p = p - tmp
        BN_sub(*p, *p, tmp);

        // check if p is prime
        if (BN_is_prime_ex(*p, BN_prime_checks, ctx, NULL)) {
            found = true;
        }
    }

    // Clean up
    BN_free(tmp);
    BN_free(rem);
    BN_free(two_q);
    BN_free(one);
    BN_CTX_free(ctx);

    return true;
}
