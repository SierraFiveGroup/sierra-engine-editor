// render_viewport.cpp
// Licensed under LGPLv2.1

#include <sierra/ui/widgets/render_viewport.hpp>
#include <QOpenGLShader>
#include <QString>

namespace SierraEditor::UI {
    RenderViewport::RenderViewport(QWidget* parent, std::string rendermsg)
        : QWidget(parent), mRenderMsg(rendermsg)
    {
        mTabs = new QTabWidget(this);

        // Create two OpenGL panels for Scene and Game views
        mSceneView = new BlueScreenGL(nullptr, &mRenderMsg); // Default to blue screen
        auto* gameView  = new BlueScreenGL(nullptr, &mRenderMsg);

        mTabs->addTab(mSceneView, "Scene");
        mTabs->addTab(gameView, "Game");

        QVBoxLayout* layout = new QVBoxLayout(this);
        layout->addWidget(mTabs);
        layout->setContentsMargins(0, 0, 0, 0);
        setLayout(layout);
    }

    void RenderViewport::switchToSceneView() {
        if (mSceneView) {
            mSceneView->deleteLater();
        }

        mSceneView = new SierraEditor::Viewport::GL::ViewportGL(nullptr);

        // Swap tab
        mTabs->removeTab(0);
        mTabs->insertTab(0, mSceneView, "Scene");
        mTabs->setCurrentIndex(0);
    }

    void RenderViewport::toggleGridView() {
        auto* viewportGL = dynamic_cast<SierraEditor::Viewport::GL::ViewportGL*>(mSceneView);
        if (viewportGL) {
            viewportGL->setRenderCoordinateSystemEnabled(!viewportGL->getRenderCoordinateSystemEnabled());
        }
    }

    // ---- BlueScreenGL ----

    BlueScreenGL::BlueScreenGL(QWidget* parent, std::string* rendermsgPtr)
        : QOpenGLWidget(parent), mRenderMsgPtr(rendermsgPtr)
    {}

    void BlueScreenGL::initializeGL() {
        initializeOpenGLFunctions();
        glClearColor(0.1f, 0.3f, 0.8f, 1.0f); // blue background

        const QString vertPath = ":/shaders/basic.vert";
        const QString fragPath = ":/shaders/basic.frag";
        bool okV = mShader.addShaderFromSourceFile(QOpenGLShader::Vertex, vertPath);
        bool okF = mShader.addShaderFromSourceFile(QOpenGLShader::Fragment, fragPath);
        bool okL = mShader.link();
        bool ok = okV && okF && okL;
        if (!ok) {
            WARN("BlueScreenGL: shader setup failed: " << mShader.log().toStdString());
        }
    }

    void BlueScreenGL::paintGL() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);

        // Orthographic projection using shader
        mProjection.setToIdentity();
        mProjection.ortho(0, width(), height(), 0, -1, 1);

        float scale = 2.0f; // x2 size
        float x = 20.0f / scale;
        float y = 20.0f / scale;

        char* text = (char*)"Undefined Message";
        if (mRenderMsgPtr) {
            text = const_cast<char*>((*mRenderMsgPtr).c_str());
        }
        static char buffer[99999];
        int num_quads = stb_easy_font_print(x, y, text, NULL, buffer, sizeof(buffer));

        if (num_quads > 0) {
            // Prepare vertices as triangles
            std::vector<float> textVertices;
            textVertices.reserve(num_quads * 6 * 6);

            float* stb_data = (float*)buffer;
            for (int q = 0; q < num_quads; ++q) {
                float* v0 = &stb_data[(q * 4 + 0) * 4];
                float* v1 = &stb_data[(q * 4 + 1) * 4];
                float* v2 = &stb_data[(q * 4 + 2) * 4];
                float* v3 = &stb_data[(q * 4 + 3) * 4];

                auto pushVertex = [&](float vx, float vy) {
                    textVertices.push_back(vx);
                    textVertices.push_back(vy);
                    textVertices.push_back(0.0f);
                    textVertices.push_back(1.0f);
                    textVertices.push_back(0.0f);
                    textVertices.push_back(0.0f); // red text
                };

                // Tri 1
                pushVertex(v0[0], v0[1]);
                pushVertex(v1[0], v1[1]);
                pushVertex(v2[0], v2[1]);
                // Tri 2
                pushVertex(v0[0], v0[1]);
                pushVertex(v2[0], v2[1]);
                pushVertex(v3[0], v3[1]);
            }

            // Model scale for text
            QMatrix4x4 model;
            model.scale(scale, scale, 1.0f);

            mShader.bind();
            mShader.setUniformValue("uProj", mProjection);
            mShader.setUniformValue("uView", QMatrix4x4());
            mShader.setUniformValue("uModel", model);

            GLuint vao, vbo;
            glGenVertexArrays(1, &vao);
            glGenBuffers(1, &vbo);

            glBindVertexArray(vao);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, textVertices.size() * sizeof(float), textVertices.data(), GL_DYNAMIC_DRAW);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

            glDrawArrays(GL_TRIANGLES, 0, num_quads * 6);

            glBindVertexArray(0);
            glDeleteBuffers(1, &vbo);
            glDeleteVertexArrays(1, &vao);

            mShader.release();
        }

        glEnable(GL_DEPTH_TEST);
    }

    void BlueScreenGL::resizeGL(int w, int h) {
        glViewport(0, 0, w, h);
    }
}