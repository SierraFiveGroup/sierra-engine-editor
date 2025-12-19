// io_utils.hpp
// Licensed under LGPLv2.1

#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

namespace SierraEditor::IO {
    // Read all lines from a text file into a vector of strings
    static inline std::vector<std::string> readFileLines(const std::string& filepath) {
        std::vector<std::string> lines;
        std::ifstream file(filepath);
        if (!file.is_open()) {
            return lines;
        }

        std::string line;

        while (std::getline(file, line)) {
            lines.push_back(line);
        }
        file.close();

        return lines;
    }

    // Write a vector of strings to a text file, each string as a new line
    static inline bool writeFileLines(const std::string& filepath, const std::vector<std::string>& lines, bool overwrite = true) {
        std::ofstream file(filepath, overwrite ? std::ios::trunc : std::ios::app);
        if (!file.is_open()) {
            return false;
        }

        for (const auto& line : lines) {
            file << line << "\n";
        }
        file.close();

        return true;
    }

    // Check if a file exists
    static inline bool fileExists(const std::string& filepath) {
        std::ifstream file(filepath);
        bool exists = file.is_open();
        file.close();

        return exists;
    }

    // Delete a file; Checks if successful
    static inline bool deleteFile(const std::string& filepath) {
        if (std::remove(filepath.c_str()) != 0) {
            return false;
        }

        return true;
    }

    // Split a string by a delimiter into a vector of strings
    static inline std::vector<std::string> splitString(const std::string& str, char delimiter) {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(str);
        while (std::getline(tokenStream, token, delimiter)) {
            tokens.push_back(token);
        }

        return tokens;
    }

    // Platform-aware path splitter
    static inline std::vector<std::string> splitPath(const std::string& path) {
#ifdef _WIN32
        return splitString(path, '\\');
#else
        return splitString(path, '/');
#endif
    }

    static inline std::string stripLastPathComponent(const std::string& path) {
        auto components = splitPath(path);
        if (components.size() <= 1) {
            return "";
        }

        components.pop_back();
#ifdef _WIN32
        std::string result;
        for (size_t i = 0; i < components.size(); ++i) {
            result += components[i];
            if (i < components.size() - 1) {
                result += '\\';
            }
        }
        return result;
#else
        std::string result;
        for (size_t i = 0; i < components.size(); ++i) {
            result += components[i];
            if (i < components.size() - 1) {
                result += '/';
            }
        }
        return result;
#endif
    }
}