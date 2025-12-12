// start_window_widget_new_project.cpp
// Licensed under LGPLv2

#include <sierra/ui/widgets/start_window_widget_new_project.hpp>
#include <sierra/ui/window/main_window.hpp>

namespace SierraEditor::UI {
    StartWindowWidgetNewProject::StartWindowWidgetNewProject(QWidget* parent)
        : QWidget(parent)
    {
        this->setWindowTitle("New Project");
        mNameLabel = new QLabel("Project Name:", this);

        #if !defined(__WIN32__)
        mProjectPath = new QLabel("Project Path: /path/to/project", this);
        #else
        mProjectPath = new QLabel("Project Path: C:\\path\\to\\project", this);
        #endif

        mFileDialogButton = new QPushButton("Browse...", this);

        // Bind to file dialog
        connect(mFileDialogButton, &QPushButton::clicked, [this]() {
            QString dir = QFileDialog::getExistingDirectory(this, tr("Select Project Directory"), "/", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
            if (!dir.isEmpty()) {
                mPathInput->setText(dir);
            }
        });

        mCreateProjectButton = new QPushButton("Create Project", this);

        mNameInput = new QLineEdit(this);
        mPathInput = new QLineEdit(this);

        QVBoxLayout* layout = new QVBoxLayout(this);
        layout->addWidget(mNameLabel);
        layout->addWidget(mNameInput);
        layout->addWidget(mProjectPath);
        layout->addWidget(mPathInput);
        layout->addWidget(mFileDialogButton);
        layout->addWidget(mCreateProjectButton);
        layout->setContentsMargins(10, 10, 10, 10);
        setLayout(layout);
    }
}