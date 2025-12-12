// start_window_widget.cpp
// Licensed under LGPLv2

#include <sierra/ui/widgets/start_window_widget.hpp>
#include <sierra/ui/window/main_window.hpp>

namespace SierraEditor::UI {
    StartWindowWidget::StartWindowWidget(QWidget* parent)
        : QWidget(parent)
    {
        this->setWindowTitle("Start Window");
        mWelcomeLabel = new QLabel("Welcome to Sierra Engine Editor! (devnote: press any button to open main window)", this);
        
        mNewProjectButton = new QPushButton("New Project", this);
        mOpenProjectButton = new QPushButton("Open Project", this);

        // Hook method to open main window on button click
        connect(mNewProjectButton, &QPushButton::clicked, [this]() {
            this->close();
            auto* mainWindow = new MainWindow();
            mainWindow->show();
        });

        connect(mOpenProjectButton, &QPushButton::clicked, [this]() {
            this->close();
            auto* mainWindow = new MainWindow();
            mainWindow->show();
        });

        QVBoxLayout* layout = new QVBoxLayout(this);
        layout->addWidget(mWelcomeLabel);
        layout->addWidget(mNewProjectButton);
        layout->addWidget(mOpenProjectButton);
        layout->setContentsMargins(10, 10, 10, 10);
        setLayout(layout);
    }
}