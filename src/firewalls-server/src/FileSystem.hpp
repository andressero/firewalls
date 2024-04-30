// Copyright [2024] <Andres Quesada, Pablo Cascante, Diego Bolanos,
// Andres Serrano>"

#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

#include "Definitions.hpp"
#include "FileProperties.hpp"

class FileSystem {
 private:
  std::array<u8, STORAGE_VOLUME> unit;
  // ? Change it to &?
  std::array<FileProperties, BLOCK_COUNT> directory;
  std::array<BLOCK_POINTER, BLOCK_COUNT> FAT;

  BLOCK_POINTER findFirstUnusedBlock();
  // Returns index of file in directory or -1 if failed
  DIRECTORY_POINTER search(FileProperties &entry);

 public:
  FileSystem();
  ~FileSystem();

  bool create(FileProperties &entry);
  bool erase(FileProperties &entry);
  bool open(FileProperties &entry);
  bool close(FileProperties &entry);
  bool write(FileProperties &entry, string &buffer, i64 bufferSize);
  bool append(FileProperties &entry, string &buffer, i64 bufferSize);
  string read(FileProperties &entry, size_t readSize);
  void print();
  i64 getFreeSpace();
  void replace(u64 block, string data);
};

#endif
