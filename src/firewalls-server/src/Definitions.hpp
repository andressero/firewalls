// Copyright [2024] <Andres Quesada, Pablo Cascante, Diego Bolanos,
// Andres Serrano>"

#ifndef DEFINITIONS_HPP
#define DEFINITIONS_HPP

#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#define BLOCK_SIZE 256
#define FILE_COUNT 100
// Hundred files of 256 bytes each
#define STORAGE_VOLUME FILE_COUNT *BLOCK_SIZE
#define BLOCK_COUNT (STORAGE_VOLUME / BLOCK_SIZE)

#define DEBUG 1

#define ERROR(message)                                                         \
  std::cerr << "\033[1;31m"                                                    \
            << "ERROR: " << message << "\033[0m" << std::endl;

#define LOG(message)                                                           \
  if (DEBUG)                                                                   \
    std::cerr << "\033[1;33m"                                                  \
              << "LOG: " << message << "\033[0m" << std::endl;

#define LAST_BLOCK -1
#define UNUSED -2
#define NONE -3
#define OPEN 0
#define CLOSED -1
#define ERROR_CODE -1
using std::string;
// Type Definitions
typedef int64_t i64;
typedef uint64_t u64;
typedef uint8_t u8;
// ? Why "POINTER"?
typedef int64_t BLOCK_POINTER;
typedef int64_t DIRECTORY_POINTER;
typedef int64_t FAT_POINTER;
typedef int64_t UNIT_POINTER;

#endif
