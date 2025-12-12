// main_window.cpp
// Licensed under LGPLv2

#include <sierra/ui/window/main_window.hpp>

namespace SierraEditor::UI {
    MainWindow::MainWindow(QWidget* parent)
        : QMainWindow(parent)
    {
        mSetupMenus();

        // The viewport is the central widget
        mViewport = new RenderViewport(this);
        setCentralWidget(mViewport);

        mSetupDockPanels();

        resize(1600, 900);
    }

    void MainWindow::mSetupMenus() {
        QMenu* fileMenu = menuBar()->addMenu("File");
        fileMenu->addAction("New Project");
        fileMenu->addAction("Open...");
        fileMenu->addSeparator();
        fileMenu->addAction("Exit", this, SLOT(close()));
    }

    void MainWindow::mSetupDockPanels() {
        // Hierarchy
        auto* hierarchyDock = new QDockWidget("Hierarchy", this);
        mHierarchy = new HierarchyPanel();
        hierarchyDock->setWidget(mHierarchy);
        addDockWidget(Qt::LeftDockWidgetArea, hierarchyDock);

        // Inspector
        auto* inspectorDock = new QDockWidget("Inspector", this);
        mInspector = new InspectorPanel();
        inspectorDock->setWidget(mInspector);
        addDockWidget(Qt::RightDockWidgetArea, inspectorDock);

        // Asset Browser
        auto* assetBrowserDock = new QDockWidget("Asset Browser", this);
        mAssetBrowser = new AssetBrowser();
        assetBrowserDock->setWidget(mAssetBrowser);
        addDockWidget(Qt::BottomDockWidgetArea, assetBrowserDock);
    }
}