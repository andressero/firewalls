// Copyright [2024] <Andres Quesada, Pablo Cascante, Diego Bolanos, Andres
// Serrano>"

#ifndef FILEPROPERTIES_H
#define FILEPROPERTIES_H

#include "Definitions.hpp"

/**
 * @brief Properties every file on FileSystem MUST have.
 *
 */
class FileProperties {
private:
    /**
   * @brief File name
   *
   */
    std::string name;
    /**
   * @brief Date in which the file was created
   *
   */
    std::string date;
    /**
   * @brief Files owner
   *
   */
    std::string owner;
    /**
   * @brief First block the file used to be stored on unit.
   *
   */
    BLOCK_INDEX startingBlock;
    /**
   * @brief Last block the file used to be stored on unit.
   *
   */
    BLOCK_INDEX lastBlock;
    /**
   * @brief It is an index relative to the beginning of the file.
   *
   */
    UNIT_INDEX cursor;
    /**
   * @brief Index of the entry in which the file is located on directory.
   *
   */
    DIRECTORY_INDEX directoryIndex;
    /**
   * @brief Opened file modes. There's two possible modes: Read(false),
   * Write(true).
   *
   */
    bool ReadWriteMode = false;

public:
    /**
   * @brief Default constructor. It gives initial values to every member.
   *
   * @details Initial values on FileProperties: startingBlock(UNUSED),
   * lastBlock(UNUSED),cursor(CLOSED) and directoryIndex(NOT_ON_DIR).
   * String members start as an empty string.
   *
   * @see FileProperties(const std::string name, const std::string date,
   * std::string owner)
   * @see FileProperties(FileProperties &properties)
   *
   */
    FileProperties();
    /**
   * @brief Constructor that gives initial values to all except name, date and
   * owner, which are set with params.
   *
   * @param name Name of the file.
   * @param date Date in which the file was created.
   * @param owner Files owner.
   *
   * @see FileProperties(FileProperties &properties)
   * @see FileProperties()
   *
   */
    FileProperties(const std::string name, const std::string date,
                   const std::string owner);
    // ? Is it necessary?
    /**
   * @brief Copy constructor
   *
   * @param FileProperties object to copy into a new FileProperties object.
   *
   * @see FileProperties(const std::string name, const std::string date, const
   * std::string owner)
   * @see FileProperties()
   *
   */
    explicit FileProperties(const FileProperties &properties);
    // ? Is it useful now?
    __attribute__((const)) bool valid() const;
    /**
   * @brief Assign operator to give the value of the right properties value to
   * the left one.
   *
   * @param properties Properties of another file.
   */
    inline void operator=(const FileProperties &properties) {
        this->name = properties.name;
        this->date = properties.date;
        this->owner = properties.owner;
        this->startingBlock = properties.startingBlock;
        this->cursor = properties.cursor;
        this->lastBlock = properties.lastBlock;
        this->directoryIndex = properties.directoryIndex;
    }
    /**
   * @brief Equals operator to compare one file properties value to another.
   *
   * @param properties Properties of another file.
   * @return true If both are equal.
   * @return false Otherwise.
   */
    bool operator==(const FileProperties &properties) const {
        const bool equalName = this->name == properties.name;
        const bool equalDate = this->date == properties.date;
        const bool equalOwner = this->owner == properties.owner;
        const bool equalStartingBlock =
            this->startingBlock == properties.startingBlock;
        const bool equalCursor = this->cursor == properties.cursor;
        const bool equallastBlock = this->lastBlock == properties.lastBlock;
        const bool equalDirectoryIndex =
            this->directoryIndex == properties.directoryIndex;
        return equalName && equalDate && equalOwner && equalStartingBlock &&
               equalCursor && equallastBlock && equalDirectoryIndex;
    }
    /**
   * @brief The opposite of equals operator.
   *
   * @see operator==(const FileProperties &properties)
   *
   */
    bool operator!=(const FileProperties &properties) const {
        return !(*this == properties);
    }

    // Gets

