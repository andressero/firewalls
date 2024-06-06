//
// Blowfish C++ implementation
//
// CC0 - PUBLIC DOMAIN
// This work is free of known copyright restrictions.
// http://creativecommons.org/publicdomain/zero/1.0/
//

#ifndef __blowfish__
#define __blowfish__

#include <stdint.h>
#include <string>
#include <numeric> 
#include "Base64.hpp"

class Blowfish {
public:
  void setKey(const std::string& key);
  void encrypt(std::string& dst, const std::string& src) const;
  void decrypt(std::string& dst, const std::string& src) const;

private:
  void setKey(const unsigned char* key, int byte_length);
  void encrypt(unsigned char* dst, const unsigned char* src, int byte_length) const;
  void decrypt(unsigned char* dst, const unsigned char* src, int byte_length) const;
  void encryptBlock(uint32_t *left, uint32_t *right) const;
  void decryptBlock(uint32_t *left, uint32_t *right) const;
  uint32_t feistel(uint32_t value) const;
    
private:
  uint32_t pary_[18];
  uint32_t sbox_[4][256];
};

#endif /* defined(__blowfish__) */
