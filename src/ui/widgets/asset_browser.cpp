// asset_browser.cpp
// Licensed under LGPLv2

#include <sierra/ui/widgets/asset_browser.hpp>
#include <QDir>

namespace SierraEditor::UI {
    AssetBrowser::AssetBrowser(QWidget* parent)
        : QWidget(parent)
    {
        mFileSystemModel = new QFileSystemModel(this);
        
        // Set root path to user's home directory
        QString homePath = QDir::homePath();
        mFileSystemModel->setRootPath(homePath);

        mTree = new QTreeView(this);
        mTree->setModel(mFileSystemModel);
        
        // Set initial root index to home directory
        mTree->setRootIndex(mFileSystemModel->index(homePath));

        // Hide columns except name
        mTree->hideColumn(1); // Size
        mTree->hideColumn(2); // Type
        mTree->hideColumn(3); // Date modified

        QVBoxLayout* layout = new QVBoxLayout(this);
        layout->addWidget(mTree);
        layout->setContentsMargins(0, 0, 0, 0);
        setLayout(layout);
    }

    void AssetBrowser::setDirectory(const QString& path) {
        QModelIndex index = mFileSystemModel->index(path);
        if (index.isValid()) {
            mTree->setRootIndex(index);
        }
    }
}