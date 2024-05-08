// Copyright [2024] <Andres Quesada, Pablo Cascante, Diego Bolanos,
// Andres Serrano>"

#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

#include "Definitions.hpp"
#include "FileProperties.hpp"

#include <string>

class FileSystem {
 private:
  u8 *unit;
  FileProperties *directory;
  BLOCK_INDEX *FAT;
 
  BLOCK_INDEX findFirstUnusedBlock();
  // Returns index of file in directory or -1 if failed
  DIRECTORY_INDEX search(FileProperties &entry);
  // ! Check use
  i64 getFreeSpace();
  void replace(u64 block, std::string data);

 public:
  FileSystem();
  ~FileSystem();
  // TODO(Quesada): 
  bool create(std::string name, std::string date, std::string owner);
  bool erase(std::string name);
  // TODO(777XXXDieguitoXXX777)
  bool open(std::string name);
  bool close(std::string name);
  // TODO(Serrano)
  bool write(std::string name, std::string &buffer, i64 bufferSize);
  bool append(std::string name, std::string &buffer, i64 bufferSize);
  std::string read(std::string name, size_t readSize);
  // No editar
  void print();
  void DumpToFile();
  void fillWithFile();
  // TODO(Pablo)
  void change2ReadMode(std::string name);
  void chang2WriteMode(std::string name);
  void changeCursor(std::string name, UNIT_INDEX cursor);
};

#endif