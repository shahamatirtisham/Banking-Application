#include "hash.hpp"

#include <openssl/evp.h>
#include <openssl/err.h>

#include <sstream>
#include <iomanip>
#include <stdexcept>

std::string sha256(const std::string& input)
{
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    if (!ctx)
        throw std::runtime_error("EVP_MD_CTX_new failed");

    const EVP_MD* md = EVP_sha256();
    if (!md)
        throw std::runtime_error("EVP_sha256 failed");

    if (EVP_DigestInit_ex(ctx, md, nullptr) != 1)
        throw std::runtime_error("EVP_DigestInit_ex failed");

    if (EVP_DigestUpdate(ctx, input.data(), input.size()) != 1)
        throw std::runtime_error("EVP_DigestUpdate failed");

    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len = 0;

    if (EVP_DigestFinal_ex(ctx, hash, &hash_len) != 1)
        throw std::runtime_error("EVP_DigestFinal_ex failed");

    EVP_MD_CTX_free(ctx);

    // convert to hex string
    std::ostringstream oss;
    for (unsigned int i = 0; i < hash_len; ++i)
    {
        oss << std::hex
            << std::setw(2)
            << std::setfill('0')
            << static_cast<int>(hash[i]);
    }

    return oss.str();
}
