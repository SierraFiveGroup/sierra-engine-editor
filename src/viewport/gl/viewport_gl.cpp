// viewport_gl.cpp
// Licensed under LGPLv2.1

#include <sierra/viewport/gl/viewport_gl.hpp>
#include <QOpenGLShader>

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

        mShader.release();

        // Draw debug text overlay
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        
        // Setup orthographic projection for text rendering
        QMatrix4x4 orthoProj;
        orthoProj.ortho(0, width(), height(), 0, -1, 1);
        
        mShader.bind();
        mShader.setUniformValue("uProj", orthoProj);
        mShader.setUniformValue("uView", QMatrix4x4());
        
        QMatrix4x4 textModel;
        float scale = 1.5f;
        textModel.scale(scale, scale, 1.0f);
        mShader.setUniformValue("uModel", textModel);

        // Prepare text
        char buffer[256];
        float fps = 1.0f / dt;
        glm::vec3 camPos = mCamera.getGLMPosition();
        glm::vec2 camOri = mCamera.getGLMYawPitch(); // X=Yaw, Y=Pitch

        snprintf(buffer, sizeof(buffer),
            "Frame Time: %.2f ms (%.1f FPS)\nPos: (%.2f, %.2f, %.2f)\nYaw: %.2f Pitch: %.2f",
            dt * 1000.0f, fps,
            camPos.x, camPos.y, camPos.z, camOri.x, camOri.y);
        
        // stb_easy_font characters are ~8px wide, ~9px tall at scale 1.0
        // Estimate text dimensions (longest line is ~35 chars)
        float margin = 10.0f / scale; // 10px margin in scaled space
        float charWidth = 8.0f;
        float lineHeight = 9.0f;
        float estimatedWidth = 35.0f * charWidth; // ~280px at scale 1.0
        float estimatedHeight = 3.0f * lineHeight; // 3 lines
        
        float x = (width() / scale) - estimatedWidth - margin;
        float y = (height() / scale) - estimatedHeight - margin;
        
        static char stb_buffer[99999];
        int num_quads = stb_easy_font_print(x, y, buffer, NULL, stb_buffer, sizeof(stb_buffer));

        if (num_quads > 0) {
            // Create VAO and VBO for text
            GLuint textVAO, textVBO;
            glGenVertexArrays(1, &textVAO);
            glGenBuffers(1, &textVBO);
            
            glBindVertexArray(textVAO);
            glBindBuffer(GL_ARRAY_BUFFER, textVBO);
            glBufferData(GL_ARRAY_BUFFER, num_quads * 4 * 16, stb_buffer, GL_DYNAMIC_DRAW);
            
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 16, (void*)0);
            
            glDrawArrays(GL_QUADS, 0, num_quads * 4);
            
            glBindVertexArray(0);
            glDeleteBuffers(1, &textVBO);
            glDeleteVertexArrays(1, &textVAO);
        }

        mShader.release();
        
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
    }
}