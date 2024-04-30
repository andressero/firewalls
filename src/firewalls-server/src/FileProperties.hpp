// Copyright [2024] <Andres Quesada, Pablo Cascante, Diego Bolanos, Andres
// Serrano>"

#ifndef FILEPROPERTIES_HPP
#define FILEPROPERTIES_HPP

#include "Definitions.hpp"

class FileProperties {
 private:
  string name;
  string date;
  string owner;
  BLOCK_POINTER startingBlock;
  UNIT_POINTER cursor;
  BLOCK_POINTER lastAccesedBlock;
  DIRECTORY_POINTER directoryIndex;

 public:
  FileProperties();
  FileProperties(string name, string date, string owner);
  explicit FileProperties(const FileProperties &properties);
  __attribute__((const)) bool valid() const;
  inline void operator=(const FileProperties &properties) {
    this->name = properties.name;
    this->date = properties.date;
    this->owner = properties.owner;
    this->startingBlock = properties.startingBlock;
    this->cursor = properties.cursor;
    this->lastAccesedBlock = properties.lastAccesedBlock;
    this->directoryIndex = properties.directoryIndex;
  }

  bool operator==(const FileProperties &properties) const {
    const bool equalName = this->name == properties.name;
    const bool equalDate = this->date == properties.date;
    const bool equalOwner = this->owner == properties.owner;
    const bool equalStartingBlock =
        this->startingBlock == properties.startingBlock;
    const bool equalCursor = this->cursor == properties.cursor;
    const bool equalLastAccesedBlock =
        this->lastAccesedBlock == properties.lastAccesedBlock;
    const bool equalDirectoryIndex =
        this->directoryIndex == properties.directoryIndex;
    return equalName && equalDate && equalOwner && equalStartingBlock &&
           equalCursor && equalLastAccesedBlock && equalDirectoryIndex;
  }

  bool operator!=(const FileProperties &properties) const {
    return !(*this == properties);
  }

  // Gets
  inline string getName() const { return this->name; }

  inline string getDate() const { return this->date; }

  inline string getOwner() const { return this->owner; }

  inline BLOCK_POINTER getStartingBlock() const { return this->startingBlock; }

  inline UNIT_POINTER getCursor() const { return this->cursor; }

  inline DIRECTORY_POINTER getDirectoryIndex() const {
    return this->directoryIndex;
  }

  inline BLOCK_POINTER getLastAccesedBlock() const {
    return this->lastAccesedBlock;
  }

  // Sets
  inline void setCursor(UNIT_POINTER cursor) { this->cursor = cursor; }

  inline void setName(string name) { this->name = name; }

  inline void setStartingBlock(BLOCK_POINTER startingBlock) {
    this->startingBlock = startingBlock;
  }

  inline void setLastAccesedBlock(BLOCK_POINTER lastAccesedBlock) {
    this->lastAccesedBlock = lastAccesedBlock;
  }

  inline void setDirectoryIndex(DIRECTORY_POINTER directoryIndex) {
    this->directoryIndex = directoryIndex;
  }
};

#endif
