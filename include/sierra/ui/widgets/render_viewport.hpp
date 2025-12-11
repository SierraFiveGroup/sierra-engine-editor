// render_viewport.hpp
// Licensed under LGPLv2

#pragma once

#include <QWidget>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>

namespace SierraEditor::UI {
    class RenderViewport : public QWidget {
        Q_OBJECT
        public:
            explicit RenderViewport(QWidget* parent = nullptr);
        
        private:
            QTabWidget* mTabs;
    };
    
    class BlueScreenGL : public QOpenGLWidget, protected QOpenGLFunctions {
        Q_OBJECT
        public:
            explicit BlueScreenGL(QWidget* parent = nullptr);
        
        protected:
            void initializeGL() override;
            void paintGL() override;
            void resizeGL(int w, int h) override;
    };    
}