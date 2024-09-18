#include "dsa_key_generation.h"

void dsa()
{
    // MAIN PARAMS NEEDED
    // P,Q,G V + D
    BIGNUM *p, *q, *g, *v;
    generateKeyPair(&p, &q, &g, &v);
}