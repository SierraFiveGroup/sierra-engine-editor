// viewport_gl.cpp
// Licensed under LGPLv2.1

#include <sierra/viewport/gl/viewport_gl.hpp>
#include <QOpenGLShader>
#include <sierra/utils.hpp>

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

        mMesh = std::make_unique<GL::Mesh>(static_cast<QOpenGLFunctions_4_1_Core*>(this));
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

        // Render coordinate system grid if enabled
        if (mRenderCoords) {
            mRenderCoordinateSystem();
        }

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
            // Convert stb_easy_font quads (x, y, u, v per vertex) to triangles (core profile)
            std::vector<float> textVertices;
            textVertices.reserve(num_quads * 6 * 6); // 6 vertices per quad, 6 floats per vertex

            float* stb_data = (float*)stb_buffer;
            for (int q = 0; q < num_quads; ++q) {
                float* v0 = &stb_data[(q * 4 + 0) * 4];
                float* v1 = &stb_data[(q * 4 + 1) * 4];
                float* v2 = &stb_data[(q * 4 + 2) * 4];
                float* v3 = &stb_data[(q * 4 + 3) * 4];

                auto pushVertex = [&](float x, float y) {
                    textVertices.push_back(x);
                    textVertices.push_back(y);
                    textVertices.push_back(0.0f);
                    textVertices.push_back(1.0f);
                    textVertices.push_back(1.0f);
                    textVertices.push_back(1.0f);
                };

                // Triangle 1: v0, v1, v2
                pushVertex(v0[0], v0[1]);
                pushVertex(v1[0], v1[1]);
                pushVertex(v2[0], v2[1]);
                // Triangle 2: v0, v2, v3
                pushVertex(v0[0], v0[1]);
                pushVertex(v2[0], v2[1]);
                pushVertex(v3[0], v3[1]);
            }

            GLuint textVAO, textVBO;
            glGenVertexArrays(1, &textVAO);
            glGenBuffers(1, &textVBO);

            glBindVertexArray(textVAO);
            glBindBuffer(GL_ARRAY_BUFFER, textVBO);
            glBufferData(GL_ARRAY_BUFFER, textVertices.size() * sizeof(float), textVertices.data(), GL_DYNAMIC_DRAW);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

            glDrawArrays(GL_TRIANGLES, 0, num_quads * 6);

            glBindVertexArray(0);
            glDeleteBuffers(1, &textVBO);
            glDeleteVertexArrays(1, &textVAO);
        }

        mShader.release();
        
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
    }

    void ViewportGL::mRenderCoordinateSystem() {
        // Grid parameters - large grid for "infinite" appearance
        const int gridSize = 1000;  // 1000x1000 grid; TODO: Dynamic and not render too far away (frame drops)
        const float gridSpacing = 1.0f;
        const float halfGrid = gridSize * gridSpacing * 0.5f;

        // Prepare grid vertices with colors (position + color)
        std::vector<float> gridVertices;
        
        // Light gray color for grid lines
        const float grayR = 0.4f, grayG = 0.4f, grayB = 0.4f;
        
        // Lines parallel to X-axis (running along X direction)
        for (int i = 0; i <= gridSize; ++i) {
            float z = -halfGrid + i * gridSpacing;
            bool isCenterZ = (i == gridSize / 2);
            
            if (!isCenterZ) {
                // Regular grid line in light gray
                gridVertices.insert(gridVertices.end(), {
                    -halfGrid, 0.0f, z, grayR, grayG, grayB,  // Start point + color
                     halfGrid, 0.0f, z, grayR, grayG, grayB   // End point + color
                });
            }
        }
        
        // Lines parallel to Z-axis (running along Z direction)
        for (int i = 0; i <= gridSize; ++i) {
            float x = -halfGrid + i * gridSpacing;
            bool isCenterX = (i == gridSize / 2);
            
            if (!isCenterX) {
                // Regular grid line in light gray
                gridVertices.insert(gridVertices.end(), {
                    x, 0.0f, -halfGrid, grayR, grayG, grayB,  // Start point + color
                    x, 0.0f,  halfGrid, grayR, grayG, grayB   // End point + color
                });
            }
        }

        // Add central X-axis (red)
        gridVertices.insert(gridVertices.end(), {
            -halfGrid, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // Start point + red
             halfGrid, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f   // End point + red
        });
        
        // Add central Z-axis (blue)
        glm::vec3 blueColor = SierraEditor::rgbtofloats(glm::vec3(37, 150, 190));
        gridVertices.insert(gridVertices.end(), {
            0.0f, 0.0f, -halfGrid, blueColor.r, blueColor.g, blueColor.b,  // Start point + blue
            0.0f, 0.0f,  halfGrid, blueColor.r, blueColor.g, blueColor.b   // End point + blue
        });

        // Create temporary VAO and VBO for grid
        GLuint gridVAO, gridVBO;
        glGenVertexArrays(1, &gridVAO);
        glGenBuffers(1, &gridVBO);
        
        glBindVertexArray(gridVAO);
        glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
        glBufferData(GL_ARRAY_BUFFER, 
                     gridVertices.size() * sizeof(float),
                     gridVertices.data(),
                     GL_STATIC_DRAW);
        
        // Position attribute
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        
        // Color attribute
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

        // Set up shader for grid rendering
        mShader.bind();
        mShader.setUniformValue("uProj", mProjection);
        mShader.setUniformValue("uView", mCamera.view());
        mShader.setUniformValue("uModel", QMatrix4x4());
        
        // Draw grid lines
        glLineWidth(1.0f);
        glDrawArrays(GL_LINES, 0, gridVertices.size() / 6);
        
        mShader.release();
        
        // Cleanup
        glBindVertexArray(0);
        glDeleteBuffers(1, &gridVBO);
        glDeleteVertexArrays(1, &gridVAO);
    }
}