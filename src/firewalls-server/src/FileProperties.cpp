// Copyright [2024] <Andres Quesada, Pablo Cascante, Diego Bolanos, Andres
// Serrano>"

#include "FileProperties.hpp"

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

FileProperties::FileProperties() : startingBlock(UNUSED), cursor(CLOSED) {}

FileProperties::FileProperties(std::string name, std::string date, std::string owner)
    : name(name), date(date), owner(owner), startingBlock(UNUSED),
      cursor(CLOSED) {}

inline FileProperties::FileProperties(const FileProperties &properties) {
  this->name = properties.name;
  this->date = properties.date;
  this->owner = properties.owner;
  this->startingBlock = properties.startingBlock;
  this->cursor = properties.cursor;
  this->lastAccesedBlock = properties.lastAccesedBlock;
  this->directoryIndex = properties.directoryIndex;
}