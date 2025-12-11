// hierarchy_panel.cpp
// Licensed under LGPLv2

#pragma once

#include <sierra/ui/widgets/hierarchy_panel.hpp>

namespace SierraEditor::UI {
    HierarchyPanel::HierarchyPanel(QWidget* parent)
        : QWidget(parent)
    {
        mTree = new QTreeWidget(this);
        mTree->setHeaderLabel("Scene Objects");

        // Placeholders
        auto* root = new QTreeWidgetItem(mTree, QStringList() << "Root");
        new QTreeWidgetItem(root, QStringList() << "Camera");
        new QTreeWidgetItem(root, QStringList() << "GameObject");
        new QTreeWidgetItem(root, QStringList() << "GameObject");

        mTree->expandAll();

        QVBoxLayout* layout = new QVBoxLayout(this);
        layout->addWidget(mTree);
        layout->setContentsMargins(0, 0, 0, 0);
        setLayout(layout);
    }
}