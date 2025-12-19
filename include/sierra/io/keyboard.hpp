// keyboard.hpp
// Licensed under LGPLv2.1

#pragma once

#include <sierra/logger.hpp>
#include <unordered_map>
#include <mutex>
#include <QKeyEvent>

namespace SierraEditor::IO {

    class Keyboard {
    public:
        Keyboard() = default;
        // No copy and move semantics
        Keyboard(const Keyboard&) = delete;
        Keyboard& operator=(const Keyboard&) = delete;
        Keyboard(Keyboard&&) = delete;
        Keyboard& operator=(Keyboard&&) = delete;

        ~Keyboard() = default;

        // Get the singleton instance of the Keyboard class
        static Keyboard& getInstance() { static Keyboard instance; return instance; }
        
        // Check if a specific key is currently pressed
        static bool isKeyPressed(int keyCode);
        
        static void handleKeyPress(QKeyEvent* event);
        static void handleKeyRelease(QKeyEvent* event);

    private:
        std::unordered_map<int, bool> mKeyStates;

        mutable std::mutex mMutex;
    };

}