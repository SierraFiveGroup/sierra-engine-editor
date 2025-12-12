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

#include <sierra/ui/widgets/hierarchy_panel.hpp>
#include <sierra/ui/widgets/inspector_panel.hpp>
#include <sierra/ui/widgets/render_viewport.hpp>
#include <sierra/ui/widgets/asset_browser.hpp>

#include <unordered_map>

#include <string>

namespace SierraEditor::UI {
    class MainWindow : public QMainWindow {
        Q_OBJECT
        public:
            explicit MainWindow(QWidget* parent = nullptr);
        
        private:
            #if defined(__APPLE__)
            constexpr static const char* SUPER_KEY = "Command";
            #else
            constexpr static const char* SUPER_KEY = "Ctrl";
            #endif

            void mSetupDockPanels();
            void mSetupMenus();

            std::unordered_map<std::string, QDockWidget*> mActiveWidgets;
    
            RenderViewport* mViewport;
            HierarchyPanel* mHierarchy;
            InspectorPanel* mInspector;
            AssetBrowser* mAssetBrowser;

            // Docked Widget pointers for the above panels
            QDockWidget* mHierarchyDock;
            QDockWidget* mInspectorDock;
            QDockWidget* mAssetBrowserDock;

            // States
            bool mIsHierarchyOpen = true;
            bool mIsInspectorOpen = true;
            bool mIsAssetBrowserOpen = true;
    };
}