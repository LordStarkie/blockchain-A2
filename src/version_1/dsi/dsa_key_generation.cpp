#include "dsa_key_generation.h"

#include <openssl/bn.h>

void generateKeyPair() {
    // Store p and q parameters in memory
    BIGNUM *p = nullptr, *q = nullptr;

    // Generate p and q with the specified bit lengths
    if (!generate_primes(&p, &q, 1024, 160)) {
        fprintf(stderr, "Prime generation failed\n");
        return;
    }

    // Print p,q for testing
    char *p_str = BN_bn2dec(p), *q_str = BN_bn2dec(q);;
    printf("Generated Large Prime p: %s\n", p_str);
    printf("Generated Smaller Prime q: %s\n", q_str);

    // Store g,h in memory
    BIGNUM *g = nullptr, *h = nullptr;

    // Generate g
    if (!generate_g(p, q, &g, &h)) {
        fprintf(stderr, "g generation failed\n");
        return;
    }

    // Print g for testing
    char *g_str = BN_bn2dec(g);
    printf("Generated g: %s\n", g_str);

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
    if (!BN_generate_prime_ex(*q, q_bits, 0, nullptr, nullptr, nullptr)) {
        fprintf(stderr, "Failed to generate q\n");
        BN_free(*q);
        BN_CTX_free(ctx);
        return false;
    }

    // Generate p
    *p = BN_new();
    BIGNUM *tmp = BN_new();
    BIGNUM *rem = BN_new();

    // Store 2*q
    BIGNUM *two_q = BN_new();

    // Store 1
    BIGNUM *one = BN_new();
    BN_one(one);

    // Copy and left shift to multiply by 2
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

        // Check if p is prime
        if (BN_check_prime(*p, ctx,nullptr)) {
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

bool generate_g(const BIGNUM *p, const BIGNUM *q, BIGNUM **g, BIGNUM **h) {
    BN_CTX *ctx = BN_CTX_new();
    if (!ctx) return false;

    BIGNUM *phi_p = BN_new();
    BIGNUM *one = BN_new();
    BN_one(one);
    BN_sub(phi_p, p, one);

    *h = BN_new();

    // Find primitive root
    do {
        if (!BN_rand_range(*h, phi_p) || BN_is_one(*h)) {
            continue;
        }
    } while (!is_primitive_root(*h, p, ctx));

    // Calculate
    *g = BN_new();
    BIGNUM *h_pow = BN_new();
    BIGNUM *phi_p_div_q = BN_new();
    BN_div(phi_p_div_q, nullptr, phi_p, q, ctx);
    BN_mod_exp(h_pow, *h, phi_p_div_q, p, ctx);
    BN_mod(*g, h_pow, p, ctx);

    // Clean up
    BN_free(h_pow);
    BN_free(phi_p_div_q);
    BN_free(phi_p);
    BN_free(one);
    BN_CTX_free(ctx);

    return true;
}

bool is_primitive_root(const BIGNUM *h, const BIGNUM *p, BN_CTX *ctx) {
    BIGNUM *phi_p = BN_new();
    BIGNUM *one = BN_new();
    BN_one(one);
    BN_sub(phi_p, p, one);

    // Find primitive root h
    bool result = true;
    for (BIGNUM *i = BN_new(); BN_cmp(i, phi_p) <= 0; BN_add(i, i, one)) {
        if (BN_is_one(i) || !BN_gcd(i, phi_p, i, ctx) || BN_is_one(i)) {
            BIGNUM *exp_res = BN_new();
            if (!BN_mod_exp(exp_res, h, i, p, ctx) || BN_is_one(exp_res)) {
                result = false;
                BN_free(exp_res);
                break;
            }
            BN_free(exp_res);
        }
    }

    // Clean Up
    BN_free(phi_p);
    BN_free(one);
    return result;
}
