// selection_manager.hpp
// Licensed under LGPLv2.1

#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <memory>

#include <sierra/project/sentity.hpp>

namespace SierraEditor::Stateful {
    class SelectionManager {
        public:
            SelectionManager() = default;
            // No copy and move
            SelectionManager(const SelectionManager&) = delete;
            SelectionManager& operator=(const SelectionManager&) = delete;
            SelectionManager(SelectionManager&&) = delete;
            SelectionManager& operator=(SelectionManager&&) = delete;
            
            ~SelectionManager() = default;

            static SelectionManager& getInstance() {
                static SelectionManager instance;
                return instance;
            }

            // Set the currently selected entity in the editor (mainly hierarchy panel)
            void setSelectedEntity(Project::SEntity* entity) {
                mSelectedEntity = entity;
            }

            // Get the currently selected entity
            Project::SEntity* getSelectedEntity() const {
                return mSelectedEntity;
            }
        
        private:
            // Vector of saved scene states (copies)
            Project::SEntity* mSelectedEntity = nullptr;
    };
}