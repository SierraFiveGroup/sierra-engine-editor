// sproject.hpp - .sierra project file wrapper
// Licensed under LGPLv2.1

#pragma once

#include <string>
#include <cstdint>
#include <sierra/io/io_utils.hpp>
//#include <rapidxml/rapidxml.hpp>
#include <iostream>
#include <sierra/logger.hpp>

#include <QDate>

namespace SierraEditor::Project {
    // TODO: Move these to unordered_map for extensibility?
    struct SProjectData {
        uint16_t version = 0; // If zero then not loaded
        std::string name;
        std::string filePath;
        std::string assetsPath;
        std::string scriptsPath;
        std::string scenesPath;
        std::string createdDate;
        std::string modifiedDate;
        std::string lastOpenedScene;
    };

    class SProject {
        public:
            SProject(const std::string& filepath = "");

            bool load();
            bool save();

            // Override the file path to load; Will fail if already loaded
            bool overrideFilePath(const std::string& newFilePath) {
                if (mData.filePath != "" && mData.version != 0) {
                    return false; // Can only overwrite if empty and if loaded
                }

                mData.filePath = newFilePath;
                return true;
            }

            void setLastScene(const std::string& sceneName) {
                mData.lastOpenedScene = sceneName;
            }

            uint16_t getVersion() const { return mData.version; }
            std::string getName() const { return mData.name; }
            std::string getFilePath() const { return mData.filePath; }
            std::string getAssetsPath() const { return mData.assetsPath; }
            std::string getScriptsPath() const { return mData.scriptsPath; }
            std::string getScenesPath() const { return mData.scenesPath; }
            std::string getCreatedDate() const { return mData.createdDate; }
            std::string getModifiedDate() const { return mData.modifiedDate; }
            std::string getLastOpenedScene() const { return mData.lastOpenedScene; }

        private:
            SProjectData mData;
    };
}