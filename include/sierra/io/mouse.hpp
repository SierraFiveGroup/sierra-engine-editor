// mouse.hpp
// Licensed under LGPLv2.1

#pragma once

#include <unordered_map>
#include <QMouseEvent>
#include <QInputEvent>
#include <QCursor>

namespace SierraEditor::IO {
    class Mouse {
        public:
            Mouse() = default;
            // No copy and move semantics
            Mouse(const Mouse&) = delete;
            Mouse& operator=(const Mouse&) = delete;
            Mouse(Mouse&&) = delete;
            Mouse& operator=(Mouse&&) = delete;

            ~Mouse() = default;

            // Get the singleton instance of the Mouse class
            static Mouse& getInstance() { static Mouse instance; return instance; }

            static bool isButtonPressed(int buttonCode);
            static float getMouseX();
            static float getMouseY();

            static void handleButtonPress(QMouseEvent* event);
            static void handleButtonRelease(QMouseEvent* event);
            static void handleMouseMove(QMouseEvent* event);
        
        private:
            float mMouseX = 0.0f;
            float mMouseY = 0.0f;
            std::unordered_map<int, bool> mButtonStates;
    };
}