// stateful_manager.hpp
// Licensed under LGPLv2.1

#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <memory>

#include <sierra/project/sproject.hpp>
#include <sierra/project/sscene.hpp>

namespace SierraEditor::Stateful {
    class StatefulManager {
        public:
            StatefulManager() = default;
            // No copy and move
            StatefulManager(const StatefulManager&) = delete;
            StatefulManager& operator=(const StatefulManager&) = delete;
            StatefulManager(StatefulManager&&) = delete;
            StatefulManager& operator=(StatefulManager&&) = delete;
            
            ~StatefulManager() = default;

            static StatefulManager& getInstance() {
                static StatefulManager instance;
                return instance;
            }

            // Save the current state of the given scene
            void saveCurrentState(std::shared_ptr<Project::SScene> scene);

            // Get state at index
            std::shared_ptr<Project::SScene> getState(size_t index);

            // Traverse back
            std::shared_ptr<Project::SScene> undo();

            // Traverse forward
            std::shared_ptr<Project::SScene> redo();

            // Purge n states from front
            void purgeFrontStates(size_t n);
        
        private:
            // Vector of saved scene states (copies)
            std::vector<std::shared_ptr<Project::SScene>> mSavedStates = {};

            size_t mCurrentStateIndex = 0;
            size_t mMaxSavedStates = 50; // Max number of saved states to keep; This may be modifiable in the future
    };
}