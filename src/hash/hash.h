#pragma once
#include <cstddef>

#ifndef HASH_H
#define HASH_H

int run();
// Taken from official OpenSSL documentation: https://wiki.openssl.org/index.php/EVP_Message_Digests
inline void digest_message(const unsigned char *message, size_t message_len, unsigned char **digest, unsigned int *digest_len);
// print hash value
inline void printDigest(const unsigned char* digest, unsigned int digest_len);
inline void handleErrors();

#endif //HASH_H
