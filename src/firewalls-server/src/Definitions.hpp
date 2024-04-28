#ifndef DEFINITIONS_HPP
#define DEFINITIONS_HPP

#include <string>
#include <stdlib.h>
#include <sstream>
#include <array>
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <cstdlib>
#include <cassert>
#include <iomanip>
   
#define BLOCK_SIZE 256
#define FILE_COUNT 100  
#define STORAGE_VOLUME FILE_COUNT * BLOCK_SIZE // Hundred files of 256 bytes each
#define BLOCK_COUNT (STORAGE_VOLUME / BLOCK_SIZE)

#define ERROR(message) std::cerr << "\033[1;31m" << "ERROR: " << message << "\033[0m" << std::endl;

#define LOG(message) std::cerr << "\033[1;33m" << "LOG: " << message << "\033[0m" << std::endl;

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
typedef int64_t BLOCK_POINTER;
typedef int64_t DIRECTORY_POINTER;
typedef int64_t FAT_POINTER;
typedef int64_t UNIT_POINTER;

#endif