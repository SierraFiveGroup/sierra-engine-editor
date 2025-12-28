// stateful_manager.cpp
// Licensed under LGPLv2.1

#include <sierra/stateful/stateful_manager.hpp>

namespace SierraEditor::Stateful {
    void StatefulManager::saveCurrentState(std::shared_ptr<Project::SScene> scene) {
        mSavedStates.push_back(*scene); // Copy the scene to saved states

        if (mMaxSavedStates > 0 && mSavedStates.size() > mMaxSavedStates) {
            mSavedStates.erase(mSavedStates.begin()); // Remove oldest state
        } else {
            mCurrentStateIndex = mSavedStates.size() - 1; // Update current index to latest
        }
    }

    std::shared_ptr<Project::SScene> StatefulManager::getState(size_t index) {
        if (index >= mSavedStates.size()) {
            return nullptr; // Out of bounds
        }
        return std::make_shared<Project::SScene>(mSavedStates[index]);
    }

    std::shared_ptr<Project::SScene> StatefulManager::undo() {
        if (mCurrentStateIndex == 0 || mSavedStates.empty()) {
            return nullptr; // No earlier state
        }
        mCurrentStateIndex--;
        return std::make_shared<Project::SScene>(mSavedStates[mCurrentStateIndex]);
    }

    std::shared_ptr<Project::SScene> StatefulManager::redo() {
        if (mCurrentStateIndex + 1 >= mSavedStates.size()) {
            return nullptr; // No later state
        }
        mCurrentStateIndex++;
        return std::make_shared<Project::SScene>(mSavedStates[mCurrentStateIndex]);
    }

    void StatefulManager::purgeFrontStates(size_t n) {
        if (n >= mSavedStates.size()) {
            mSavedStates.clear();
            mCurrentStateIndex = 0;
            return;
        }

        mSavedStates.erase(mSavedStates.begin(), mSavedStates.begin() + n);
        if (mCurrentStateIndex < n) {
            mCurrentStateIndex = 0;
        } else {
            mCurrentStateIndex -= n;
        }
    }
}