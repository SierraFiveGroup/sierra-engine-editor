// mesh.cpp
// Licensed under LGPLv2.1

#include <sierra/viewport/gl/mesh.hpp>

namespace SierraEditor::Viewport::GL {
    // Simple cube data
    static const float cubeVertices[] = {
        // Front
        -1,-1, 1,   1,-1, 1,   1, 1, 1,
        -1,-1, 1,   1, 1, 1,  -1, 1, 1,
        // Back
        -1,-1,-1,  -1, 1,-1,   1, 1,-1,
        -1,-1,-1,   1, 1,-1,   1,-1,-1,
        // Left
        -1,-1,-1,  -1,-1, 1,  -1, 1, 1,
        -1,-1,-1,  -1, 1, 1,  -1, 1,-1,
        // Right
         1,-1,-1,   1, 1,-1,   1, 1, 1,
         1,-1,-1,   1, 1, 1,   1,-1, 1,
        // Top
        -1, 1,-1,  -1, 1, 1,   1, 1, 1,
        -1, 1,-1,   1, 1, 1,   1, 1,-1,
        // Bottom
        -1,-1,-1,   1,-1,-1,   1,-1, 1,
        -1,-1,-1,   1,-1, 1,  -1,-1, 1
    };

    Mesh::Mesh(QOpenGLFunctions_4_5_Core* gl)
        :
        mGL(gl)
    {}

    Mesh::~Mesh() {
        destroy();
    }

    void Mesh::create() {
        mVertexCount = sizeof(cubeVertices) / (3 * sizeof(float));

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

        mGL->glEnableVertexAttribArray(0);
        mGL->glVertexAttribPointer(
            0, 3, GL_FLOAT, GL_FALSE,
            3 * sizeof(float),
            nullptr
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