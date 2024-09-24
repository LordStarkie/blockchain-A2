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

// convert SHA-256 hash to decimal string
void sha256_to_decimal(const unsigned char *digest, unsigned int digest_len, char **decimal_str) {
    BIGNUM *bn = BN_new();
    if (bn == nullptr) {
        throw std::runtime_error("Failed to create BIGNUM");
    }

    BN_bin2bn(digest, digest_len, bn);

    *decimal_str = BN_bn2dec(bn);
    if (*decimal_str == nullptr) {
        BN_free(bn);
        throw std::runtime_error("Failed to convert BIGNUM to decimal string");
    }

    BN_free(bn);
}

void handleErrors()
{
    ERR_print_errors_fp(stderr);
    abort();
}