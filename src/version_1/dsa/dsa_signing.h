#ifndef BLOCKCHAIN_A2_DSA_SIGNING_H
#define BLOCKCHAIN_A2_DSA_SIGNING_H
#include <openssl/types.h>

void key_sign(const BIGNUM *p, const BIGNUM *q, const BIGNUM *g, const BIGNUM *m);
bool calulate_r(BIGNUM** r,const BIGNUM *g, const BIGNUM *e, const BIGNUM *p, const BIGNUM *q);
bool calulate_s(BIGNUM **s, const BIGNUM *m, const BIGNUM *d, const BIGNUM *r, const BIGNUM *e, const BIGNUM *q);
bool select_e(BIGNUM **e, const BIGNUM *p);

#endif //BLOCKCHAIN_A2_DSA_SIGNING_H
