#pragma once
#include <cstddef>

#ifndef HASH_H
#define HASH_H

int run();
inline void digest_message(const unsigned char *message, size_t message_len, unsigned char **digest, unsigned int *digest_len);
inline void printDigest(const unsigned char* digest, unsigned int digest_len);
inline void handleErrors();

#endif //HASH_H
