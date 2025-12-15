// main_window.hpp
// Licensed under LGPLv2

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

#include <sierra/ui/widgets/hierarchy_panel.hpp>
#include <sierra/ui/widgets/inspector_panel.hpp>
#include <sierra/ui/widgets/render_viewport.hpp>
#include <sierra/ui/widgets/asset_browser.hpp>
#include <sierra/ui/widgets/generic_panel.hpp>
#include <sierra/project/sproject.hpp>
#include <unordered_map>
#include <memory>
#include <string>

namespace SierraEditor::UI {
    class MainWindow : public QMainWindow {
        Q_OBJECT
        public:
            explicit MainWindow(const char* projectPath, QWidget* parent = nullptr);
        
        private:
            void mSetupDockPanels();
            void mSetupMenus();

            std::unordered_map<std::string, QDockWidget*> mActiveWidgets;
            std::shared_ptr<Project::SProject> mCurrentProject;
    
            RenderViewport* mViewport;
            GenericPanel* mGenericLeft;
            GenericPanel* mGenericRight;
            GenericPanel* mGenericBottom;
    };
}