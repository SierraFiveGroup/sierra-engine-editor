// start_window_widget_new_project.hpp
// Licensed under LGPLv2

#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QObject>
#include <QLineEdit>
#include <QFileDialog>
#include <QString>

namespace SierraEditor::UI {
    class StartWindowWidgetNewProject : public QWidget {
        Q_OBJECT
        public:
            explicit StartWindowWidgetNewProject(QWidget* parent = nullptr);
    
        private:
            QLabel* mNameLabel;
            QLabel* mProjectPath;
            QLineEdit* mNameInput;
            QLineEdit* mPathInput;
            QPushButton* mFileDialogButton;
            QPushButton* mCreateProjectButton;
    };
}