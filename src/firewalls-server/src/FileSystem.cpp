// Copyright [2024] <Andres Quesada, Pablo Cascante, Diego Bolanos, Andres
// Serrano>"

#include "FileSystem.hpp"

// FileSystem
FileSystem::FileSystem() {
  // ! It is still empty but it's reserved for STORATE_VOLUME chars
  for (i64 i = 0; i < BLOCK_COUNT; ++i) {
    this->FAT[i] = UNUSED;
  }
}

FileSystem::~FileSystem() {  // Free directory members memory
}

BLOCK_POINTER FileSystem::findFirstUnusedBlock() {
  for (size_t i = 0; i < BLOCK_COUNT; ++i) {
    if (this->FAT[i] == UNUSED) {
      return i;
    }
  }
  ERROR("Unable to find unused blocks. There's no Blocks available at Unit")
  return NONE;
}

DIRECTORY_POINTER FileSystem::search(FileProperties &entry) {
  if (entry.getName().empty()) {
    ERROR("Unable to find files. Filename is empty")
    return ERROR_CODE;
  }
  for (size_t i = 0; i < BLOCK_COUNT; ++i) {
    if (this->directory.at(i).getName() == entry.getName()) {
      return i;
    }
  }
  LOG("No file with that name!")
  return NONE;
}

bool FileSystem::create(FileProperties &entry) {
  // Check if fileproperties is valid
  if (!entry.valid()) {
    ERROR("Unable to create. Incomplete entry file properties")
    return false;
  }
  // Is there a file with the same name already on Directory?
  if (search(entry) != NONE) {
    ERROR("Unable to create. That file name already exists")
    return false;
  }
  // Look for space on directory
  int directoryIndex = NONE;
  for (u64 i = 0; i < BLOCK_COUNT; ++i) {
    if (this->directory[i].getName().empty()) {
      directoryIndex = i;
      break;
    }
  }
  if (directoryIndex == NONE) {
    ERROR("Unable to create file. There is no space left on directory")
    return false;
  }
  // ? Always starts at zero when it's created
  entry.setDirectoryIndex(directoryIndex);
  this->directory[directoryIndex] = entry;
  // this->directory[directoryIndex].setCursor(0);

  // ? Should it reserve the space for when it has content or now?
  // Stablish where does the file can be stored using FAT.
  FAT_POINTER unusedBlockIndex = findFirstUnusedBlock();
  if (unusedBlockIndex == NONE) {
    ERROR("Unable to create. There's no Blocks available at Unit")
    return false;
  }
  assert(unusedBlockIndex >= 0);
  this->directory[directoryIndex].setStartingBlock(unusedBlockIndex);
  this->directory[directoryIndex] = entry;
  this->FAT[unusedBlockIndex] = -1;  // Spaced reserved on FAT
  assert(entry.valid());
  return true;
}

// ! It doesn't work, cause the for loop it's finding a -2(UNUSED) and that's
// not possible if the file is created (exists) on unit then the last block
// used to store file's data is always -1 (which means used but no next).
bool FileSystem::erase(FileProperties &entry) {
  // const DIRECTORY_POINTER index = this->search(entry);
  const DIRECTORY_POINTER index = entry.getDirectoryIndex();
  assert(entry == this->directory[index]);
  if (index == NONE) {
    ERROR("Could not find file!")
    return false;
  }
  BLOCK_POINTER currentBlock = this->directory[index].getStartingBlock();

  for (BLOCK_POINTER i = this->FAT[currentBlock]; i != UNUSED;
       i = this->FAT[i]) {
    this->FAT[currentBlock] = -1;  // No next block
    currentBlock = i;
  }
  this->directory[index].setStartingBlock(-1);

  return true;
}

bool FileSystem::open(FileProperties &entry) {
  // assert(entry.valid()); // make release removes this
  if (!entry.valid()) {
    ERROR("Unable to open. File does not exist")
    return false;
  }
  DIRECTORY_POINTER index = entry.getDirectoryIndex();
  assert(this->directory[index] == entry);
  // TODO(any) make file properties remember directory pointer
  // to not having to call search every time
  // this->directory[index].setCursor(OPEN);
  entry.setCursor(OPEN);
  this->directory[index] = entry;
  return true;
}

