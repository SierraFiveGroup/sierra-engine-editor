// sscene.hpp
// Licensed under LGPLv2.1

#pragma once

#include <string>
#include <cstdint>
#include <vector>
#include <sierra/io/io_utils.hpp>
#include <sierra/logger.hpp>
#include <unordered_set>

namespace SierraEditor::Project {
    struct SSceneData {
        uint16_t version = 0;
        std::string name;
        std::string filePath;
        //std::unordered_set<FutureEntityObjectOrStructIdk> entities;
    };

    class SScene {
        public:
            SScene(const std::string& filepath = "");
            
            // Define copy constructor and assignment
            SScene(const SScene& other) {
                mData = other.mData;
            }

            SScene& operator=(const SScene& other) {
                if (this != &other) {
                    mData = other.mData;
                }
                return *this;
            }

            bool load();
            bool save();

            uint16_t getVersion() const { return mData.version; }
            std::string getName() const { return mData.name; }
            std::string getFilePath() const { return mData.filePath; }

        private:
            SSceneData mData;
    };
}