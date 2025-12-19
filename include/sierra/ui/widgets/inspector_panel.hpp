// inspector_panel.hpp
// Licensed under LGPLv2.1

#pragma once

#include <QWidget>
#include <QListWidget>
#include <QVBoxLayout>

namespace SierraEditor::UI {
    class InspectorPanel : public QWidget {
        Q_OBJECT
        public:
            explicit InspectorPanel(QWidget* parent = nullptr);

        private:
            QListWidget* mList;
    };
}