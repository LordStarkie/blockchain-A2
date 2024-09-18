#include "dsa_signing.h"
#include <openssl/bn.h>

void key_sign(const BIGNUM *p, const BIGNUM *q, const BIGNUM *g, const BIGNUM *m)
{
    // Store e in memory
    BIGNUM *e = nullptr;
    // GENERATE E
    if (!select_e(&e,p))
    {
        fprintf(stderr, "e generation failed\n");
        return;
    }

    // Store r in memory
    BIGNUM *r = nullptr;
    // GENERATE R
    if (!calulate_r(&r,g,e,p,q))
    {
        fprintf(stderr, "r generation failed\n");
        return;
    }

    // Print e for testing
    char *e_str = BN_bn2dec(e);
    printf("Generated e: %s\n", e_str);
    // Print r for testing
    char *r_str = BN_bn2dec(r);
    printf("Generated r: %s\n", r_str);

    // Clean up

}

bool calulate_r(BIGNUM** r,const BIGNUM *g, const BIGNUM *e, const BIGNUM *p, const BIGNUM *q)
{
    BN_CTX *ctx = BN_CTX_new();
    if (!ctx) return false;

    *r = BN_new();
    BIGNUM *mod_p = BN_new();

    BN_mod_exp(mod_p, g, e, p,ctx);
    BN_mod(*r, mod_p, q, ctx);

    // Clean up
    BN_free(mod_p);

    return true;
}

bool calulate_s(BIGNUM **s, const BIGNUM *m, const BIGNUM *d, const BIGNUM *r, const BIGNUM *e, const BIGNUM *q);

bool select_e(BIGNUM **e, const BIGNUM *p)
{
    BN_CTX *ctx = BN_CTX_new();
    if (!ctx) return false;

    // phi_p = p-1
    BIGNUM *phi_p = BN_new();
    BIGNUM *one = BN_new();
    BN_one(one);
    BN_sub(phi_p, p, one);

    // Init r
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

    // Cleanup
    BN_free(one);
    BN_free(phi_p);
    BN_CTX_free(ctx);

    return true;
}

