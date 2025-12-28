// hierarchy_panel.cpp
// Licensed under LGPLv2.1

#include <sierra/ui/widgets/hierarchy_panel.hpp>
#include <QTimer>

namespace SierraEditor::UI {
    HierarchyPanel::HierarchyPanel(std::shared_ptr<Project::SScene>* currentScene, QWidget* parent)
        : QWidget(parent), mCurrentSceneRef(currentScene)
    {
        mTree = new QTreeWidget(this);

        // Initialize last scene for change detection
        if (mCurrentSceneRef) {
            mLastScene = *mCurrentSceneRef;
        }

        // Placeholders
        mPopulateHierarchy();
        //auto* root = new QTreeWidgetItem(mTree, QStringList() << "Root");
        //new QTreeWidgetItem(root, QStringList() << "Camera");
        //new QTreeWidgetItem(root, QStringList() << "GameObject");
        //new QTreeWidgetItem(root, QStringList() << "GameObject");

        mTree->expandAll();

        QVBoxLayout* layout = new QVBoxLayout(this);
        layout->addWidget(mTree);
        layout->setContentsMargins(0, 0, 0, 0);
        setLayout(layout);

        // Set up timer to check for scene updates
        QTimer* updateTimer = new QTimer(this);
        connect(updateTimer, &QTimer::timeout, this, &HierarchyPanel::mCheckSceneUpdate);
        updateTimer->start(100);  // Check every 100ms
    }

    void HierarchyPanel::mCheckSceneUpdate() {
        if (!mCurrentSceneRef) {
            return;
        }

        // Check if the scene pointer has changed
        std::shared_ptr<Project::SScene> currentScene = *mCurrentSceneRef;
        if (currentScene != mLastScene) {
            mLastScene = currentScene;
            mPopulateHierarchy();
        }
    }

    void HierarchyPanel::mPopulateHierarchy() {
        mTree->clear();

        if (!mCurrentSceneRef) {
            ERROR("HierarchyPanel: (You did something very wrong!) CurrentSceneRef is nullptr!");
        }

        if (!*mCurrentSceneRef) {
            mTree->setHeaderLabel("<No Scene Loaded>");
            mRootItem = new QTreeWidgetItem(mTree, QStringList() << "<No Scene Loaded>");
            return;
        }

        std::string sceneName = (*mCurrentSceneRef)->getName();
        mTree->setHeaderLabel(QString::fromStdString(sceneName));
        mRootItem = new QTreeWidgetItem(mTree, QStringList() << "Scene Root");

        // Populate entities
        for (const auto& entity : (*mCurrentSceneRef)->getEntities()) {
            QString entityName = QString::fromStdString(entity->getName());
            new QTreeWidgetItem(mRootItem, QStringList() << entityName);
        }

        mTree->expandAll();
    }
}