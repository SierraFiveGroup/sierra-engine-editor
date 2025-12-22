// mesh.hpp
// Licensed under LGPLv2.1

#pragma once

#include <QOpenGLFunctions_4_1_Core>

namespace SierraEditor::Viewport::GL {
    class Mesh {
        public:
            explicit Mesh(QOpenGLFunctions_4_1_Core* gl);
            ~Mesh();

            void create();
            void destroy();
            void draw() const;

        private:
            QOpenGLFunctions_4_1_Core* mGL = nullptr;

            GLuint mVAO = 0;
            GLuint mVBO = 0;
            GLuint mEBO = 0;
            int mVertexCount = 0;
    };
}