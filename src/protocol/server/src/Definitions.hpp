// Copyright [2024] <Andres Quesada, Pablo Cascante, Diego Bolanos,
// Andres Serrano>"

#ifndef DEFINITIONS_HPP
#define DEFINITIONS_HPP

#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <arpa/inet.h>
#include <string>
#include <vector>
#include <sstream>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdio>
#include <csignal>
#include <unordered_map>
#include <chrono>    // For std::chrono::seconds
#include <thread>    // For std::this_thread::sleep_for
#include <sqlite3.h>

/**
 * @brief asserts that a certain condition is true, else it gives an error
 * message to stderr
 *
 */
#define ASSERT(condition)                                                      \
  do {                                                                         \
    if (!(condition)) {                                                        \
      fprintf(stderr, "Assertion failed: %s", #condition);                     \
      fprintf(stderr, ", in line %d of file %s\n", __LINE__, __FILE__);        \
      exit(EXIT_FAILURE);                                                      \
    }                                                                          \
  } while (0)

/**
 * @brief Byte size of a block on FileSystems unit
 *
 */
#define BLOCK_SIZE 256
/**
 * @brief Count of files the FileSystems unit can handle.
 *
 */
#define FILE_COUNT 100
/**
 * @brief FileSystems Unit can support a storage volume of a hundred files of
 * 256 bytes each
 *
 */
#define STORAGE_VOLUME FILE_COUNT *BLOCK_SIZE
/**
 * @brief FileSystems unit block count
 *
 */
#define BLOCK_COUNT (STORAGE_VOLUME / BLOCK_SIZE)
/**
 * @brief
 *
 */
#define DEBUG 1
/**
 * @brief General output format to convey errors to stderr
 *
 */
#define ERROR(message)                                                         \
  std::cerr << "\033[1;31m"                                                    \
            << "ERROR: " << message << "\033[0m" << std::endl;

#define LOG(message)                                                           \
  if (DEBUG)                                                                   \
    std::cerr << "\033[1;32m"                                                  \
              << "LOG: " << message << "\033[0m" << std::endl;

// FAT table

/**
 * @brief Value that FAT stores to indicate that a block is the last block a
 * file used.
 *
 */
#define LAST_BLOCK -1
/**
 * @brief Value that FAT stores to indicate that a block has not been used by
 * any file on the FileSystem
 *
 */
#define UNUSED -2
// Files dir. index

/**
 * @brief Value that the FileSystems directory uses to indicate that an entry is
 * empty.
 *
 */
#define NOT_ON_DIR -1

// Cursor
/**
 * @brief Initial value that indicates, via cursor, that a file on the
 * FileSystem is opened.
 *
 */
#define OPEN 0
/**
 * @brief Value that indicates, via cursor, that a file on the
 * FileSystem is closed
 *
 */
#define CLOSED -1
// ?
#define NOT_FOUND -5

// TODO(all): Add error values to here, document the digit syntax rules.
/**
 * @brief Error codes to some FileSystem methods.
 * 
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