// render_viewport.hpp
// Licensed under LGPLv2.1

#pragma once

#include <QWidget>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_1_Core>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>

#include <string>

#ifndef STB_EASY_FONT_IMPLEMENTATION
#define STB_EASY_FONT_IMPLEMENTATION
#endif

#include <sierra/viewport/gl/viewport_gl.hpp>

#include <stb/stb_easy_font.h>

namespace SierraEditor::UI {
    class RenderViewport : public QWidget {
        Q_OBJECT
        public:
            explicit RenderViewport(QWidget* parent = nullptr, std::string rendermsg = "No Project Loaded!");

            void setRenderMessage(const std::string& msg) { mRenderMsg = msg; }

            // Switch from the default BlueScreenGL to the actual Scene View
            void switchToSceneView();

            // Toggle grid view (only in non-blue screen mode)
            void toggleGridView();

            // Toggle camera details view (only in non-blue screen mode)
            void toggleCameraDetailsView();
        
        private:
            QTabWidget* mTabs;
            std::string mRenderMsg;
            QOpenGLWidget* mSceneView = nullptr;
    };
    
    class BlueScreenGL : public QOpenGLWidget, protected QOpenGLFunctions_4_1_Core {
        Q_OBJECT
        public:
            explicit BlueScreenGL(QWidget* parent = nullptr, std::string* rendermsgPtr = nullptr);
        
        protected:
            void initializeGL() override;
            void paintGL() override;
            void resizeGL(int w, int h) override;

        private:
            std::string* mRenderMsgPtr = nullptr;
            QOpenGLShaderProgram mShader;
            QMatrix4x4 mProjection;
    };
}