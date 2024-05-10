// Copyright [2024] <Andres Quesada, Pablo Cascante, Diego Bolanos, Andres
// Serrano>"

#include "FileSystem.hpp"

// FileSystem
FileSystem::FileSystem()
    : unit(new u8[STORAGE_VOLUME]()),
      directory(new FileProperties[BLOCK_COUNT]()),
      FAT(new BLOCK_INDEX[BLOCK_COUNT]()) {}

FileSystem::~FileSystem() {
  delete[] this->unit;
  delete[] this->directory;
  delete[] this->FAT;
}

BLOCK_INDEX FileSystem::findFirstUnusedBlock() {
  for (size_t i = 0; i < BLOCK_COUNT; ++i) {
    if (this->FAT[i] == UNUSED) {
      return i;
    }
  }
  ERROR("Unable to find unused blocks. There's no Blocks available at Unit")
  return ERROR_NO_BLOCKS_AVAILABLE;
}

DIRECTORY_INDEX FileSystem::search(const std::string name) {
  if (name.empty()) {
    ERROR("Unable to find files. Filename is empty")
    return ERROR_EMPTY_FILENAME;
  }
  for (size_t i = 0; i < BLOCK_COUNT; ++i) {
    if (this->directory[i].getName() == name) {
      return i;
    }
  }
  LOG("No file with that name!")
  return ERROR_NO_FILE_BY_THAT_NAME;
}

bool FileSystem::create(const std::string name, const std::string date,
                        const std::string owner) {
  // Check if data for the file is valid
  if (name.empty() || date.empty() || owner.empty()) {
    ERROR("Unable to create. Incomplete entry file properties")
    return false;
  }
  // Is there a file with the same name already on Directory?
  if (search(name) != ERROR_NO_FILE_BY_THAT_NAME) {
    ERROR("Unable to create. That file name already exists")
    return false;
  }
  // Look for space on directory
  DIRECTORY_INDEX directoryIndex = ERROR_NO_DIRECTORY_INDEX;
  for (u64 i = 0; i < BLOCK_COUNT; ++i) {
    if (this->directory[i].getName().empty()) {
      directoryIndex = i;
      break;
    }
  }
  if (directoryIndex == ERROR_NO_DIRECTORY_INDEX) {
    ERROR("Unable to create file. There is no space left on directory")
    return false;
  }

  // Stablish where does the file can be stored using FAT.
  FAT_INDEX unusedBlockIndex = findFirstUnusedBlock();
  if (unusedBlockIndex == ERROR_NO_BLOCKS_AVAILABLE) {
    ERROR("Unable to create. There's no Blocks available at Unit")
    return false;
  }
  // Set all file info to directory.
  this->directory[directoryIndex].setDirectoryIndex(directoryIndex);
  this->directory[directoryIndex].setStartingBlock(unusedBlockIndex);
  this->directory[directoryIndex].setLastAccessedBlock(unusedBlockIndex);
  this->directory[directoryIndex].setName(name);
  this->directory[directoryIndex].setDate(date);
  this->directory[directoryIndex].setOwner(owner);
  // Reserve space on FAT
  this->FAT[unusedBlockIndex] = -1;

  return true;
}

bool FileSystem::efface(const std::string name) {
  DIRECTORY_INDEX file_index = this->search(name);
  if (file_index == ERROR_NO_FILE_BY_THAT_NAME) {
    ERROR("Unable to efface. There's no file by that name")
    return false;
  }
  ASSERT(this->directory[file_index].getName() == name);
  // The file can be effaced only if it's closed
  if (this->directory[file_index].getCursor() != CLOSED) {
    ERROR("Unable to efface. The file most be closed in order to efface it.")
    return false;
  }
  for (BLOCK_INDEX i =
           this->FAT[this->directory[file_index].getStartingBlock()];
       i != LAST_BLOCK; i = this->FAT[i]) {
    this->unit[i] = '\0';
    this->FAT[i] = UNUSED;
  }
  this->directory[file_index].setDirectoryIndex(NOT_ON_DIR);
  this->directory[file_index].setStartingBlock(UNUSED);
  this->directory[file_index].setLastAccessedBlock(UNUSED);
  this->directory[file_index].setName("");
  this->directory[file_index].setDate("");
  this->directory[file_index].setOwner("");
  
  return true;
}

bool FileSystem::open(const std::string name) {
  DIRECTORY_INDEX file_index = this->search(name);
  if (file_index == ERROR_NO_FILE_BY_THAT_NAME) {
    ERROR("Unable to open. File does not exist")
    return false;
  }
  this->directory[file_index].seek(OPEN);
  return true;
}

