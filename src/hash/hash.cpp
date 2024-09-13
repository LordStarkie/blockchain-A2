#include <openssl/evp.h>
#include <openssl/err.h>

#include <string>
#include <iomanip>
#include <iostream>
#include <cstring>

#include "hash.h"

// https://wiki.openssl.org/index.php/EVP_Message_Digests
void digest_message(const unsigned char *message, const size_t message_len, unsigned char **digest, unsigned int *digest_len)
{
    EVP_MD_CTX *mdctx;

    if((mdctx = EVP_MD_CTX_new()) == nullptr)
        handleErrors();

    if(1 != EVP_DigestInit_ex(mdctx, EVP_sha256(), nullptr))
        handleErrors();

    if(1 != EVP_DigestUpdate(mdctx, message, message_len))
        handleErrors();

    if((*digest = static_cast<unsigned char *>(OPENSSL_malloc(EVP_MD_size(EVP_sha256())))) == nullptr)
        handleErrors();

    if(1 != EVP_DigestFinal_ex(mdctx, *digest, digest_len))
        handleErrors();

    EVP_MD_CTX_free(mdctx);
}

// print hash value
void printDigest(const unsigned char* digest, const unsigned int digest_len) {
    std::cout << "Digest is: ";
    for (unsigned int i = 0; i < digest_len; i++) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(digest[i]);
    }
    std::cout << std::endl;
}

inline void handleErrors()
{
    ERR_print_errors_fp(stderr);
    abort();
}

int run() {
    const char* message = "Hello";

    // pointer to hash
    unsigned char* digest;
    // pointer to hash length
    unsigned int digest_len;

    // call to run hash function
    digest_message(reinterpret_cast<const unsigned char*>(message), strlen(message), &digest, &digest_len);
    printDigest(digest, digest_len);

    // free memory
    OPENSSL_free(digest);
    return 0;
}
