// start_window_widget.hpp
// Licensed under LGPLv2.1

#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QObject>
#include <QFont>
#include <QSpacerItem>
#include <QListView>
//#include <QListViewItem>

namespace SierraEditor::UI {
    class StartWindowWidget : public QWidget {
        Q_OBJECT
        public:
            explicit StartWindowWidget(QWidget* parent = nullptr);
    
        private:
            QLabel* mTitleLabel;
            QLabel* mWelcomeLabel;
            QPushButton* mNewProjectButton;
            QPushButton* mOpenProjectButton;

            QListView* mRecentProjectsList;
    
    };
}