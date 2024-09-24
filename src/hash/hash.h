#ifndef HASH_H
#define HASH_H

#include <cstddef>
#include <openssl/evp.h>
#include <openssl/err.h>

void digest_message(const unsigned char *message, size_t message_len, unsigned char **digest, unsigned int *digest_len);
void sha256_to_decimal(const unsigned char *digest, unsigned int digest_len, char **decimal_str);
void handleErrors();

#endif //HASH_H
