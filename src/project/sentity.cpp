// sentity.cpp
// Licensed under LGPLv2.1

#include <sierra/project/sentity.hpp>

namespace SierraEditor::Project {
    void SEntity::setName(const std::string& name) {
        mName = name;
    }

    std::string SEntity::getName() const {
        return mName;
    }

    void SEntity::setActive(bool active) {
        mActive = active;
    }

    bool SEntity::isActive() const {
        return mActive;
    }

    void SEntity::addComponent(const std::string& componentName) {
        mComponents.insert(componentName);
    }

    void SEntity::removeComponent(const std::string& componentName) {
        mComponents.erase(componentName);
    }

    bool SEntity::hasComponent(const std::string& componentName) const {
        return mComponents.find(componentName) != mComponents.end();
    }
}