bool FileSystem::close(FileProperties &entry) {
  DIRECTORY_POINTER index = entry.getDirectoryIndex();
  assert(entry == this->directory[index]);
  //? assert(entry.valid());
  if (!entry.valid()) {
    ERROR("Unable to close. File is not valid!")
    return false;
  }
  entry.setCursor(CLOSED);
  this->directory[index] = entry;
  return true;
}

// ! Is looking for an unused block and then for various unused blocks. That
// makes no sense, is double work on the same thing.
i64 FileSystem::getFreeSpace() {
  i64 freeSpace = 0;
  // BLOCK_POINTER currentBlock = this->findFirstUnusedBlock();
  // if (currentBlock == NONE) {
  //   ERROR("Unable to find free space. There's no Blocks available at Unit")
  //   return ERROR_CODE;
  // }
  for (BLOCK_POINTER i = 0; i < BLOCK_COUNT; ++i) {
    if (this->FAT[i] == UNUSED) {
      freeSpace += BLOCK_SIZE;
    }
  }
  return freeSpace;
}

bool FileSystem::write(FileProperties &entry, string &buffer,
                       i64 bufferSize) {
  const DIRECTORY_POINTER index = this->search(entry);
  assert(entry == this->directory[index]);
  assert(entry.valid());
  assert(entry.getCursor() >= OPEN);
  assert(bufferSize > 0);
  if (index < 0) {
    ERROR("Unable to write. File does not exist");
    return false;
  }
  // 1. CHECK IF THERE IS ENOUGH SPACE
  // ! It didn't take the consideration about what was used before by the same
  // file. So if we already reserved some space when it was created or if we
  // wrote something before this won't take into consideration the space we
  // already have form those previous calls. Remember that write can overwrite a
  // file. If the file contains "Hello World!" and we write "Bye" then it must
  // contain: "Byelo World!" not just "Bye".
  if (this->getFreeSpace() < bufferSize) {
    ERROR("Unable to write file. There is no enough space left on unit to "
          "store this buffer");
  }

  /*
      startingBlock = 5 nextBlock = 5 LastBlock = 4
      1 | 3  | Hola
      2 | 9  |
      3 | 4  | Mundo
      4 | -1 | Hermoso
      5 | -2 |lastblock
    */
  // ! There's no way for this to happen cause "create" comes before write, and
  // it already reserved an starting block. If it wansn't able to, then it
  // would've returned false.
  BLOCK_POINTER currentBlock = this->directory[index].getStartingBlock();
  if (currentBlock == UNUSED) {
    currentBlock = findFirstUnusedBlock();
    entry.setStartingBlock(currentBlock);
    this->directory[index] = entry;
  }
  BLOCK_POINTER EoFBlock = currentBlock;
  for (BLOCK_POINTER i = 0; i < bufferSize; i += BLOCK_SIZE) {
    // this->unit.replace(currentBlock * BLOCK_SIZE, BLOCK_SIZE, buffer.substr(i, BLOCK_SIZE));
    this->replace(currentBlock, buffer.substr(i, BLOCK_SIZE));
    FAT[currentBlock] = LAST_BLOCK;
    BLOCK_POINTER nextBlock = findFirstUnusedBlock();
    FAT[currentBlock] = nextBlock;
    EoFBlock = currentBlock;
    currentBlock = nextBlock;
  }
  entry.setLastAccesedBlock(EoFBlock);
  entry.setCursor(entry.getCursor() + bufferSize);
  this->directory[index] = entry;

  FAT[EoFBlock] = LAST_BLOCK;

  // this->print();

  return true;
}

void FileSystem::replace(u64 block, string data) {
  for (u64 i = 0; i < BLOCK_SIZE || i <= data.size(); ++i) {
    this->unit[block * BLOCK_SIZE + i] = data[i];
  }
}

