// hierarchy_panel.hpp
// Licensed under LGPLv2.1

#pragma once

#include <QWidget>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <memory>
#include <sierra/project/sscene.hpp>
#include <sierra/stateful/selection_manager.hpp>
#include <sierra/ui/widgets/refreshable_panel.hpp>

namespace SierraEditor::UI {
    class HierarchyPanel : public QWidget, public RefreshablePanel {
        Q_OBJECT
        public:
            explicit HierarchyPanel(std::shared_ptr<Project::SScene>* currentSceneRef, QWidget* parent = nullptr);

            void setCurrentScene(std::shared_ptr<Project::SScene>* scene) {
                mCurrentSceneRef = scene;
                mLastScene = mCurrentSceneRef ? *mCurrentSceneRef : nullptr;
                mPopulateHierarchy();
            }

            void refresh() override {
                mPopulateHierarchy();
            }

            QTreeWidgetItem* getRootItem() const {
                return mRootItem;
            }

            QTreeWidgetItem* getItemAtCursor() const {
                QPoint cursorPos = mTree->viewport()->mapFromGlobal(QCursor::pos());
                return mTree->itemAt(cursorPos);
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