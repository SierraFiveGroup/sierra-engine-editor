// main_window.cpp
// Licensed under LGPLv2

#include <sierra/ui/window/main_window.hpp>
#include <sierra/logger.hpp>
#include <sierra/io/io_utils.hpp>

namespace SierraEditor::UI {
    MainWindow::MainWindow(QWidget* parent)
        : QMainWindow(parent)
    {
        mCurrentProject = std::make_shared<Project::SProject>(""); // Empty path for now
        this->setWindowTitle("Sierra Engine Editor");
        mSetupMenus();

        // The viewport is the central widget
        mViewport = new RenderViewport(this, "No Project Loaded!");
        setCentralWidget(mViewport);

        mSetupDockPanels();

        resize(1600, 900);
    }

    void MainWindow::mSetupMenus() {
        // Actions
        QAction* openProjectAction = new QAction("Open Project", this);
        openProjectAction->setShortcut(QKeySequence(SUPER_KEY | Qt::Key_O));
        connect(openProjectAction, &QAction::triggered, this, [this]() {
            // Popup file dialog to open project
            QString dir = QFileDialog::getOpenFileName(this, tr("Select Project File"), QDir::homePath(), "All Files (*);;Sierra Project Files (*.sierra)", nullptr, QFileDialog::ReadOnly | QFileDialog::DontResolveSymlinks | QFileDialog::DontUseNativeDialog);
            if (!dir.isEmpty()) {
                if (!mCurrentProject->overrideFilePath(dir.toStdString())) {
                    TODO("Project is already loaded; cannot override file path; ADD SUPPORT FOR SWITCHING PROJECTS");
                    return; // TODO: Save current project and load new one
                }

                if (mCurrentProject->load()) {
                    LOG("Loaded project: " << mCurrentProject->getName() << " (Version " << mCurrentProject->getVersion() << ")");
                    this->setWindowTitle(QString::fromStdString("Sierra Engine Editor - " + mCurrentProject->getName()));
                    mViewport->setRenderMessage("No Scene Loaded!");
                    mGenericLeft->addNewTab(new HierarchyPanel(), "Hierarchy");
                    mGenericRight->addNewTab(new InspectorPanel(), "Inspector");
                    mGenericBottom->addNewTab(new AssetBrowser(), "Asset Browser");
                } else {
                    ERROR("Failed to load project at: " << dir.toStdString());
                }
            }
        });

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
        auto* left = new QDockWidget(nullptr, this);
        mGenericLeft = new GenericPanel();
        left->setWidget(mGenericLeft);
        addDockWidget(Qt::LeftDockWidgetArea, left);

        // Inspector
        auto* right = new QDockWidget(nullptr, this);
        mGenericRight = new GenericPanel();
        right->setWidget(mGenericRight);
        addDockWidget(Qt::RightDockWidgetArea, right);

        // Asset Browser
        auto* bottom = new QDockWidget(nullptr, this);
        mGenericBottom = new GenericPanel();
        bottom->setWidget(mGenericBottom);
        addDockWidget(Qt::BottomDockWidgetArea, bottom);
    }
}