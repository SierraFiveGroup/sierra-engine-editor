// main_window.cpp
// Licensed under LGPLv2.1

#include <sierra/ui/window/main_window.hpp>
#include <sierra/logger.hpp>
#include <sierra/io/io_utils.hpp>

namespace SierraEditor::UI {
    MainWindow::MainWindow(const char* projectPath, QWidget* parent)
        : QMainWindow(parent)
    {
        mCurrentProject = std::make_shared<Project::SProject>(projectPath ? std::string(projectPath) : "");
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
        openProjectAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_O)); // Auto-adjusts to CMD on Mac
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
                    LOG("Project path: " << mCurrentProject->getFilePath());

                    this->setWindowTitle(QString::fromStdString("Sierra Engine Editor - " + mCurrentProject->getName()));
                    mViewport->setRenderMessage("No Scene Loaded!");

                    mGenericLeft->addNewTab(new HierarchyPanel(), "Hierarchy");
                    mGenericRight->addNewTab(new InspectorPanel(), "Inspector");
                    auto* assetBrowser = new AssetBrowser();
                    // Remove the last part of the file path to get the project directory
                    assetBrowser->setDirectory(QString::fromStdString(IO::stripLastPathComponent(mCurrentProject->getFilePath())));
                    mGenericBottom->addNewTab(assetBrowser, "Asset Browser");

                    mViewport->switchToSceneView();
                } else {
                    ERROR("Failed to load project at: " << dir.toStdString());
                }
            }
        });

        QAction* saveAllAction = new QAction("Save All", this);
        saveAllAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_S));
        connect(saveAllAction, &QAction::triggered, this, [this]() {
            if (mCurrentProject->getVersion() != 0) {
                if (mCurrentProject->save()) {
                    LOG("Project saved: " << mCurrentProject->getName());
                } else {
                    ERROR("Failed to save project: " << mCurrentProject->getName());
                }
            } else {
                WARN("No project loaded; cannot save.");
            }
        });

        QAction* exitAction = new QAction("Exit", this);
        //exitAction->setShortcut(QKeySequence(std::string(SUPER_KEY) + "+Q"));
        connect(exitAction, &QAction::triggered, this, &MainWindow::close);

        QAction* spawnHierarchyAction = new QAction("Hierarchy", this);
        spawnHierarchyAction->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_H));
        // Connect to spawn new Hierarchy panel
        connect(spawnHierarchyAction, &QAction::triggered, this, [this]() {
            mSpawnGenericPanelWithWidget("Hierarchy", new HierarchyPanel());
        });

        QAction* spawnInspectorAction = new QAction("Inspector", this);
        spawnInspectorAction->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_I));
        connect(spawnInspectorAction, &QAction::triggered, this, [this]() {
            mSpawnGenericPanelWithWidget("Inspector", new InspectorPanel());
        });

        QAction* spawnAssetBrowserAction = new QAction("Asset Browser", this);
        spawnAssetBrowserAction->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_A));
        connect(spawnAssetBrowserAction, &QAction::triggered, this, [this]() {
            mSpawnGenericPanelWithWidget("Asset Browser", new AssetBrowser());
        });

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
        mActiveGenerics["Hierarchy"] = mGenericLeft;
        addDockWidget(Qt::LeftDockWidgetArea, left);

        // Inspector
        auto* right = new QDockWidget(nullptr, this);
        mGenericRight = new GenericPanel();
        right->setWidget(mGenericRight);
        mActiveGenerics["Inspector"] = mGenericRight;
        addDockWidget(Qt::RightDockWidgetArea, right);

        // Asset Browser
        auto* bottom = new QDockWidget(nullptr, this);
        mGenericBottom = new GenericPanel();
        bottom->setWidget(mGenericBottom);
        mActiveGenerics["Asset Browser"] = mGenericBottom;
        addDockWidget(Qt::BottomDockWidgetArea, bottom);
    }

    void MainWindow::mSpawnGenericPanelWithWidget(const std::string& panelName, QWidget* widgetToAdd) {
        // Count existing panels in mActiveGenerics that match panelName
        int count = 0;
        for (const auto& pair : mActiveGenerics) {
            if (pair.first == panelName) {
                count++;
            }
        }

        auto* newPanel = new GenericPanel();
        newPanel->addNewTab(widgetToAdd, QString::fromStdString(panelName + (count > 0 ? " (" + std::to_string(count) + ")" : "")));
        mActiveGenerics[panelName] = newPanel;

        auto* dockWidget = new QDockWidget(nullptr, this);

        connect(dockWidget, &QDockWidget::destroyed, this, [this, panelName]() {
            mRemoveFromActiveGenerics(panelName);
        });

        dockWidget->setWidget(newPanel);
        // Don't auto-dock; let the user place it
        dockWidget->setFloating(true);
        dockWidget->show();
    }

    void MainWindow::mRemoveFromActiveGenerics(const std::string& panelName) {
        mActiveGenerics.erase(panelName);
    }
}