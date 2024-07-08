#include "MemoryManager.hpp"

int main(int argc, char* argv[]) {
  int offsetBits = 8;
  int pageBits = 8;
  int frameBits = 7;
  std::string backingStorageFile = "BACKING_STORE.bin";
  
  MemoryManager memoryManager(offsetBits, pageBits, frameBits, backingStorageFile);

  std::ifstream input_file (argv[1], std::ifstream::in);

  if (!input_file.is_open()) {
    return EXIT_FAILURE; // Archivo faltante
  }

  #if 1
  size_t logical = 0, physical = 0, value = 0;

  while (input_file >> logical) {
    std::cout << logical << std::endl;
    uint32_t pageNumber = memoryManager.getPageNumber(logical);
    uint32_t offset = memoryManager.getOffset(logical);

    if (memoryManager.getPageValid(pageNumber) == false) {  // page fault
        handlePageFault(pageNumber);
    }

    uint32_t frameNumber = memoryManager.getPageTable(pageNumber);
    physical = memoryManager.getPhysicalAddress(frameNumber, offset);
    value = memoryManager.getMemory(physical);
    std::cout << "Virtual address: " << logical << " Physical address: " << physical << " Value: " << value << std::endl;
  }
  #endif

  input_file.close();

  return EXIT_SUCCESS;
}
