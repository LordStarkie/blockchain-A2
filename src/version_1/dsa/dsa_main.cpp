#include "dsa_key_generation.h"
#include "dsa_signing.h"

void dsa()
{
    BIGNUM *p, *q, *g, *v, *d;
    generate_key_pair(&p, &q, &g, &v, &d);

    BIGNUM *r,*s,*m;
    key_sign(p,q,g,d,&r,&s,&m);

}