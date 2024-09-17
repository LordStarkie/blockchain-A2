#include "dsa_key_generation.h"
#include <openssl/bn.h>

void generateKeyPair() {
    // Store p and q parameters in memory
    BIGNUM *p = nullptr, *q = nullptr;
    // GENERATE P AND Q
    if (!generate_primes(&p, &q, 1024, 160)) {
        fprintf(stderr, "Prime generation failed\n");
        return;
    }

    // Store g,h in memory
    BIGNUM *g = nullptr, *h = nullptr;
    // GENERATE G
    if (!generate_g(p, q, &g, &h)) {
        fprintf(stderr, "g generation failed\n");
        return;
    }

    // Store d in memory
    BIGNUM *d = nullptr;
    // GENERATE D
    if (!generate_d(q,&d)) {
        fprintf(stderr, "d generation failed\n");
        return;
    }

    // Store v in memory
    BIGNUM *v = nullptr;
    // GENERATE V
    if (!generate_v(g,d,p,&v)) {
        fprintf(stderr, "v generation failed\n");
        return;
    }

    // Print p,q for testing
    char *p_str = BN_bn2dec(p), *q_str = BN_bn2dec(q);;
    printf("Generated p: %s\n", p_str);
    printf("Generated q: %s\n", q_str);
    // Print g for testing
    char *g_str = BN_bn2dec(g);
    printf("Generated g: %s\n", g_str);
    // Print d for testing
    char *d_str = BN_bn2dec(d);
    printf("Generated d: %s\n", d_str);
    // Print v for testing
    char *v_str = BN_bn2dec(v);
    printf("Generated v: %s\n", v_str);

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
