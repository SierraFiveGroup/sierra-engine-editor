// asset_browser.cpp
// Licensed under LGPLv2

#include <sierra/ui/widgets/asset_browser.hpp>

namespace SierraEditor::UI {
    AssetBrowser::AssetBrowser(QWidget* parent)
        : QWidget(parent)
    {
        mTree = new QTreeWidget(this);
        mTree->setHeaderLabel("Assets");

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