bool FileSystem::close(const std::string name) {
  DIRECTORY_INDEX file_index = this->search(name);
  if (file_index == ERROR_NO_FILE_BY_THAT_NAME) {
    ERROR("Unable to close. File is not valid!")
    return false;
  }
  this->directory[file_index].seek(CLOSED);
  return true;
}

// ! Is looking for an unused block and then for various unused blocks. That
// makes no sense, is double work on the same thing.
i64 FileSystem::getFreeSpace() {
  i64 freeSpace = 0;
  // BLOCK_INDEX currentBlock = this->findFirstUnusedBlock();
  // if (currentBlock == ERROR_NO_BLOCKS_AVAILABLE) {
  //   ERROR("Unable to find free space. There's no Blocks available at Unit")
  //   return ERROR_CODE;
  // }
  for (BLOCK_INDEX i = 0; i < BLOCK_COUNT; ++i) {
    if (this->FAT[i] == UNUSED) {
      freeSpace += BLOCK_SIZE;
    }
  }
  return freeSpace;
}

bool FileSystem::write(FileProperties &entry, std::string &buffer,
                       i64 bufferSize) {
  if (entry.getReadWriteMode() != true) {
    ERROR("You have to be in write mode to be able to write!")
    return false;
  }
  const DIRECTORY_INDEX index = this->search(entry);
  ASSERT(entry == this->directory[index]);
  ASSERT(entry.valid());
  ASSERT(entry.getCursor() >= OPEN);
  ASSERT(bufferSize > 0);
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
  BLOCK_INDEX currentBlock = this->directory[index].getStartingBlock();
  if (currentBlock == UNUSED) {
    currentBlock = findFirstUnusedBlock();
    entry.setStartingBlock(currentBlock);
    this->directory[index] = entry;
  }
  BLOCK_INDEX EoFBlock = currentBlock;
  for (BLOCK_INDEX i = 0; i < bufferSize; i += BLOCK_SIZE) {
    // this->unit.replace(currentBlock * BLOCK_SIZE, BLOCK_SIZE,
    // buffer.substr(i, BLOCK_SIZE));
    this->replace(currentBlock, buffer.substr(i, BLOCK_SIZE));
    FAT[currentBlock] = LAST_BLOCK;
    BLOCK_INDEX nextBlock = findFirstUnusedBlock();
    FAT[currentBlock] = nextBlock;
    EoFBlock = currentBlock;
    currentBlock = nextBlock;
  }
  entry.setLastAccessedBlock(EoFBlock);
  entry.seek(entry.getCursor() + bufferSize);
  this->directory[index] = entry;

  FAT[EoFBlock] = LAST_BLOCK;

  // this->print();

  return true;
}

void FileSystem::replace(u64 block, std::string data) {
  for (u64 i = 0; i < BLOCK_SIZE || i <= data.size(); ++i) {
    this->unit[block * BLOCK_SIZE + i] = data[i];
  }
}

bool FileSystem::append(FileProperties &entry, std::string &buffer,
                        i64 bufferSize) {
  if (entry.getReadWriteMode() != true) {
    ERROR("You have to be in write mode to be able to append!")
    return false;
  }
  (void)bufferSize;
  (void)entry;
  (void)buffer;

  ASSERT(entry == this->directory[entry.getDirectoryIndex()]);
  ASSERT(entry.valid());
  ASSERT(entry.getCursor() >= OPEN);
  /*
  entry.setLastAccesedBlock(EoFBlock);
  this->directory[index] = entry;
  */
  return true;
}

std::string FileSystem::read(FileProperties &entry, size_t readSize) {
  if (entry.getReadWriteMode() != false) {
    ERROR("You have to be in read mode to be able to read!")
    return "";
  }
  const DIRECTORY_INDEX index = entry.getDirectoryIndex();
  ASSERT(this->directory[index] == entry);
  ASSERT(entry.valid());
  ASSERT(entry.getCursor() >= OPEN);
  ASSERT(readSize > 0);

  std::stringstream buffer;

  if (index == ERROR_NO_DIRECTORY_INDEX) {
    return buffer.str();
  }

  BLOCK_INDEX currentBlock = this->directory[index].getStartingBlock();
  // BLOCK_INDEX currentBlock = this->directory[index].getLastAccesedBlock();

  size_t counter = 0;
  BLOCK_INDEX i = currentBlock;
  bool canContinue = true;
  for (; canContinue; i = this->FAT[i]) {
    canContinue = (i != UNUSED) && (i != LAST_BLOCK) && (counter <= readSize);
    for (u64 j = 0; j < BLOCK_SIZE && counter <= readSize; j++) {
      buffer << this->unit[i * BLOCK_SIZE + j];
      ++counter;
    }
    // #if 0
    //     if (counter < readSize && i != UNUSED && i != LAST_BLOCK) {
    //       // buffer << this->unit.substr(i * BLOCK_SIZE, BLOCK_SIZE);
    //       for (u64 j = 0; j < BLOCK_SIZE; ++j) {
    //         buffer << this->unit[i * BLOCK_SIZE + j];
    //       }
    //       counter += BLOCK_SIZE;
    //     } else {
    //       break;
    //     }
    // #endif
  }

  //! ESTO ESTA EN DUDA SI SE TIENE QUE DEVOLVER A 0 O SI AVANZAR A UN VALOR
  // entry.getCursor() +  // this->directory[index].getCursor() +
  // entry.seek(counter);
  this->directory[index].seek(counter);
  LOG("CURSOR " + std::to_string(this->directory[index].getCursor()))

  entry.setLastAccessedBlock(i);
  this->directory[index] = entry;
  return buffer.str();
}

