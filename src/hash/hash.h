#ifndef HASH_H
#define HASH_H

#include <cstddef>
#include <openssl/evp.h>
#include <openssl/err.h>

int run();
void digest_message(const unsigned char *message, size_t message_len, unsigned char **digest, unsigned int *digest_len);
void printDigest(const unsigned char* digest, unsigned int digest_len);
void handleErrors();

#endif //HASH_H
