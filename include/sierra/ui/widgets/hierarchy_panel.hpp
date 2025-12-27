// hierarchy_panel.hpp
// Licensed under LGPLv2.1

#pragma once

#include <QWidget>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <memory>
#include <sierra/project/sscene.hpp>

namespace SierraEditor::UI {
    class HierarchyPanel : public QWidget {
        Q_OBJECT
        public:
            explicit HierarchyPanel(std::shared_ptr<Project::SScene>* currentSceneRef, QWidget* parent = nullptr);

            void setCurrentScene(std::shared_ptr<Project::SScene>* scene) {
                mCurrentSceneRef = scene;
                mLastScene = mCurrentSceneRef ? *mCurrentSceneRef : nullptr;
                mPopulateHierarchy();
            }
    
        private slots:
            void mCheckSceneUpdate();

        private:
            void mPopulateHierarchy();
            
            QTreeWidget* mTree;
            QTreeWidgetItem* mRootItem;
            std::shared_ptr<Project::SScene>* mCurrentSceneRef = nullptr;
            std::shared_ptr<Project::SScene> mLastScene = nullptr;
    
    };
}