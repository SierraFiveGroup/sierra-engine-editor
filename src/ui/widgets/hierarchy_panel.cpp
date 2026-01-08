// hierarchy_panel.cpp
// Licensed under LGPLv2.1

#include <sierra/ui/widgets/hierarchy_panel.hpp>
#include <sierra/ui/widgets/generic_panel.hpp>
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
        // Disconnect all previous connections to avoid dangling pointers
        disconnect(mTree, &QTreeWidget::itemClicked, nullptr, nullptr);
        
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
            auto* entityItem = new QTreeWidgetItem(mRootItem, QStringList() << entityName);
            
            // Store entity pointer in item data for later retrieval
            entityItem->setData(0, Qt::UserRole, QVariant::fromValue(static_cast<void*>(entity.get())));
        }

        // Single connection for all items
        QObject::connect(mTree, &QTreeWidget::itemClicked, this, [this](QTreeWidgetItem* item, int column) {
            if (!item) return;
            
            // Retrieve entity pointer from item data
            void* entityPtr = item->data(0, Qt::UserRole).value<void*>(); // TODO: push to selection manager correctly
            if (entityPtr) {
                Stateful::SelectionManager::getInstance().setSelectedEntity(static_cast<Project::SEntity*>(entityPtr));
                
                // Trigger main window to refresh (via parent); TODO: Move to refreshable panel system as a callable, universal method
                auto* parentGeneric = qobject_cast<GenericPanel*>(item->treeWidget()->parentWidget()->parentWidget()->parentWidget()->parentWidget()); // Took some attempts; this code is weird, I should fix this eventually
                //DBG(parentGeneric);
                if (parentGeneric) {
                    mBlockNextRefresh = true;
                    //DBG("Exists!");
                    parentGeneric->sendSignalToRefreshMainWindow();
                }
            }
        });

        mTree->expandAll();
    }
}