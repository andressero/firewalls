// Copyright [2024] <Andres Quesada, Pablo Cascante, Diego Bolanos, Andres
// Serrano>"

#include "FileSystem.hpp"

// FileSystem
FileSystem::FileSystem()
    : unit(new u8[STORAGE_VOLUME]()),
      directory(new FileProperties[BLOCK_COUNT]()),
      FAT(new BLOCK_INDEX[BLOCK_COUNT]()) {
  for (size_t i = 0; i < BLOCK_COUNT; ++i) {
    this->FAT[i] = UNUSED;
  }
}

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
    ERROR("Unable to find files. Filename cannot be empty.")
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
    ERROR("Unable to create. Incomplete entry file properties.")
    return false;
  }
  // Is there a file with the same name already on Directory?
  if (search(name) != ERROR_NO_FILE_BY_THAT_NAME) {
    ERROR("Unable to create. That file name already exists.")
    return false;
  }
  // Look for space on directory
  DIRECTORY_INDEX directoryIndex = ERROR_NO_DIRECTORY_INDEX;
  for (u64 i = 0; i < BLOCK_COUNT; ++i) {
    if (this->directory[i].getDirectoryIndex() == NOT_ON_DIR) {
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
  // Set all file info to directory entry.
  this->directory[directoryIndex].setDirectoryIndex(directoryIndex);
  this->directory[directoryIndex].setStartingBlock(unusedBlockIndex);
  this->directory[directoryIndex].setLastBlock(unusedBlockIndex);
  this->directory[directoryIndex].setName(name);
  this->directory[directoryIndex].setDate(date);
  this->directory[directoryIndex].setOwner(owner);
  // Reserve space on FAT
  this->FAT[unusedBlockIndex] = -1;

  return true;
}

bool FileSystem::efface(const std::string name) {
  DIRECTORY_INDEX file_index = this->search(name);
  if (file_index == ERROR_EMPTY_FILENAME) {
    ERROR("Unable to efface. The file name cannot be empty.");
  } else if (file_index == ERROR_NO_FILE_BY_THAT_NAME) {
    ERROR("Unable to efface. There's no file by that name")
    return false;
  }
  ASSERT(this->directory[file_index].getName() == name);
  // The file can be effaced only if it's closed
  if (this->directory[file_index].getCursor() != CLOSED) {
    ERROR("Unable to efface. The file MUST be closed in order to efface it.")
    return false;
  }
  for (BLOCK_INDEX i =
           this->FAT[this->directory[file_index].getStartingBlock()];
       i != LAST_BLOCK;) {
    for (UNIT_INDEX j = 0; j < BLOCK_SIZE; ++j) {
      this->unit[i * BLOCK_SIZE + j] = '\0';
    }
    i = this->FAT[i];
    this->FAT[i] = UNUSED;
  }
  this->directory[file_index].setDirectoryIndex(NOT_ON_DIR);
  this->directory[file_index].setStartingBlock(UNUSED);
  this->directory[file_index].setLastBlock(UNUSED);
  this->directory[file_index].setName("");
  this->directory[file_index].setDate("");
  this->directory[file_index].setOwner("");

  return true;
}

bool FileSystem::open(const std::string name) {
  DIRECTORY_INDEX file_index = this->search(name);
  if (file_index == ERROR_EMPTY_FILENAME) {
    ERROR("Unable to open. The filename cannot be empty.");
    return false;
  } else if (file_index == ERROR_NO_FILE_BY_THAT_NAME) {
    ERROR("Unable to open. There's no file by that name.")
    return false;
  }
  this->directory[file_index].seek(OPEN);
  return true;
}

bool FileSystem::close(const std::string name) {
  DIRECTORY_INDEX file_index = this->search(name);
  if (file_index == ERROR_EMPTY_FILENAME) {
    ERROR("Unable to close. The filename cannot be empty.");
    return false;
  } else if (file_index == ERROR_NO_FILE_BY_THAT_NAME) {
    ERROR("Unable to close. There's no file by that name.")
    return false;
  }
  this->directory[file_index].seek(CLOSED);
  return true;
}

i64 FileSystem::getFreeSpace() {
  i64 freeSpace = 0;
  for (BLOCK_INDEX i = 0; i < BLOCK_COUNT; ++i) {
    if (this->FAT[i] == UNUSED) {
      freeSpace += BLOCK_SIZE;
    }
  }
  return freeSpace;
}

bool FileSystem::canWrite(const std::string &name, const i64 &bufferSize) {
  if (this->getFreeSpace() < bufferSize) {
    ERROR("Unable to write. There is not enough space");
    return false;
  }

  DIRECTORY_INDEX fileIndex = this->search(name);

  if (fileIndex == ERROR_EMPTY_FILENAME) {
    ERROR("Unable to write. Filename cannot be empty.");
    return false;
  }
  if (fileIndex == ERROR_NO_FILE_BY_THAT_NAME) {
    ERROR("Unable to write. File does not exist");
    return false;
  }
  if (this->directory[fileIndex].getCursor() < OPEN) {
    ERROR("Unable to write. File is not open");
    return false;
  }
  if (this->directory[fileIndex].getReadWriteMode() == false) {
    ERROR("Unable to write. File is not in write mode");
    return false;
  }
  return true;
}

bool FileSystem::write(const std::string name, std::string &buffer,
                       i64 bufferSize) {
  if (!canWrite(name, bufferSize)) {
    return false;
  }

  // Get file
  FileProperties &file = this->directory[this->search(name)];

  UNIT_INDEX cursor = file.getCursor();
  BLOCK_INDEX currentBlock = file.getStartingBlock();

  // Go to the block that cursor is referring to
  i64 blocksToJump = cursor / BLOCK_SIZE;
  for (; blocksToJump > 1; --blocksToJump) {
    currentBlock = this->FAT[currentBlock];
  }

  UNIT_INDEX blockOffset = (cursor + 1) % BLOCK_SIZE;
  i64 lastBlockIndex = BLOCK_SIZE - 1;
  bool lastBlockChanged = false;

  // Write
  for (int i = 0; i < bufferSize; ++i) {
    if (blockOffset == BLOCK_SIZE) {
      blockOffset = 0;
      if (this->FAT[currentBlock] == LAST_BLOCK) {
        // Find a new block and update FAT table
        BLOCK_INDEX previousBlock = currentBlock;
        currentBlock = findFirstUnusedBlock();
        this->FAT[previousBlock] = currentBlock;
        lastBlockChanged = true;
      } else {
        // Go to next block (cursor is not at the end of the file)
        currentBlock = FAT[currentBlock];
      }
    }
    // Write a character
    this->unit[currentBlock * BLOCK_SIZE + blockOffset] = buffer[i];
    ++blockOffset;
    ++cursor;
  }

  // Update file's cursor after writing
  file.seek(cursor);

  if (lastBlockChanged) {
    file.setLastBlock(currentBlock);
  }
  return true;
}

i64 FileSystem::findEOF(const std::string name) {
  DIRECTORY_INDEX file_index = this->search(name);
  if (file_index == ERROR_EMPTY_FILENAME) {
    ERROR("Unable to find EOF. FIlename cannot be empty.");
    return -1;
  } else if (file_index == ERROR_NO_FILE_BY_THAT_NAME) {
    ERROR("Unable to find EOF. There's no file by that name");
    return -1;
  }
  BLOCK_INDEX lastBlock = this->directory[file_index].getLastBlock();
  i64 counter = 0;
  for (u64 j = 0; j < BLOCK_SIZE; j++) {
    if (this->unit[lastBlock * BLOCK_SIZE + j] == 0) {
      return counter;
    }
    ++counter;
  }
  return -1;
}

bool FileSystem::append(const std::string name, std::string &buffer,
                        i64 bufferSize) {
  UNIT_INDEX eofIndex = this->findEOF(name);
  if (eofIndex == -1) {
    ERROR("Unable to append. The EOF not found.")
    return false;
  }
  // Set the cursor position at the end of the file and write from there
  this->changeCursor(name, eofIndex);
  return this->write(name, buffer, bufferSize);
}

std::string FileSystem::read(const std::string name, size_t readSize) {
  ASSERT(readSize > 0);
  DIRECTORY_INDEX file_index = this->search(name);
  if (file_index == ERROR_EMPTY_FILENAME) {
    ERROR("Unable to read. File name cannot be empty.")
  } else if (file_index == ERROR_NO_FILE_BY_THAT_NAME) {
    ERROR("Unable to read. There's no file by that name");
    return "";
  }

  if (this->directory[file_index].getCursor() == CLOSED) {
    ERROR("Unable to read. The File must be opened in order to read.");
    return "";
  }
  if (this->directory[file_index].getReadWriteMode() != false) {
    ERROR("Unable to read. The File MUST be on read mode in order to read it!")
    return "";
  }
  std::stringstream buffer;

  BLOCK_INDEX currentBlock = this->directory[file_index].getStartingBlock();

  size_t counter = 0;
  BLOCK_INDEX i = currentBlock;
  bool canContinue = true;
  for (; canContinue; i = this->FAT[i]) {
    canContinue = (i < BLOCK_COUNT) && (i != LAST_BLOCK) && (counter <= readSize);
    if(!canContinue){break;}
    for (u64 j = 0; j < BLOCK_SIZE && counter <= readSize; j++) {
      buffer << this->unit[i * BLOCK_SIZE + j];
      ++counter;
    }
  }

  this->directory[file_index].seek(counter);
  LOG("CURSOR " + std::to_string(this->directory[file_index].getCursor()))

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
