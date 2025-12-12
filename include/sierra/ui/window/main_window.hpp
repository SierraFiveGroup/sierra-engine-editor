// main_window.hpp
// Licensed under LGPLv2

#pragma once

#include <QMainWindow>
#include <QMenuBar>
#include <QDockWidget>
#include <sierra/ui/window/main_window.hpp>

#include <sierra/ui/widgets/hierarchy_panel.hpp>
#include <sierra/ui/widgets/inspector_panel.hpp>
#include <sierra/ui/widgets/render_viewport.hpp>
#include <sierra/ui/widgets/asset_browser.hpp>

namespace SierraEditor::UI {
    class MainWindow : public QMainWindow {
        Q_OBJECT
        public:
            explicit MainWindow(QWidget* parent = nullptr);
        
        private:
            void mSetupDockPanels();
            void mSetupMenus();

            RenderViewport* mViewport;
            HierarchyPanel* mHierarchy;
            InspectorPanel* mInspector;
            AssetBrowser* mAssetBrowser;
    };
}