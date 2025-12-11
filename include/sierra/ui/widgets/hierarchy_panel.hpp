// hierarchy_panel.hpp
// Licensed under LGPLv2

#pragma once

#include <QWidget>
#include <QTreeWidget>
#include <QVBoxLayout>

namespace SierraEditor::UI {
    class HierarchyPanel : public QWidget {
        Q_OBJECT
        public:
            explicit HierarchyPanel(QWidget* parent = nullptr);
    
        private:
            QTreeWidget* mTree;
    
    };
}