// mesh.cpp
// Licensed under LGPLv2.1

#include <sierra/viewport/gl/mesh.hpp>

namespace SierraEditor::Viewport::GL {
    // Simple cube data with positions and colors (pos XYZ, color RGB)
    static const float cubeVertices[] = {
        // Front
        -1,-1, 1,  0.8f, 0.8f, 0.8f,    1,-1, 1,  0.8f, 0.8f, 0.8f,    1, 1, 1,  0.8f, 0.8f, 0.8f,
        -1,-1, 1,  0.8f, 0.8f, 0.8f,    1, 1, 1,  0.8f, 0.8f, 0.8f,   -1, 1, 1,  0.8f, 0.8f, 0.8f,
        // Back
        -1,-1,-1,  0.8f, 0.8f, 0.8f,   -1, 1,-1,  0.8f, 0.8f, 0.8f,    1, 1,-1,  0.8f, 0.8f, 0.8f,
        -1,-1,-1,  0.8f, 0.8f, 0.8f,    1, 1,-1,  0.8f, 0.8f, 0.8f,    1,-1,-1,  0.8f, 0.8f, 0.8f,
        // Left
        -1,-1,-1,  0.8f, 0.8f, 0.8f,   -1,-1, 1,  0.8f, 0.8f, 0.8f,   -1, 1, 1,  0.8f, 0.8f, 0.8f,
        -1,-1,-1,  0.8f, 0.8f, 0.8f,   -1, 1, 1,  0.8f, 0.8f, 0.8f,   -1, 1,-1,  0.8f, 0.8f, 0.8f,
        // Right
         1,-1,-1,  0.8f, 0.8f, 0.8f,    1, 1,-1,  0.8f, 0.8f, 0.8f,    1, 1, 1,  0.8f, 0.8f, 0.8f,
         1,-1,-1,  0.8f, 0.8f, 0.8f,    1, 1, 1,  0.8f, 0.8f, 0.8f,    1,-1, 1,  0.8f, 0.8f, 0.8f,
        // Top
        -1, 1,-1,  0.8f, 0.8f, 0.8f,   -1, 1, 1,  0.8f, 0.8f, 0.8f,    1, 1, 1,  0.8f, 0.8f, 0.8f,
        -1, 1,-1,  0.8f, 0.8f, 0.8f,    1, 1, 1,  0.8f, 0.8f, 0.8f,    1, 1,-1,  0.8f, 0.8f, 0.8f,
        // Bottom
        -1,-1,-1,  0.8f, 0.8f, 0.8f,    1,-1,-1,  0.8f, 0.8f, 0.8f,    1,-1, 1,  0.8f, 0.8f, 0.8f,
        -1,-1,-1,  0.8f, 0.8f, 0.8f,    1,-1, 1,  0.8f, 0.8f, 0.8f,   -1,-1, 1,  0.8f, 0.8f, 0.8f
    };

    Mesh::Mesh(QOpenGLFunctions_4_5_Core* gl)
        :
        mGL(gl)
    {}

    Mesh::~Mesh() {
        destroy();
    }

    void Mesh::create() {
        mVertexCount = sizeof(cubeVertices) / (6 * sizeof(float));

        mGL->glGenVertexArrays(1, &mVAO);
        mGL->glGenBuffers(1, &mVBO);

        mGL->glBindVertexArray(mVAO);
        mGL->glBindBuffer(GL_ARRAY_BUFFER, mVBO);
        mGL->glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(cubeVertices),
            cubeVertices,
            GL_STATIC_DRAW
        );

        // Position attribute
        mGL->glEnableVertexAttribArray(0);
        mGL->glVertexAttribPointer(
            0, 3, GL_FLOAT, GL_FALSE,
            6 * sizeof(float),
            nullptr
        );

        // Color attribute
        mGL->glEnableVertexAttribArray(1);
        mGL->glVertexAttribPointer(
            1, 3, GL_FLOAT, GL_FALSE,
            6 * sizeof(float),
            (void*)(3 * sizeof(float))
        );

        mGL->glBindVertexArray(0);
    }

    void Mesh::destroy() {
        if (mVBO) mGL->glDeleteBuffers(1, &mVBO);
        if (mVAO) mGL->glDeleteVertexArrays(1, &mVAO);
        mVAO = mVBO = 0;
    }

    void Mesh::draw() const {
        mGL->glBindVertexArray(mVAO);
        mGL->glDrawArrays(GL_TRIANGLES, 0, mVertexCount);
        mGL->glBindVertexArray(0);
    }
}