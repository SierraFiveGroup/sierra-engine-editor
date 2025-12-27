// main_window.hpp
// Licensed under LGPLv2.1

#pragma once

#include <QMainWindow>
#include <QMenuBar>
#include <QDockWidget>
#include <QWidget>
#include <QAction>
#include <QKeySequence>
#include <sierra/ui/window/main_window.hpp>
#include <QString>
#include <QFileDialog>
#include <QDir>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QMenu>
#include <QContextMenuEvent>
#include <QInputDialog>
#include <QLineEdit>

#include <sierra/ui/widgets/hierarchy_panel.hpp>
#include <sierra/ui/widgets/inspector_panel.hpp>
#include <sierra/ui/widgets/render_viewport.hpp>
#include <sierra/ui/widgets/asset_browser.hpp>
#include <sierra/ui/widgets/generic_panel.hpp>
#include <sierra/ui/widgets/console_output.hpp>
#include <sierra/ui/widgets/profiler_panel.hpp>
#include <sierra/project/sproject.hpp>
#include <sierra/project/sscene.hpp>
#include <sierra/logger.hpp>
#include <sierra/io/keyboard.hpp>
#include <sierra/io/mouse.hpp>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <string>

namespace SierraEditor::UI {
    class MainWindow : public QMainWindow {
        Q_OBJECT
        public:
            explicit MainWindow(const char* projectPath, QWidget* parent = nullptr);
            void openProject(const QString& projectPath);
            void openScene(const QString& scenePath);

            std::shared_ptr<Project::SScene>* getCurrentSceneRef() {
                return &mCurrentScene;
            }

        private:
            void mSetupDockPanels();
            void mSetupMenus();
            void mSpawnGenericPanelWithWidget(const std::string& panelName, QWidget* widgetToAdd);
            void mRemoveFromActiveGenerics(const std::string& panelName);
            void mCreateContextMenuForWidget(QMenu* menu, QWidget* widget);
            QWidget* mGetWidgetUnderCursor(const QPoint& globalPos);

            std::unordered_map<std::string, GenericPanel*> mActiveGenerics;
            std::shared_ptr<Project::SProject> mCurrentProject = nullptr;
            std::shared_ptr<Project::SScene> mCurrentScene = nullptr;
    
            RenderViewport* mViewport;
            GenericPanel* mGenericLeft;
            GenericPanel* mGenericRight;
            GenericPanel* mGenericBottom;

        protected:
            void keyPressEvent(QKeyEvent* event) override {
                IO::Keyboard::handleKeyPress(event);
                QMainWindow::keyPressEvent(event);
            }

            void keyReleaseEvent(QKeyEvent* event) override {
                IO::Keyboard::handleKeyRelease(event);
                QMainWindow::keyReleaseEvent(event);
            }

            void mousePressEvent(QMouseEvent* event) override {
                IO::Mouse::handleButtonPress(event);
                QMainWindow::mousePressEvent(event);
            }

            void mouseReleaseEvent(QMouseEvent* event) override {
                IO::Mouse::handleButtonRelease(event);
                QMainWindow::mouseReleaseEvent(event);
            }

            void mouseMoveEvent(QMouseEvent* event) override {
                IO::Mouse::handleMouseMove(event);
                QMainWindow::mouseMoveEvent(event);
            }

            void contextMenuEvent(QContextMenuEvent* event) override;
    };
}