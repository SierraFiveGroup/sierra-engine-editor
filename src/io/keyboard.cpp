// keyboard.cpp
// Licensed under LGPLv2.1

#include <sierra/io/keyboard.hpp>

namespace SierraEditor::IO {
    bool Keyboard::isKeyPressed(int keyCode) {
        std::lock_guard<std::mutex> lock(getInstance().mMutex);
        auto& keyStates = getInstance().mKeyStates;
        auto it = keyStates.find(keyCode);
        if (it != keyStates.end()) {
            return it->second;
        }
        return false;
    }

    void Keyboard::handleKeyPress(QKeyEvent* event) {
        LOG("Holy shit a key got pressed!");
        std::lock_guard<std::mutex> lock(getInstance().mMutex);
        getInstance().mKeyStates[event->key()] = true;
    }

    void Keyboard::handleKeyRelease(QKeyEvent* event) {
        LOG("Holy shit a key got released!");
        std::lock_guard<std::mutex> lock(getInstance().mMutex);
        getInstance().mKeyStates[event->key()] = false;
    }
}