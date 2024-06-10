// Copyright [2024] <Andres Quesada, Pablo Cascante, Diego Bolanos, Andres
// Serrano>"
#ifndef AUTHUTILS_HPP
#define AUTHUTILS_HPP

#include "FileSystem.hpp"
#include "SHA256.hpp"

inline std::string sha256ToString(const BYTE *hash) {
  std::string answer(2 * SHA256_BLOCK_SIZE, '0');
  for (size_t i = 0; i < SHA256_BLOCK_SIZE; ++i) {
    char temp[3] = {0};
    snprintf(temp, 3, "%02x", hash[i]);
    answer[2 * i] = temp[0];
    answer[2 * i + 1] = temp[1];
  }
  return answer;
}

inline std::string sha256Hash(std::string &strInput) {
  const BYTE *input = reinterpret_cast<const BYTE *>(strInput.data());

  BYTE hash[SHA256_BLOCK_SIZE];
  SHA256_CTX context;
  sha256_init(&context);
  sha256_update(&context, input, strInput.size());
  sha256_final(&context, hash);
  return sha256ToString(hash);
}

#endif // AUTHUTILS_HPP