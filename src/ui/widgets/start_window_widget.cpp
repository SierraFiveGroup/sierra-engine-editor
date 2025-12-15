// start_window_widget.cpp
// Licensed under LGPLv2

#include <sierra/ui/widgets/start_window_widget.hpp>
#include <sierra/ui/window/main_window.hpp>
#include <sierra/ui/widgets/start_window_widget_new_project.hpp>

namespace SierraEditor::UI {
    StartWindowWidget::StartWindowWidget(QWidget* parent)
        : QWidget(parent)
    {
        this->setWindowTitle("Start Window");
        this->setMinimumWidth(500);

        QFont titleFont("Sans Serif", 24, QFont::Bold);
        mTitleLabel = new QLabel("Sierra Engine Editor", this);
        mTitleLabel->setFont(titleFont);

        QSpacerItem* titleSpacer = new QSpacerItem(0, 10, QSizePolicy::Minimum, QSizePolicy::Fixed);
        mWelcomeLabel = new QLabel("Welcome to the Sierra Engine Editor!", this);
        
        mNewProjectButton = new QPushButton("New Project", this);
        mOpenProjectButton = new QPushButton("Open Editor", this); // TODO: Change to Open Project dialog

        // Hook method to open main window on button click
        connect(mNewProjectButton, &QPushButton::clicked, [this]() {
            this->close();
            auto* newProjectWindow = new StartWindowWidgetNewProject();
            newProjectWindow->show();
        });

        connect(mOpenProjectButton, &QPushButton::clicked, [this]() {
            this->close();
            auto* mainWindow = new MainWindow("", nullptr);
            mainWindow->show();
        });

        QVBoxLayout* layout = new QVBoxLayout(this);
        layout->addWidget(mTitleLabel);
        layout->addItem(titleSpacer);
        layout->addWidget(mWelcomeLabel);
        layout->addWidget(mNewProjectButton);
        layout->addWidget(mOpenProjectButton);
        layout->setContentsMargins(10, 10, 10, 10);
        setLayout(layout);
    }
}