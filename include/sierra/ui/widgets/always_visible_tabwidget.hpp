#pragma once

#include <QTabBar>
#include <QTabWidget>

class AlwaysVisibleTabWidget : public QTabWidget {
    Q_OBJECT
public:
    using QTabWidget::QTabWidget;

protected:
    void showEvent(QShowEvent* event) override {
        QTabWidget::showEvent(event);

        // Force the tab bar to always show
        if (tabBar())
            tabBar()->show();
    }

    void hideEvent(QHideEvent* event) override {
        // Prevent auto-hiding
        if (tabBar())
            tabBar()->show();

        QTabWidget::hideEvent(event);
    }

    void setVisible(bool v) override {
        QTabWidget::setVisible(v);

        if (tabBar())
            tabBar()->show();
    }
};