// mouse.cpp
// Licensed under LGPLv2.1

#include <sierra/io/mouse.hpp>
#include <sierra/logger.hpp>

namespace SierraEditor::IO {
    bool Mouse::isButtonPressed(int buttonCode) {
        auto& instance = getInstance();
        auto it = instance.mButtonStates.find(buttonCode);
        if (it != instance.mButtonStates.end()) {
            return it->second;
        }
        return false;
    }

    float Mouse::getMouseX() {
        return getInstance().mMouseX;
    }
    
    float Mouse::getMouseY() {
        return getInstance().mMouseY;
    }

    void Mouse::handleButtonPress(QMouseEvent* event) {
        auto& instance = getInstance();
        instance.mButtonStates[event->button()] = true;
    }

    void Mouse::handleButtonRelease(QMouseEvent* event) {
        auto& instance = getInstance();
        instance.mButtonStates[event->button()] = false;
    }

    void Mouse::handleMouseMove(QMouseEvent* event) {
        auto& instance = getInstance();
        instance.mMouseX = static_cast<float>(event->position().x());
        instance.mMouseY = static_cast<float>(event->position().y());
    }
}