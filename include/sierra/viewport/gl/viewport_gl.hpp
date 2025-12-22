// viewport_gl.hpp
// Licensed under LGPLv2.1

#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_1_Core>
#include <QOpenGLShaderProgram>
#include <QSurfaceFormat>
#include <QMatrix4x4>
#include <QElapsedTimer>
#include <QTimer>

#include <memory>

#include <sierra/viewport/gl/camera.hpp>
#include <sierra/viewport/gl/mesh.hpp>
#include <sierra/logger.hpp>

#ifndef STB_EASY_FONT_IMPLEMENTATION
#define STB_EASY_FONT_IMPLEMENTATION
#endif

#include <stb/stb_easy_font.h>

namespace SierraEditor::Viewport::GL {
    class ViewportGL final : public QOpenGLWidget, protected QOpenGLFunctions_4_1_Core {
        Q_OBJECT
        public:
            explicit ViewportGL(QWidget* parent = nullptr) : QOpenGLWidget(parent) {
                // Ensure per-widget format matches global defaults
                QSurfaceFormat fmt;
                fmt.setRenderableType(QSurfaceFormat::OpenGL);
                fmt.setVersion(4, 1);
                fmt.setProfile(QSurfaceFormat::CoreProfile);
                fmt.setDepthBufferSize(24);
                fmt.setStencilBufferSize(8);
                fmt.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
                fmt.setSamples(0);
                setFormat(fmt);

                // Set focus policy to only accept when clicked into viewport
                setFocusPolicy(Qt::ClickFocus);

                mFrameTick.setInterval(16); // ~60 Hz
                connect(&mFrameTick, &QTimer::timeout, this, QOverload<>::of(&ViewportGL::update));
                mFrameTick.start();

                TODO("Make grid rendering more efficient (don't create VBO/VAO every frame)");
            }

            void setRenderCoordinateSystemEnabled(bool enabled) {
                mRenderCoords = enabled;
            }

            bool getRenderCoordinateSystemEnabled() const {
                return mRenderCoords;
            }
        
        protected:
            void initializeGL() override;
            void resizeGL(int w, int h) override;
            void paintGL() override;

        private:
            void mRenderCoordinateSystem();
            
            QOpenGLShaderProgram mShader;
            bool mShaderReady = false;
            QMatrix4x4 mProjection;
            GL::Camera mCamera;
            std::unique_ptr<GL::Mesh> mMesh;
            QElapsedTimer mDeltaTimer;
            QTimer mFrameTick;
            bool mRenderCoords = true;
    };
}