bool FileSystem::append(FileProperties &entry, string &buffer,
                      i64 bufferSize) {
  (void)bufferSize;
  (void)entry;
  (void)buffer;

  assert(entry == this->directory[entry.getDirectoryIndex()]);
  assert(entry.valid());
  assert(entry.getCursor() >= OPEN);
  /*
  entry.setLastAccesedBlock(EoFBlock);
  this->directory[index] = entry;
  */
  return true;
}

string FileSystem::read(FileProperties &entry, size_t readSize) {
  const DIRECTORY_POINTER index = entry.getDirectoryIndex();
  assert(this->directory[index] == entry);
  assert(entry.valid());
  assert(entry.getCursor() >= OPEN);
  assert(readSize > 0);

  std::stringstream buffer;

  if (index == NONE) {
    return buffer.str();
  }

  BLOCK_POINTER currentBlock = this->directory[index].getStartingBlock();
  // BLOCK_POINTER currentBlock = this->directory[index].getLastAccesedBlock();

  size_t counter = 0;
  BLOCK_POINTER i = currentBlock;
  bool canContinue = true;
  for (;canContinue; i = this->FAT[i]) {
    canContinue = (i != UNUSED) && (i != LAST_BLOCK) && (counter <= readSize);
    for (u64 j = 0; j < BLOCK_SIZE && counter <=readSize; j++) {
      buffer << this->unit[i * BLOCK_SIZE + j];
      ++counter;
    }
    #if 0
    if (counter < readSize && i != UNUSED && i != LAST_BLOCK) {
      // buffer << this->unit.substr(i * BLOCK_SIZE, BLOCK_SIZE);
      for (u64 j = 0; j < BLOCK_SIZE; ++j) {
        buffer << this->unit[i * BLOCK_SIZE + j];
      }
      counter += BLOCK_SIZE;
    } else {
      break;
    }
    #endif
  }

 //! ESTO ESTA EN DUDA SI SE TIENE QUE DEVOLVER A 0 O SI AVANZAR A UN VALOR
 // entry.getCursor() +  // this->directory[index].getCursor() +
  entry.setCursor(counter);
  this->directory[index].setCursor(counter);
  LOG("CURSOR " + std::to_string(this->directory[index].getCursor()))

  entry.setLastAccesedBlock(i);
  this->directory[index] = entry;
  return buffer.str();
}

void FileSystem::print() {

  // FAT Table
  i64 width = 5;
  std::cout << "FAT Table" << std::endl;
  for (size_t i = 0; i < BLOCK_COUNT; ++i) {
    std::cout << "Block " << std::setw(width) << i;
  }
  std::cout << std::endl;
  for (size_t i = 0; i < BLOCK_COUNT; ++i) {
    std::cout << std::setw(width) << this->FAT[i];
  }
  std::cout << std::endl
            << "=================================================" << std::endl;

  // Directory
  i64 directoryWidth = 25;
  std::cout << "Directory" << std::endl;
  std::cout << std::setw(directoryWidth) << "Index"
            << "Name"
            << "Date"
            << "Owner"
            << "StartingBlock"
            << "Cursor" << std::endl;
  for (size_t i = 0; i < BLOCK_COUNT; ++i) {
    std::cout << std::setw(directoryWidth) << i << this->directory[i].getName()
              << this->directory[i].getDate() << this->directory[i].getOwner()
              << this->directory[i].getStartingBlock()
              << this->directory[i].getCursor() << std::endl;
  }

  std::cout << std::endl
            << "=================================================" << std::endl;

  // Unit
  std::cout << std::hex << std::uppercase << std::setfill('0');

  for (size_t i = 0; i < BLOCK_SIZE; ++i) {
    for (size_t j = 0; j < BLOCK_COUNT; ++j) {
      std::cout << std::setw(2)
                << static_cast<int>(this->unit[i * BLOCK_COUNT + j]) << " ";
    }
    std::cout << std::endl;
  }
}