void FileSystem::print() {
  // FAT Table
  i64 width = 5;
  std::cout << "FAT Table" << std::endl;
#if 0
  for (size_t i = 0; i < BLOCK_COUNT; ++i) {
    std::cout << "Block " << std::setw(width) << i;
  }
  std::cout << std::endl;
  for (size_t i = 0; i < BLOCK_COUNT; ++i) {
    std::cout << std::setw(width) << this->FAT[i];
  }
#endif

  for (size_t i = 0; i < BLOCK_COUNT; ++i) {
    std::cout << "Block " << std::setw(width) << i << std::setw(width)
              << this->FAT[i] << std::endl;
  }
  std::cout << std::endl
            << "=================================================" << std::endl;

  // Directory
  i64 directoryWidth = 25;
  std::cout << "Directory" << std::endl;
  std::cout << std::setw(directoryWidth) << "Index" << std::setw(directoryWidth)
            << "Name" << std::setw(directoryWidth) << "Date"
            << std::setw(directoryWidth) << "Owner" << std::setw(directoryWidth)
            << "StartingBlock" << std::setw(directoryWidth) << "Cursor"
            << std::endl;
  for (size_t i = 0; i < BLOCK_COUNT; ++i) {
    std::cout << std::setw(directoryWidth) << i << std::setw(directoryWidth)
              << this->directory[i].getName() << std::setw(directoryWidth)
              << this->directory[i].getDate() << std::setw(directoryWidth)
              << this->directory[i].getOwner() << std::setw(directoryWidth)
              << this->directory[i].getStartingBlock()
              << std::setw(directoryWidth) << this->directory[i].getCursor()
              << std::endl;
  }

  std::cout << std::endl
            << "=================================================" << std::endl;

  // Unit
  std::cout << std::hex << std::uppercase << std::setfill('0');

  for (size_t i = 0; i < BLOCK_SIZE; ++i) {
    for (size_t j = 0; j < BLOCK_COUNT; ++j) {
      std::cout << std::setw(2) << this->unit[i * BLOCK_COUNT + j] << " ";
    }
    std::cout << std::endl;
  }
}

void FileSystem::fillWithFile() {
  std::string filename = "data.txt";
  std::ifstream file(filename);

  if (file.is_open()) {
    std::string line;
    int i = 0;
    while (std::getline(file, line)) {
      FileProperties newUser("User" + std::to_string(i), "30-4-2023", "Server");
      this->create(newUser.getName(), newUser.getDate(), newUser.getOwner());
      this->open(newUser.getName());
      this->write(newUser.getName(), line, line.size());
      this->close(newUser.getName());
      ++i;
    }
    file.close();
  } else {
    ERROR("Unable to open file: " << filename);
  }
  return;
}

void FileSystem::DumpToFile() {
  std::string filename = "output.txt";
  std::ofstream file(filename);
  if (file.is_open()) {
    int i = 0;
    std::string data;
    while (!(this->directory[i].getName().empty())) {
      data = this->read(this->directory[i].getName(),
                        sizeof(this->directory[i])); // This size of is too big
      file << data << std::endl;
      ++i;
    }
    file.close();
  } else {
    ERROR("Unable to open file: " << filename);
  }
}

void FileSystem::change2ReadMode(const std::string name) {
  DIRECTORY_INDEX file_index = this->search(name);
  this->directory[file_index].changeMode(false);
}

void FileSystem::chang2WriteMode(const std::string name) {
  DIRECTORY_INDEX file_index = this->search(name);
  this->directory[file_index].changeMode(true);
}

void FileSystem::changeCursor(const std::string name, UNIT_INDEX cursor) {
  DIRECTORY_INDEX file_index = this->search(name);
  this->directory[file_index].seek(cursor);
}
