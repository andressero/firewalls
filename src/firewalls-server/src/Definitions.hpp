// Copyright [2024] <Andres Quesada, Pablo Cascante, Diego Bolanos,
// Andres Serrano>"

#ifndef DEFINITIONS_HPP
#define DEFINITIONS_HPP

#include <cstdint>
#include <cstdlib>
#include <fstream>
// ! Check
#include <iomanip> 
//
#include <iostream>
#include <string>

#define ASSERT(condition)                                                      \
  do {                                                                         \
    if (!(condition)) {                                                        \
      fprintf(stderr, "Assertion failed: %s", #condition);                     \
      fprintf(stderr, ", in line %d of file %s\n", __LINE__, __FILE__);        \
      exit(EXIT_FAILURE);                                                      \
    }                                                                          \
  } while (0)

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

// FAT table
#define LAST_BLOCK -1
#define UNUSED -2
// Files dir. index
#define NOT_ON_DIR -1
// Cursor
#define OPEN 0
#define CLOSED -1
// ?
#define NOT_FOUND -5

// TODO(all): Add error values to here
/**
 * @details This codes syntax is according with the next digit specification:
 *  * 1x: blah blah
 *  * 2x: blah blah
 *
 */
enum error_code {
  ERROR_NO_BLOCKS_AVAILABLE = -10,
  ERROR_EMPTY_FILENAME = -11,
  ERROR_NO_FILE_BY_THAT_NAME = -12,
  ERROR_FAILED_SEARCH = -13,
  ERROR_NO_DIRECTORY_INDEX = -14
};

// Type Definitions
typedef int64_t i64;
typedef uint64_t u64;
typedef uint8_t u8;
typedef int64_t BLOCK_INDEX;
typedef int64_t DIRECTORY_INDEX;
typedef int64_t FAT_INDEX;
typedef int64_t UNIT_INDEX;

#endif