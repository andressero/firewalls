// Copyright [2024] <Andres Quesada, Pablo Cascante, Diego Bolanos,
// Andres Serrano>"

#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

#include "Definitions.hpp"
#include "FileProperties.hpp"

#include <string>

// TODO(any): Doxygen
class FileSystem {
 private:
  u8 *unit;
  FileProperties *directory;
  BLOCK_INDEX *FAT;
 
  BLOCK_INDEX findFirstUnusedBlock();
  // Returns index of file in directory or -1 if failed
  DIRECTORY_INDEX search(const std::string name);
  // ! Check use
  i64 getFreeSpace();
  void replace(u64 block, std::string data);
  i64 findEOF(const std::string name);

 public:
  FileSystem();
  ~FileSystem();
  bool create(const std::string name, const std::string date, const std::string owner);
  bool efface(const std::string name);
  bool open(const std::string name);
  bool close(const std::string name);
  bool write(const std::string name, std::string &buffer, i64 bufferSize);
  bool append(const std::string name, std::string &buffer, i64 bufferSize);
  std::string read(const std::string name, size_t readSize);
  void print();
  void DumpToFile();
  void fillWithFile();
  void change2ReadMode(const std::string name);
  void chang2WriteMode(const std::string name);
  void changeCursor(const std::string name, UNIT_INDEX cursor);
};

#endif