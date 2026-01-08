// sentity.hpp
// Licensed under LGPLv2.1

#pragma once

#include <unordered_set>
#include <string>

namespace SierraEditor::Project {
    class SEntity {
        public:
            SEntity() = default;

            // TODO: Define how copying and moving entities should work

            ~SEntity() = default;

            void setName(const std::string& name);
            std::string getName() const;
            void setActive(bool active);
            bool isActive() const;

            void addComponent(const std::string& componentName);
            void removeComponent(const std::string& componentName);
            bool hasComponent(const std::string& componentName) const;
            const std::unordered_set<std::string>& getComponents() const {
                return mComponents;
            }
            // TODO: getComponentByName/ById

        private:
            std::string mName;
            bool mActive = true;
            std::unordered_set<std::string> mComponents; // TODO: Discuss how components will work in general, this is a placeholder
                                                         // This will probably be linked to symbols from loaded ScriptableComponents (DLLs/SOs)
    };
}