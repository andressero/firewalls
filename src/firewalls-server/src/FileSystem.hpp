// Copyright [2024] <Andres Quesada, Pablo Cascante, Diego Bolanos,
// Andres Serrano>"

#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

#include "Definitions.hpp"
#include "FileProperties.hpp"

#include <string>

/**
 * @brief File System that give all the characteristics to handle files within a
 * bigger System that may need it.
 *
 * @details Detailed information about this kind of Systems can be found in
 * Chapters 13 and 14 of "Operating Systems Concepts Tenth Edition"(2018) by
 * A. Silberschatz, P.B. Galvin & G. Gagne. https://www.os-book.com/OS10/
 *
 */
class FileSystem {
private:
  /**
   * @brief Array that holds all properties of the Files within the System.
   *
   */
  FileProperties *directory;
  /**
   * @brief Array that holds all content of the Files within the System.
   *
   */
  u8 *unit;
  /**
   * @brief Array that represents units usage, storing the connections in
   * between blocks that store a file content together.
   *
   */
  BLOCK_INDEX *FAT;
  /**
   * @brief Gives the index to the first free Block in Unit.
   *
   * @return The first free Block index if there's one.
   * ERROR_NO_BLOCKS_AVAILABLE otherwise.
   *
   * @see error_code
   *
   */
  BLOCK_INDEX findFirstUnusedBlock();
  /**
   * @brief Searches for a File with that name in directory.
   *
   * @param name Name of the file to search.
   * @return index of file in directory or ERROR_EMPTY_FILENAME if failed cause
   * of name being an empty string, ERROR_NO_FILE_BY_THAT_NAME if it was unable
   * to find the file being searched.
   */
  DIRECTORY_INDEX search(const std::string name);
  /**
   * @brief Get the amount of free space in Unit.
   *
   * @return amount of free space in Unit.
   */
  // ! Check use
  i64 getFreeSpace();
  /**
   * @brief
   *
   * @param block
   * @param data
   */
  void replace(u64 block, std::string data);
  /**
   * @brief
   *
   * @param name Files name
   * @return i64
   */
  i64 findEOF(const std::string name);

public:
  FileSystem();
  ~FileSystem();
  /**
   * @brief It reserves a first block on FAT and gives an entry in directory
   * with the initial properties and the name, date and owner the params
   * provided.
   *
   * @param name Name of the file being created.
   * @param date Files creation date.
   * @param owner Owner of the file being created.
   * @return true if the Files creation succeeded.
   * @return false Otherwise.
   */
  bool create(const std::string name, const std::string date,
              const std::string owner);
  /**
   * @brief Efface/Deletes the file from the directory, unit and FAT.
   * 
   * @param name Name of the file to efface.
   * @return true If the effacing succeeded.
   * @return false Otherwise.
   */
  bool efface(const std::string name);
  /**
   * @brief 
   * 
   * @param name 
   * @return true 
   * @return false 
   */
  bool open(const std::string name);
  /**
   * @brief 
   * 
   * @param name 
   * @return true 
   * @return false 
   */
  bool close(const std::string name);
  /**
   * @brief 
   * 
   * @param name 
   * @param buffer 
   * @param bufferSize 
   * @return true 
   * @return false 
   */
  bool write(const std::string name, std::string &buffer, i64 bufferSize);
  /**
   * @brief 
   * 
   * @param name 
   * @param buffer 
   * @param bufferSize 
   * @return true 
   * @return false 
   */
  bool append(const std::string name, std::string &buffer, i64 bufferSize);
  /**
   * @brief 
   * 
   * @param name 
   * @param readSize 
   * @return std::string 
   */
  std::string read(const std::string name, size_t readSize);
  /**
   * @brief Prints the entire FileSystem.
   * 
   */
  void print();
  /**
   * @brief Gives a file outside of the FileSystem the contents of a File thats in it.
   * 
   */
  void DumpToFile();
  /**
   * @brief Gives the FileSystem the contents of a file outside itself.
   * 
   */
  void fillWithFile();
  /**
   * @brief Changes the mode of a file by that name to read Mode.
   * 
   * @param name Name of the file that will change its mode.
   */
  void change2ReadMode(const std::string name);
  /**
   * @brief Changes the mode of a file by that name to write Mode.
   * 
   * @param name Name of the file that will change its mode. 
   */
  void chang2WriteMode(const std::string name);
  /**
   * @brief Changes the position of the cursor within a file with that name.
   * 
   * @param name Name of the file.
   * @param cursor Position to set the cursor.
   */
  void changeCursor(const std::string name, UNIT_INDEX cursor);
};

#endif