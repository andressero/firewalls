// Copyright [2024] <Pablo Cascante, Andres Quesada, Andres Serrano>"

#include <iostream>
#include <fstream>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define OFFSET_BITS 8
#define PAGE_BITS 8
#define FRAME_BITS 7    // the physical memory is smaller than the virtual one

#define PAGE_SIZE (1 << OFFSET_BITS)
#define FRAME_SIZE PAGE_SIZE

#define NUMBER_OF_PAGES (1 << PAGE_BITS)
#define NUMBER_OF_FRAMES (1 << FRAME_BITS)
#define MEMORY_SIZE (FRAME_SIZE * NUMBER_OF_FRAMES)

#define GET_PAGE_NUMBER(address) ((address & ((1 << (OFFSET_BITS + PAGE_BITS))- (1 << OFFSET_BITS))) >> OFFSET_BITS)
#define GET_OFFSET(address) (address & ((1 << OFFSET_BITS) - 1))
#define GET_PHYSICAL_ADDRESS(frame, offset) ((frame_number << OFFSET_BITS) | offset)

int8_t memory[MEMORY_SIZE];
uint32_t page_table[NUMBER_OF_PAGES], next_available_frame;
uint8_t page_valid[NUMBER_OF_PAGES];

uint32_t total_cnt, page_fault_cnt;

// uint32_t select_victim_frame() {
//     // FIFO
//     if(next_available_frame < NUMBER_OF_FRAMES) {
//         return next_available_frame++;
//     }
//     uint32_t victim = (next_available_frame++) % NUMBER_OF_FRAMES;
//     for(size_t i = 0; i != NUMBER_OF_PAGES; ++i) {  // invalidate the victim page
//         if(page_valid[i] && page_table[i] == victim) {
//             page_valid[i] = 0;
//             break;
//         }
//     }
//     return victim;
// }

// void handle_page_fault(uint32_t page_number) {
//     page_table[page_number] = select_victim_frame();
//     fseek(backing_storage, page_number * PAGE_SIZE, SEEK_SET);
//     fread(memory + page_table[page_number] * PAGE_SIZE, sizeof(int8_t), PAGE_SIZE, backing_storage);
//     page_valid[page_number] = 1;
//     ++page_fault_cnt;
// }

uint32_t translate_address(uint32_t logical) {
    ++total_cnt;
    uint32_t page_number, offset, frame_number;
    page_number = GET_PAGE_NUMBER(logical);
    offset = GET_OFFSET(logical);
    if(page_valid[page_number] == 0) {    // page fault
      handle_page_fault(page_number);
    }
    frame_number = page_table[page_number];
    return GET_PHYSICAL_ADDRESS(frame_number, offset);
}

int main(int argc, char **argv) {
  std::ifstream input_file (argv[1], std::ifstream::in);

  if (!input_file.is_open()) {
    return EXIT_FAILURE; // Archivo faltante
  }

  memset(page_valid, 0, sizeof(uint8_t) * NUMBER_OF_PAGES);

  #if 1
  uint32_t logical = 0, physical = 0;
  int8_t value = 0;

  while (input_file >> logical) {
    std::cout << logical << std::endl;
    physical = translate_address(logical);
    value = memory[physical];
    std::cout << "Virtual address: " << logical << " Physical address: " << physical << " Value: " << value << std::endl;
  }

  input_file.close();
  #endif

  /*
  char line[8];
  while(fgets(line, 8, input_file)) {
    uint32_t logical, physical;
    int8_t value;
    sscanf(line, "%u", &logical);
    physical = translate_address(logical);
    value = memory[physical];
    printf("Virtual address: %u Physical address: %u Value: %d\n", logical, physical, value);
  }
  fclose(input_file);
  */
  return EXIT_SUCCESS;
}
