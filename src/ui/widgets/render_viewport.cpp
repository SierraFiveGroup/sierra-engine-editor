// render_viewport.cpp
// Licensed under LGPLv2.1

#include <sierra/ui/widgets/render_viewport.hpp>

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

    // ---- BlueScreenGL ----

    BlueScreenGL::BlueScreenGL(QWidget* parent, std::string* rendermsgPtr)
        : QOpenGLWidget(parent), mRenderMsgPtr(rendermsgPtr)
    {}

    void BlueScreenGL::initializeGL() {
        initializeOpenGLFunctions();
        glClearColor(0.1f, 0.3f, 0.8f, 1.0f); // blue background
    }

    void BlueScreenGL::paintGL() {
        // Orthographic projection
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(0, width(), height(), 0, -1, 1);

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        float scale = 2.0f; // x2 size
        glScalef(scale, scale, 1.0f);

        // Text stays at (x, y) visually
        float x = 20.0f / scale;
        float y = 20.0f / scale;

        // Text and color
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);

        char* text = "Undefined Message"; // This goes against C++ ISO. Whoops.
        if (mRenderMsgPtr) {
            text = const_cast<char*>((*mRenderMsgPtr).c_str());
        }
        static char buffer[99999];
        int num_quads = stb_easy_font_print(x, y, const_cast<char*>(text), NULL, buffer, sizeof(buffer));

        glColor3f(1.f, 0.f, 0.f); // Red
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(2, GL_FLOAT, 16, buffer);
        glDrawArrays(GL_QUADS, 0, num_quads * 4);
        glDisableClientState(GL_VERTEX_ARRAY);

        glEnable(GL_DEPTH_TEST);

        // Restore matrices
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();

        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
    }

    void BlueScreenGL::resizeGL(int w, int h) {
        glViewport(0, 0, w, h);
    }
}