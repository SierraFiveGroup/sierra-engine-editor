// viewport_gl.cpp
// Licensed under LGPLv2.1

#include <sierra/viewport/gl/viewport_gl.hpp>

namespace SierraEditor::Viewport::GL {
    void ViewportGL::initializeGL() {
        initializeOpenGLFunctions();

        mDeltaTimer.start();

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);

        mShader.addShaderFromSourceFile(
            QOpenGLShader::Vertex, "../shaders/basic.vert");
        mShader.addShaderFromSourceFile(
            QOpenGLShader::Fragment, "../shaders/basic.frag");
        mShader.link();

        mMesh = std::make_unique<GL::Mesh>(static_cast<QOpenGLFunctions_4_5_Core*>(this));
        mMesh->create(); // VAO/VBO
    }

    void ViewportGL::resizeGL(int w, int h) {
        mProjection.setToIdentity();
        mProjection.perspective(
            60.0f, float(w) / float(h), 0.1f, 1000.0f);
    }

    void ViewportGL::paintGL() {
        float dt = mDeltaTimer.restart() / 1000.0f;
        dt = std::min(dt, 0.05f); // Clamp to avoid large jumps

        mCamera.update(dt);

        glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mShader.bind();
        mShader.setUniformValue("uProj", mProjection);
        mShader.setUniformValue("uView", mCamera.view());
        mShader.setUniformValue("uModel", QMatrix4x4());

        mMesh->draw();
    }
}