// generic_panel.hpp
// Licensed under LGPLv2.1

#pragma once

#include <QWidget>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QToolButton>
#include <QMenu>
#include <sierra/ui/widgets/always_visible_tabwidget.hpp>
#include <sierra/ui/widgets/asset_browser.hpp>
#include <sierra/ui/widgets/inspector_panel.hpp>
#include <sierra/ui/widgets/hierarchy_panel.hpp>
#include <sierra/ui/widgets/console_output.hpp>
#include <sierra/ui/widgets/profiler_panel.hpp>

namespace SierraEditor::UI {
    class GenericPanel : public QWidget {
        Q_OBJECT

    public:
        explicit GenericPanel(QWidget* parent = nullptr);
        void setActiveTab(int index) { tabs->setCurrentIndex(index); }

    public slots:
        void addNewTab(QWidget* w, const QString& title);

    private:
        void mShowPanelMenu();

        //QTabWidget* tabs = nullptr;
        AlwaysVisibleTabWidget* tabs = nullptr;
    };
}