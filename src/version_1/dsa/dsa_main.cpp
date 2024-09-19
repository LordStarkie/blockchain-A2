#include "dsa_key_generation.h"
#include "dsa_signing.h"
#include "dsa_verification.h"

#include <iostream>
#include <openssl/bn.h>


void dsa()
{
    // Key Generation
    BIGNUM *p, *q, *g, *v, *d;
    generate_key_pair(&p, &q, &g, &v, &d);

    // Signing
    BIGNUM *r,*s, *m = BN_new();
    // Set Message
    BN_set_word(m, 2131223);
    key_sign(p,q,g,d,&r,&s,&m);

    // Verification
    if (key_verification(m, s, r, g, v, p, q)) {
        std::cout << "Message verification successful" << std::endl;
    } else {
        std::cout << "Message verification unsuccessful" << std::endl;
    }

    // Cleanup
    BN_free(p); BN_free(q); BN_free(g); BN_free(v);
    BN_free(d); BN_free(r); BN_free(s); BN_free(m);
}