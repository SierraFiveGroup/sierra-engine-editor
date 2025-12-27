// generic_panel.cpp
// Licensed under LGPLv2.1

#include <sierra/ui/widgets/generic_panel.hpp>
#include <sierra/ui/window/main_window.hpp>

#include <QTabBar>
#include <QAction>
#include <QCursor>
#include <QDockWidget>

namespace SierraEditor::UI {

    GenericPanel::GenericPanel(QWidget* parent)
        : QWidget(parent)
    {
        tabs = new AlwaysVisibleTabWidget(this);
        tabs->setTabsClosable(true);

        tabs->tabBar()->setVisible(true);
        tabs->setDocumentMode(false);

        connect(tabs, &QTabWidget::tabCloseRequested, this, [this](int index){
            QWidget* w = tabs->widget(index);
            tabs->removeTab(index);
            if (w) w->deleteLater();

            if (tabs->count() == 0) {
                // Destroy this panel if no tabs remain, and destroy the parent dock widget as well
                QWidget* p = this->parentWidget();
                if (p) p->deleteLater();
            }
        });

        auto* layout = new QVBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(tabs);

        // Create the "+" button
        QToolButton* plusButton = new QToolButton();
        plusButton->setText("+");
        plusButton->setAutoRaise(true);
        plusButton->setToolTip("Add panel");
        plusButton->setMaximumWidth(30);
        plusButton->setMaximumHeight(24);

        // Ensure tab bar has minimum height for corner widget visibility
        tabs->tabBar()->setMinimumHeight(24);

        tabs->setCornerWidget(plusButton, Qt::TopRightCorner);

        connect(plusButton, &QToolButton::clicked, this, [this]() {
            mShowPanelMenu();
        });
    }

    void GenericPanel::mShowPanelMenu()
    {
        QMenu menu(this);

        QAction* inspectorAction = menu.addAction("Inspector Panel");
        QAction* hierarchyAction = menu.addAction("Hierarchy Panel");
        QAction* assetAction     = menu.addAction("Asset Browser");
        QAction* consoleAction   = menu.addAction("Console Output");
        QAction* profilerAction  = menu.addAction("Profiler Panel");

        QAction* chosen = menu.exec(QCursor::pos());
        if (!chosen)
            return;

        QWidget* newWidget = nullptr;
        QString title;

        // Get parent as MainWindow to pass current scene reference if needed
        QWidget* parentWidget = this->parentWidget();
        while (parentWidget) {
            if (auto* dock = qobject_cast<QDockWidget*>(parentWidget)) {
                parentWidget = dock->parentWidget();
            } else {
                break;
            }
        }

        if (chosen == inspectorAction) {
            newWidget = new InspectorPanel();
            title = "Inspector";
        }
        else if (chosen == hierarchyAction) {
            newWidget = new HierarchyPanel(parentWidget
                ? static_cast<MainWindow*>(parentWidget)->getCurrentSceneRef()
                : nullptr, nullptr);
            title = "Hierarchy";
        }
        else if (chosen == assetAction) {
            newWidget = new AssetBrowser();
            title = "Asset Browser";
        }
        else if (chosen == consoleAction) {
            newWidget = new ConsoleOutputWidget();
            title = "Console";
        }
        else if (chosen == profilerAction) {
            newWidget = new ProfilerPanel();
            title = "Profiler";
        }

        addNewTab(newWidget, title);
    }

    void GenericPanel::addNewTab(QWidget* w, const QString& title)
    {
        int newIndex = tabs->addTab(w, title);
        tabs->setCurrentIndex(newIndex);
    }
}