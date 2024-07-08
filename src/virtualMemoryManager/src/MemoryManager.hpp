// Copyright [2024] <Pablo Cascante, Andres Quesada, Andres Serrano>

#include <iostream>
#include <fstream>
#include <vector>

class MemoryManager {
 private:
  const size_t OFFSET_BITS;
  const size_t PAGE_BITS;
  const size_t FRAME_BITS;
  const size_t PAGE_SIZE;
  const size_t FRAME_SIZE;
  const size_t NUMBER_OF_PAGES;
  const size_t NUMBER_OF_FRAMES;
  const size_t MEMORY_SIZE;

  size_t next_available_frame = 0;
  std::vector<size_t> memory;
  std::vector<size_t> page_table;
  std::vector<bool> page_valid;
  std::ifstream backing_store;

 public:
  MemoryManager(size_t offsetBits, size_t pageBits, size_t frameBits, const std::string &backingStorageFile)
    : OFFSET_BITS(offsetBits), PAGE_BITS(pageBits), FRAME_BITS(frameBits),
      PAGE_SIZE(1 << OFFSET_BITS), FRAME_SIZE(PAGE_SIZE),
      NUMBER_OF_PAGES(1 << PAGE_BITS), NUMBER_OF_FRAMES(1 << FRAME_BITS),
      MEMORY_SIZE(FRAME_SIZE * NUMBER_OF_FRAMES), memory(MEMORY_SIZE, 0),
      page_table(NUMBER_OF_PAGES, 0), page_valid(NUMBER_OF_PAGES, false),
      backing_store(backingStorageFile, std::ios::in | std::ios::binary)
  {
    if (!backing_store) {
      throw std::runtime_error("Failed to open backing store file.");
    }
  }

  size_t getPageNumber(size_t address) const {
    return (address & ((1 << (OFFSET_BITS + PAGE_BITS)) - (1 << OFFSET_BITS))) >> OFFSET_BITS;
  }

  size_t getOffset(size_t address) const {
    return address & ((1 << OFFSET_BITS) - 1);
  }

  size_t getPhysicalAddress(size_t frame, size_t offset) const {
    return (frame << OFFSET_BITS) | offset;
  }

  size_t getMemory(size_t physical) const {
    return memory.at(physical);
  }

  size_t getPageTable(size_t physical) const {
    return page_table.at(physical);
  }

  bool getPageValid(size_t physical) const {
    return page_valid.at(physical);
  }

  void loadPageFromBackingStore(size_t page_number, size_t *buffer) {
    backing_store.seekg(page_number * PAGE_SIZE, std::ios::beg);
    backing_store.read(reinterpret_cast<char *>(buffer), PAGE_SIZE);
  }
};
