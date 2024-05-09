// Copyright [2024] <Andres Quesada, Pablo Cascante, Diego Bolanos, Andres
// Serrano>"

#include "FileProperties.hpp"

FileProperties::FileProperties()
    : startingBlock(UNUSED), lastAccessedBlock(UNUSED), cursor(CLOSED),
      directoryIndex(NOT_ON_DIR) {}

FileProperties::FileProperties(const std::string name, const std::string date,
                               const std::string owner)
    : name(name), date(date), owner(owner), startingBlock(UNUSED),
      lastAccessedBlock(UNUSED), cursor(CLOSED), directoryIndex(NOT_ON_DIR) {}

inline FileProperties::FileProperties(const FileProperties &properties) {
  this->name = properties.name;
  this->date = properties.date;
  this->owner = properties.owner;
  this->startingBlock = properties.startingBlock;
  this->cursor = properties.cursor;
  this->lastAccessedBlock = properties.lastAccessedBlock;
  this->directoryIndex = properties.directoryIndex;
}

// ? Is it necessary
__attribute__((const)) bool FileProperties::valid() const {
  const bool nameNotEmpty = !this->name.empty();
  const bool dateNotEmpty = !this->date.empty();
  const bool ownerNotEmpty = !this->owner.empty();
  const bool startingBlockLessThanBlockCount =
      this->startingBlock < BLOCK_COUNT;
  const bool cursorLessThanStorageVolume = this->cursor < STORAGE_VOLUME;
  return nameNotEmpty && dateNotEmpty && ownerNotEmpty &&
         startingBlockLessThanBlockCount && cursorLessThanStorageVolume;
}
