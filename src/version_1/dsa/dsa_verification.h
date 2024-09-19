#ifndef BLOCKCHAIN_A2_DSA_VERIFICATION_H
#define BLOCKCHAIN_A2_DSA_VERIFICATION_H
#include <openssl/types.h>

bool key_verification(const BIGNUM *m, const BIGNUM *s, const BIGNUM *r, const BIGNUM *g, const BIGNUM *v,
                      const BIGNUM *p, const BIGNUM *q);

bool key_generate_x(BIGNUM **x, const BIGNUM *v1, const BIGNUM *v2, const BIGNUM *g,
                    const BIGNUM *v, const BIGNUM *p, const BIGNUM *q);

bool key_generate_v1(BIGNUM **v1, const BIGNUM *m, const BIGNUM *s, const BIGNUM *q);
bool key_generate_v2(BIGNUM **v2, const BIGNUM *r, const BIGNUM *s, const BIGNUM *q);


#endif //BLOCKCHAIN_A2_DSA_VERIFICATION_H