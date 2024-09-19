#include "dsa_verification.h"
#include <openssl/bn.h>

bool key_verification(const BIGNUM *m, const BIGNUM *s, const BIGNUM *r, const BIGNUM *g, const BIGNUM *v,
                      const BIGNUM *p, const BIGNUM *q) {
    // INIT
    BIGNUM *v1 = BN_new();
    BIGNUM *v2 = BN_new();
    BIGNUM *x = BN_new();

    char *v1_str = nullptr;
    char *v2_str = nullptr;
    char *x_str = nullptr;

    if (!key_generate_v1(&v1, m, s, q))
    {
        fprintf(stderr, "v1 generation failed\n");
        goto cleanup;
    }

    if (!key_generate_v2(&v2, r, s, q))
    {
        fprintf(stderr, "v2 generation failed\n");
        goto cleanup;
    }

    if (!key_generate_x(&x, v1, v2, g, v, p, q))
    {
        fprintf(stderr, "v2 generation failed\n");
        goto cleanup;
    }

    // Print generated values for testing
    v1_str = BN_bn2dec(v1);
    v2_str = BN_bn2dec(v2);
    x_str = BN_bn2dec(x);

    printf("Generated v1: %s\n", v1_str);
    printf("Generated v2: %s\n", v2_str);
    printf("Generated x: %s\n", x_str);

    // Free string representations
    OPENSSL_free(v1_str);
    OPENSSL_free(v2_str);
    OPENSSL_free(x_str);

    if (BN_cmp(x, r) == 0){
        BN_free(v1);
        BN_free(v2);
        BN_free(x);
        return true;
    } else {
        BN_free(v1);
        BN_free(v2);
        BN_free(x);
        return false;
    }

    cleanup:
        BN_free(v1);
        BN_free(v2);
        BN_free(x);
        return false;
}
bool key_generate_x(BIGNUM **x, const BIGNUM *v1, const BIGNUM *v2, const BIGNUM *g,
                    const BIGNUM *v, const BIGNUM *p, const BIGNUM *q) {
    BN_CTX *ctx = BN_CTX_new();
    if (!ctx) return false;

    // Init
    BIGNUM *g_v1 = BN_new();
    BIGNUM *v_v2 = BN_new();
    BIGNUM *gv1_vv2_mod_p = BN_new();

    // g_v1 = g^v1 mod p
    BN_mod_exp(g_v1, g, v1, p, ctx);
    // v_v2 = v^v2 mod p
    BN_mod_exp(v_v2, v, v2, p, ctx);
    // (g_v1 * v_v2) mod p
    BN_mod_mul(gv1_vv2_mod_p, g_v1, v_v2, p, ctx);
    // x = ((g_v1 * v_v2) mod p) mod q
    BN_mod(*x, gv1_vv2_mod_p, q, ctx);

    // Clean up
    BN_free(g_v1);
    BN_free(v_v2);
    BN_free(gv1_vv2_mod_p);
    BN_CTX_free(ctx);
    return true;
}

bool key_generate_v1(BIGNUM **v1, const BIGNUM *m, const BIGNUM *s, const BIGNUM *q) {
    BN_CTX *ctx = BN_CTX_new();
    if (!ctx) return false;

    // Init
    BIGNUM *s_inverse = BN_new();

    // s^-1 mod q
    BN_mod_inverse(s_inverse, s,q, ctx);
    // m * (s^-1 mod q) mod q
    BN_mod_mul(*v1, m, s_inverse,q, ctx);

    // Cleanup
    BN_free(s_inverse);
    BN_CTX_free(ctx);
    return true;
}

bool key_generate_v2(BIGNUM **v2, const BIGNUM *r, const BIGNUM *s, const BIGNUM *q){
    BN_CTX *ctx = BN_CTX_new();
    if (!ctx) return false;

    // Init
    BIGNUM *s_inverse = BN_new();

    // s^-1 mod q
    BN_mod_inverse(s_inverse, s,q, ctx);
    // r * (s^-1 mod q) mod q
    BN_mod_mul(*v2, r, s_inverse,q, ctx);

    // Cleanup
    BN_free(s_inverse);
    BN_CTX_free(ctx);
    return true;
}



