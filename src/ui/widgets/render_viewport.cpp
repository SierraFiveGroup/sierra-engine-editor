// render_viewport.cpp
// Licensed under LGPLv2

#include <sierra/ui/widgets/render_viewport.hpp>

namespace SierraEditor::UI {
    RenderViewport::RenderViewport(QWidget* parent)
        : QWidget(parent)
    {
        mTabs = new QTabWidget(this);

        // Create two OpenGL panels for Scene and Game views
        auto* sceneView = new BlueScreenGL();
        auto* gameView  = new BlueScreenGL();

        mTabs->addTab(sceneView, "Scene");
        mTabs->addTab(gameView, "Game");

        QVBoxLayout* layout = new QVBoxLayout(this);
        layout->addWidget(mTabs);
        layout->setContentsMargins(0, 0, 0, 0);
        setLayout(layout);
    }

    // ---- BlueScreenGL ----

    BlueScreenGL::BlueScreenGL(QWidget* parent)
        : QOpenGLWidget(parent)
    {}

    void BlueScreenGL::initializeGL() {
        initializeOpenGLFunctions();
        glClearColor(0.1f, 0.3f, 0.8f, 1.0f); // blue background
    }

    void BlueScreenGL::paintGL() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void BlueScreenGL::resizeGL(int w, int h) {
        glViewport(0, 0, w, h);
    }
}