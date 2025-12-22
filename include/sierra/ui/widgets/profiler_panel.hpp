// profiler_panel.hpp
// Licensed under LGPLv2.1

#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>

namespace SierraEditor::UI {
    class ProfilerPanel : public QWidget {
        Q_OBJECT
        public:
            explicit ProfilerPanel(QWidget* parent = nullptr);
        
        private:
            QVBoxLayout* mLayout;
    };
}