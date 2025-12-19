// asset_browser.hpp
// Licensed under LGPLv2.1

#pragma once

#include <QWidget>
#include <QTreeView>
#include <QFileSystemModel>
#include <QVBoxLayout>
#include <QModelIndex>
#include <QString>

namespace SierraEditor::UI {
    class AssetBrowser : public QWidget {
        Q_OBJECT
        public:
            explicit AssetBrowser(QWidget* parent = nullptr);

            void setDirectory(const QString& path);

        private:
            QTreeView* mTree;
            QFileSystemModel* mFileSystemModel;
    
    };
}