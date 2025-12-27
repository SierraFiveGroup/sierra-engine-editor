// asset_browser.cpp
// Licensed under LGPLv2.1

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

    void AssetBrowser::refresh() {
        mFileSystemModel->setRootPath(mFileSystemModel->rootPath()); // Refresh by resetting the root path; TODO: Better way?
    }

    void AssetBrowser::createScene(const std::string& name) {
        // Create a file in the scenes directory
        QString currentPath = mFileSystemModel->filePath(mTree->rootIndex()) + "/scenes";
        QString scenePath = QDir(currentPath).filePath(QString::fromStdString(name + ".scene"));
        QFile sceneFile(scenePath);
        if (sceneFile.open(QIODevice::WriteOnly)) {
            std::string baseFormat = // TODO: Discuss
                "<Scene>\n"
                "    <Version>1</Version>\n"
                "    <Name>" + name + "</Name>\n"
                "    <Entities>\n" // Might be renamed later
                "    </Entities>\n"
                "</Scene>\n"
            ;
            sceneFile.write(baseFormat.c_str(), static_cast<qint64>(baseFormat.size()));
            sceneFile.close();
            refresh();
        }
    }

    void AssetBrowser::createDirectory(const std::string& name, const std::string& parentPath) {
        QString dirPath = QDir(QString::fromStdString(parentPath)).filePath(QString::fromStdString(name));
        QDir dir;
        if (dir.mkdir(dirPath)) {
            refresh();
        }
    }
}