#include "dsa_signing.h"
#include <openssl/bn.h>

void key_sign(const BIGNUM *p, const BIGNUM *q, const BIGNUM *g, const BIGNUM *d, BIGNUM **r, BIGNUM **s, BIGNUM **m)
{
    // Init
    *r = BN_new();
    *s = BN_new();
    *m = BN_new();
    BIGNUM *e = BN_new();

    char *m_str = nullptr;
    char *e_str = nullptr;
    char *r_str = nullptr;
    char *s_str = nullptr;

    // SET VALUES FOR TESTING
    BN_set_word(*m, 343);

    // GENERATE E
    if (!generate_e(&e, p))
    {
        fprintf(stderr, "e generation failed\n");
        goto cleanup;
    }

    // GENERATE R
    if (!generate_r(r,g,e,p,q))
    {
        fprintf(stderr, "r generation failed\n");
        goto cleanup;
    }

    // GENERATE S
    if (!generate_s(s, *m, d, *r, e, q))
    {
        fprintf(stderr, "r generation failed\n");
        goto cleanup;
    }

    // Print generated values for testing
    m_str = BN_bn2dec(*m);
    e_str = BN_bn2dec(e);
    r_str = BN_bn2dec(*r);
    s_str = BN_bn2dec(*s);

    printf("Generated m: %s\n", m_str);
    printf("Generated e: %s\n", e_str);
    printf("Generated r: %s\n", r_str);
    printf("Generated s: %s\n", s_str);


    cleanup:
    // Free string representations
    OPENSSL_free(m_str);
    OPENSSL_free(e_str);
    OPENSSL_free(r_str);
    OPENSSL_free(s_str);
    BN_free(e);
}


bool generate_r(BIGNUM** r,const BIGNUM *g, const BIGNUM *e, const BIGNUM *p, const BIGNUM *q)
{
    BN_CTX *ctx = BN_CTX_new();
    if (!ctx) return false;

    // Init
    *r = BN_new();
    BIGNUM *mod_p = BN_new();

    BN_mod_exp(mod_p, g, e, p,ctx);
    BN_mod(*r, mod_p, q, ctx);

    // Clean up
    BN_free(mod_p);
    BN_CTX_free(ctx);

    return true;
}

bool generate_s(BIGNUM **s, const BIGNUM *m, const BIGNUM *d, const BIGNUM *r, const BIGNUM *e, const BIGNUM *q)
{
    BN_CTX *ctx = BN_CTX_new();
    if (!ctx) return false;

    // Init
    BIGNUM *e_1_mod_q = BN_new();
    BIGNUM *dr = BN_new();
    BIGNUM *m_dr = BN_new();

    // e^1 mod q
    BN_mod_inverse(e_1_mod_q, e, q, ctx);
    // d*r
    BN_mul(dr, d, r, ctx);
    // m + dr
    BN_add(m_dr, m, dr);
    // (m+dr) * (e^-1 mod q) mod q
    BN_mod_mul(*s, m_dr, e_1_mod_q, q, ctx);

    // Cleanup
    BN_free(e_1_mod_q);
    BN_free(dr);
    BN_free(m_dr);
    BN_CTX_free(ctx);

    return true;
}

bool generate_e(BIGNUM **e, const BIGNUM *p)
{
    BN_CTX *ctx = BN_CTX_new();
    if (!ctx) return false;

    // Init
    BIGNUM *phi_p = BN_new();
    BIGNUM *one = BN_new();
    BN_one(one);
    BN_sub(phi_p, p, one);
    *e = BN_new();

    do {
        if (!BN_rand_range(*e, phi_p)) {
            // Error
            BN_free(*e);
            BN_free(one);
            BN_free(phi_p);
            BN_CTX_free(ctx);
            return false;
        }
        // Make sure e is not 0
    } while (BN_is_one(*e) || BN_is_zero(*e));

    // SET VALUES FOR TESTING
    BN_set_word(*e,427);

    // Cleanup
    BN_free(one);
    BN_free(phi_p);
    BN_CTX_free(ctx);

    return true;
}

