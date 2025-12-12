// main_window.cpp
// Licensed under LGPLv2

#include <sierra/ui/window/main_window.hpp>

namespace SierraEditor::UI {
    MainWindow::MainWindow(QWidget* parent)
        : QMainWindow(parent)
    {
        this->setWindowTitle("Sierra Engine Editor");
        mSetupMenus();

        // The viewport is the central widget
        mViewport = new RenderViewport(this);
        setCentralWidget(mViewport);

        mSetupDockPanels();

        resize(1600, 900);
    }

    void MainWindow::mSetupMenus() {
        // Actions
        QAction* openProjectAction = new QAction("Open Project", this);

        QAction* saveAllAction = new QAction("Save All", this);
        //saveAllAction->setShortcut(QKeySequence(std::string(SUPER_KEY) + "+S"));

        QAction* exitAction = new QAction("Exit", this);
        //exitAction->setShortcut(QKeySequence(std::string(SUPER_KEY) + "+Q"));
        connect(exitAction, &QAction::triggered, this, &MainWindow::close);

        QAction* spawnHierarchyAction = new QAction("Hierarchy", this);
        /*connect(spawnHierarchyAction, &QAction::triggered, this, []() {
        });*/

        QAction* spawnInspectorAction = new QAction("Inspector", this);
        //connect(spawnInspectorAction, &QAction::triggered, this, []() {
        //});

        QAction* spawnAssetBrowserAction = new QAction("Asset Browser", this);
        //connect(spawnAssetBrowserAction, &QAction::triggered, this, []() {
        //});

        // Menus
        QMenu* fileMenu = menuBar()->addMenu("File");
        fileMenu->addAction(openProjectAction);
        fileMenu->addSeparator();
        fileMenu->addAction(saveAllAction);
        fileMenu->addSeparator();
        fileMenu->addAction(exitAction);

        QMenu* editMenu = menuBar()->addMenu("Edit");
        editMenu->addAction("Undo");
        editMenu->addAction("Redo");
        editMenu->addSeparator();
        editMenu->addAction("Cut");
        editMenu->addAction("Copy");
        editMenu->addAction("Paste");

        QMenu* windowMenu = menuBar()->addMenu("Window");
        windowMenu->addAction(spawnHierarchyAction);
        windowMenu->addAction(spawnInspectorAction);
        windowMenu->addAction(spawnAssetBrowserAction);
    }

    void MainWindow::mSetupDockPanels() {
        // Hierarchy
        auto* left = new QDockWidget("Hierarchy", this);
        mGenericLeft = new GenericPanel();
        //mGenericLeft->addNewTab(new HierarchyPanel(), "Hierarchy");
        left->setWidget(mGenericLeft);
        addDockWidget(Qt::LeftDockWidgetArea, left);

        // Inspector
        auto* right = new QDockWidget("Inspector", this);
        mGenericRight = new GenericPanel();
        //mGenericRight->addNewTab(new InspectorPanel(), "Inspector");
        right->setWidget(mGenericRight);
        addDockWidget(Qt::RightDockWidgetArea, right);

        // Asset Browser
        auto* bottom = new QDockWidget("Asset Browser", this);
        mGenericBottom = new GenericPanel();
        //mGenericBottom->addNewTab(new AssetBrowser(), "Asset Browser");
        bottom->setWidget(mGenericBottom);
        addDockWidget(Qt::BottomDockWidgetArea, bottom);
    }
}