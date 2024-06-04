// Copyright [2024] <Andres Quesada, Pablo Cascante, Diego Bolanos, Andres
// Serrano>

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <fstream>
#include <string>

class Logger
{
 public:
    Logger() = default;
    ~Logger() = default;

    /**
     * @brief Writes "message" at an specified text file or log.txt by default.
     * 
     * @param message Message or Text to write at the logging text file.
     * @param filename log.txt by default or a certain text file name that the
     * message will be written on. This file will be used as a logging file.
     */
    void log(const std::string& message, const std::string& filename = "log.txt") {
        std::ofstream file(filename, std::ios_base::app);
        if (file.is_open()) {
            file << message << std::endl;
            file.close();
        } else {
            std::cerr << "Unable to open file " << filename << std::endl;
        }
    }
};

#endif