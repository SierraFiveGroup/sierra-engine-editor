// viewport_gl.hpp
// Licensed under LGPLv2.1

#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
#include <QElapsedTimer>
#include <QTimer>

#include <memory>

#include <sierra/viewport/gl/camera.hpp>
#include <sierra/viewport/gl/mesh.hpp>

namespace SierraEditor::Viewport::GL {
    class ViewportGL final : public QOpenGLWidget, protected QOpenGLFunctions_4_5_Core {
        Q_OBJECT
        public:
            explicit ViewportGL(QWidget* parent = nullptr) : QOpenGLWidget(parent) {
                setFocusPolicy(Qt::StrongFocus);

                mFrameTick.setInterval(16); // ~60 Hz
                connect(&mFrameTick, &QTimer::timeout, this, QOverload<>::of(&ViewportGL::update));
                mFrameTick.start();
            }
        
        protected:
            void initializeGL() override;

            void resizeGL(int w, int h) override;

            void paintGL() override;

        private:
            QOpenGLShaderProgram mShader;
            QMatrix4x4 mProjection;
            GL::Camera mCamera;
            std::unique_ptr<GL::Mesh> mMesh;
            QElapsedTimer mDeltaTimer;
            QTimer mFrameTick;
    };
}