    /**
   * @brief Get the Files name.
   *
   * @return the name of the file. It'll be an empty string if there's no name
   * yet.
   */
    inline const std::string &getName() const { return this->name; }
    /**
   * @brief Get the Files date
   *
   * @return the date of the file. It'll be an empty string if there's no date
   * yet.
   */
    inline const std::string &getDate() const { return this->date; }
    /**
   * @brief Get the Files owner.
   *
   * @return the owner of the file. It'll be an empty string if there's no owner
   * yet.
   */
    inline const std::string &getOwner() const { return this->owner; }
    /**
   * @brief Get the Starting block that File uses to be stored in Unit.
   *
   * @return the index to the starting block.
   *
   * @see startingBlock
   *
   */
    inline const BLOCK_INDEX &getStartingBlock() const {
        return this->startingBlock;
    }
    /**
   * @brief Get the Files cursor
   *
   * @see cursor
   *
   * @return Files cursor value.
   */
    inline const UNIT_INDEX &getCursor() const { return this->cursor; }
    /**
   * @brief Get the index of the File on Directory.
   *
   * @return Files index on directory.
   *
   * @see directoryIndex
   *
   */
    inline const DIRECTORY_INDEX &getDirectoryIndex() const {
        return this->directoryIndex;
    }
    /**
   * @brief Get the Last block that File uses to be stored in Unit.
   *
   * @return the index to the last block.
   *
   * @see lastBlock
   *
   */
    inline const BLOCK_INDEX &getLastBlock() const { return this->lastBlock; }
    /**
   * @brief Get the mode in which the File is opened.
   *
   * @see ReadWriteMode
   *
   */
    inline const bool &getReadWriteMode() const { return this->ReadWriteMode; }

    // Sets

    /**
   * @brief Sets the cursor position.
   *
   * @param cursor index in which the cursor will be positioned on Files
   * content.
   */
    inline void seek(UNIT_INDEX cursor) {
        ASSERT(cursor >= 0 || cursor == CLOSED);
        this->cursor = cursor;
    }
    /**
   * @brief Set the name of the File to be a certain one.
   *
   * @param name Name that the file will have.
   *
   */
    inline void setName(const std::string &name) { this->name = name; }
    /**
   * @brief Set the date of the File to be a certain one.
   *
   * @param date Date that the file will have.
   *
   */
    inline void setDate(const std::string &date) { this->date = date; }
    /**
   * @brief Set the owner of the File to be a certain one.
   *
   * @param owner Owner that the file will have.
   *
   */
    inline void setOwner(const std::string &owner) { this->owner = owner; }
    /**
   * @brief Set the Starting Block to be a certain block_index in Unit
   *
   * @param startingBlock starting block in which the File is supposed to be
   * stored.
   */
    inline void setStartingBlock(BLOCK_INDEX startingBlock) {
        ASSERT(startingBlock >= 0 || startingBlock == UNUSED);
        this->startingBlock = startingBlock;
    }
    /**
   * @brief Set the last Block to be a certain block_index in Unit
   *
   * @param lastBlock last block in which the File is supposed to be stored.
   */
    inline void setLastBlock(BLOCK_INDEX lastBlock) {
        ASSERT(lastBlock >= 0 || lastBlock == UNUSED);
        this->lastBlock = lastBlock;
    }
    /**
   * @brief Set index to the entry on directory which is supposed to store the
   * File.
   *
   * @param directoryIndex entry index to set the File.
   */
    inline void setDirectoryIndex(DIRECTORY_INDEX directoryIndex) {
        ASSERT(directoryIndex >= 0 || directoryIndex == NOT_ON_DIR);
        this->directoryIndex = directoryIndex;
    }
    /**
   * @brief Set the Files ReadWriteMode to a certain one.
   *
   * @param mode false for read, true for write mode.
   *
   * @see ReadWriteMode
   *
   */
    inline void changeMode(bool mode) {
        if (mode) {
            // Write
            this->ReadWriteMode = true;
        } else {
            // Read
            this->ReadWriteMode = false;
        }
        this->seek(0);
    }
};

#endif
