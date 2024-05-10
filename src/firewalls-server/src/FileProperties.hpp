// Copyright [2024] <Andres Quesada, Pablo Cascante, Diego Bolanos, Andres
// Serrano>"

#ifndef FILEPROPERTIES_HPP
#define FILEPROPERTIES_HPP

#include "Definitions.hpp"

// TODO(any): Doxygen
class FileProperties {
private:
  std::string name;
  std::string date;
  std::string owner;
  BLOCK_INDEX startingBlock;
  BLOCK_INDEX lastBlock;
  UNIT_INDEX cursor;
  DIRECTORY_INDEX directoryIndex;
  // Read is false, Write is true
  bool ReadWriteMode = false;

public:
  FileProperties();
  FileProperties(const std::string name, const std::string date,
                 const std::string owner);
  // ? Is it necessary?
  explicit FileProperties(const FileProperties &properties);
  // ? Is it useful now?
  __attribute__((const)) bool valid() const;

  inline void operator=(const FileProperties &properties) {
    this->name = properties.name;
    this->date = properties.date;
    this->owner = properties.owner;
    this->startingBlock = properties.startingBlock;
    this->cursor = properties.cursor;
    this->lastBlock = properties.lastBlock;
    this->directoryIndex = properties.directoryIndex;
  }

  bool operator==(const FileProperties &properties) const {
    const bool equalName = this->name == properties.name;
    const bool equalDate = this->date == properties.date;
    const bool equalOwner = this->owner == properties.owner;
    const bool equalStartingBlock =
        this->startingBlock == properties.startingBlock;
    const bool equalCursor = this->cursor == properties.cursor;
    const bool equallastBlock =
        this->lastBlock == properties.lastBlock;
    const bool equalDirectoryIndex =
        this->directoryIndex == properties.directoryIndex;
    return equalName && equalDate && equalOwner && equalStartingBlock &&
           equalCursor && equallastBlock && equalDirectoryIndex;
  }

  bool operator!=(const FileProperties &properties) const {
    return !(*this == properties);
  }

  // Gets
  inline const std::string &getName() const { return this->name; }

  inline const std::string &getDate() const { return this->date; }

  inline const std::string &getOwner() const { return this->owner; }

  inline const BLOCK_INDEX &getStartingBlock() const {
    return this->startingBlock;
  }

  inline const UNIT_INDEX &getCursor() const { return this->cursor; }

  inline const DIRECTORY_INDEX &getDirectoryIndex() const {
    return this->directoryIndex;
  }

  inline const BLOCK_INDEX &getLastBlock() const {
    return this->lastBlock;
  }

  inline const bool &getReadWriteMode() const { return this->ReadWriteMode; }

  // Sets
  inline void seek(UNIT_INDEX cursor) { this->cursor = cursor; }

  inline void setName(const std::string &name) { this->name = name; }

  inline void setDate(const std::string &date) { this->date = date; }
  
  inline void setOwner(const std::string &owner) { this->owner = owner; }

  inline void setStartingBlock(BLOCK_INDEX startingBlock) {
    this->startingBlock = startingBlock;
  }

  inline void setLastBlock(BLOCK_INDEX lastBlock) {
    this->lastBlock = lastBlock;
  }

  inline void setDirectoryIndex(DIRECTORY_INDEX directoryIndex) {
    this->directoryIndex = directoryIndex;
  }

  inline void changeMode(bool mode) {
    if (mode) {
      this->ReadWriteMode = true; // Write
    } else {
      this->ReadWriteMode = false; // Read
    }
    this->seek(0);
  }
};

#endif