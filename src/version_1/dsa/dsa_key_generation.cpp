#include "dsa_key_generation.h"
#include <openssl/bn.h>

void generateKeyPair(BIGNUM **p, BIGNUM **q, BIGNUM **g, BIGNUM **v) {
    // Allocate memory for BIGNUMs if not already allocated
    *p = BN_new();
    *q = BN_new();
    *g = BN_new();
    *v = BN_new();

    BIGNUM *d = BN_new();
    BIGNUM *h = BN_new();

    char *p_str = nullptr;
    char *q_str = nullptr;
    char *g_str = nullptr;
    char *d_str = nullptr;
    char *v_str = nullptr;

    // GENERATE P, Q
    if (!generate_primes(p, q, 1024, 160)) {
        fprintf(stderr, "Prime generation failed\n");
        goto cleanup;
    }

    // GENERATE G
    if (!generate_g(*p, *q, g, &h)) {
        fprintf(stderr, "g generation failed\n");
        goto cleanup;
    }

    // GENERATE D
    if (!generate_d(*q, &d)) {
        fprintf(stderr, "d generation failed\n");
        goto cleanup;
    }

    // GENERATE V
    if (!generate_v(*g, d, *p, v)) {
        fprintf(stderr, "v generation failed\n");
        goto cleanup;
    }

    // Print generated values for testing
    p_str = BN_bn2dec(*p);
    q_str = BN_bn2dec(*q);
    g_str = BN_bn2dec(*g);
    d_str = BN_bn2dec(d);
    v_str = BN_bn2dec(*v);

    printf("Generated p: %s\n", p_str);
    printf("Generated q: %s\n", q_str);
    printf("Generated g: %s\n", g_str);
    printf("Generated d: %s\n", d_str);
    printf("Generated v: %s\n", v_str);

    // Free string representations
    OPENSSL_free(p_str);
    OPENSSL_free(q_str);
    OPENSSL_free(g_str);
    OPENSSL_free(d_str);
    OPENSSL_free(v_str);

    cleanup:
    // Free memory for BIGNUMs
    BN_free(d);
    BN_free(h);
}

// Generate a pair of primes where p = 1 mod q and p,q is exactly p_bits,q_bits in size
bool generate_primes(BIGNUM **p, BIGNUM **q, const int p_bits, const int q_bits) {
    BN_CTX *ctx = BN_CTX_new();
    if (!ctx) return false;

    *q = BN_new();
    if (!*q || !BN_generate_prime_ex(*q, q_bits, 0, nullptr, nullptr, nullptr)) {
        fprintf(stderr, "Failed to generate q\n");
        if (*q) BN_free(*q);
        BN_CTX_free(ctx);
        return false;
    }

    // Generate p and placeholders
    *p = BN_new();
    BIGNUM *tmp = BN_new();
    BIGNUM *rem = BN_new();
    BIGNUM *two_q = BN_new();
    BIGNUM *one = BN_new();
    BIGNUM *gcd = BN_new(); // GCD placeholder
    if (!*p || !tmp || !rem || !two_q || !one || !gcd) {
        fprintf(stderr, "Failed to allocate BIGNUMs\n");
        BN_free(*q); BN_free(*p); BN_free(tmp); BN_free(rem);
        BN_free(two_q); BN_free(one); BN_free(gcd);
        BN_CTX_free(ctx);
        return false;
    }

    BN_one(one);
    BN_copy(two_q, *q);
    BN_lshift1(two_q, two_q);

    bool found = false;
    while (!found) {
        if (!BN_rand(*p, p_bits, BN_RAND_TOP_ONE, BN_RAND_BOTTOM_ODD)) {
            fprintf(stderr, "Failed to generate random p\n");
            BN_free(*q); BN_free(*p); BN_free(tmp); BN_free(rem);
            BN_free(two_q); BN_free(one); BN_free(gcd);
            BN_CTX_free(ctx);
            return false;
        }

        BN_mod(rem, *p, two_q, ctx);
        BN_sub(tmp, rem, one);
        BN_sub(*p, *p, tmp);

        if (BN_is_prime_ex(*p, BN_prime_checks, ctx, nullptr)) {
            BN_gcd(gcd, *p, *q, ctx);  // Calculate the GCD
            if (BN_is_one(gcd)) {
                found = true;  // Ensure gcd(p, q) = 1
            }
        }
    }

    // Clean up
    BN_free(tmp); BN_free(rem); BN_free(two_q); BN_free(one); BN_free(gcd);
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

bool generate_d(const BIGNUM *q, BIGNUM **d)
{
    BN_CTX *ctx = BN_CTX_new();
    if (!ctx) return false;

    *d = BN_new();
    BIGNUM *one = BN_new();
    BIGNUM *q_minus_1 = BN_new();
    if (!*d || !one || !q_minus_1) {
        BN_free(*d);
        BN_free(one);
        BN_free(q_minus_1);
        BN_CTX_free(ctx);
        return false;
    }

    BN_one(one);
    BN_sub(q_minus_1, q, one);

    do {
        if (!BN_rand_range(*d, q_minus_1)) {
            // Error
            BN_free(*d);
            BN_free(one);
            BN_free(q_minus_1);
            BN_CTX_free(ctx);
            return false;
        }
        // Make sure d is not 0
    } while (BN_is_one(*d) || BN_is_zero(*d));

    // Cleanup
    BN_free(one);
    BN_free(q_minus_1);
    BN_CTX_free(ctx);

    return true;
}

bool generate_v(const BIGNUM *g, const BIGNUM *d, const BIGNUM *p, BIGNUM **v) {
    BN_CTX *ctx = BN_CTX_new();
    if (!ctx) return false;

    *v = BN_new();

    // Calculate v = g^d mod p
    if (!BN_mod_exp(*v, g, d, p, ctx)) {
        fprintf(stderr, "Failed to compute g^d mod p\n");
        BN_free(*v);
        BN_CTX_free(ctx);
        return false;
    }

    // Clean up
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