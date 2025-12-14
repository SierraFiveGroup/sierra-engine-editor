// asset_browser.hpp
// Licensed under LGPLv2

#pragma once

#include <QWidget>
#include <QTreeView>
#include <QFileSystemModel>
#include <QVBoxLayout>

namespace SierraEditor::UI {
    class AssetBrowser : public QWidget {
        Q_OBJECT
        public:
            explicit AssetBrowser(QWidget* parent = nullptr);
    
        private:
            QTreeView* mTree;
            QFileSystemModel* mFileSystemModel;
    
    };
}