// start_window_widget_new_project.cpp
// Licensed under LGPLv2.1

#include <sierra/ui/widgets/start_window_widget_new_project.hpp>
#include <sierra/ui/window/main_window.hpp>
#include <sierra/logger.hpp>

#if !defined(__WIN32__)
    #include <unistd.h>
    #include <sys/stat.h>
    #include <sys/types.h>
#else
    #include <windows.h>
    #include <direct.h>
#endif

namespace SierraEditor::UI {
    StartWindowWidgetNewProject::StartWindowWidgetNewProject(QWidget* parent)
        : QWidget(parent)
    {
        this->setWindowTitle("New Project");
        this->setMinimumWidth(500);
        
        QFont titleFont("Sans Serif", 24, QFont::Bold);
        mTitleLabel = new QLabel("New Project", this);
        mTitleLabel->setFont(titleFont);

        QSpacerItem* titleSpacer = new QSpacerItem(0, 10, QSizePolicy::Minimum, QSizePolicy::Fixed);
        
        mNameLabel = new QLabel("Project Name:", this);

        #if !defined(__WIN32__)
        mProjectPath = new QLabel("Project Path: /path/to/project", this);
        #else
        mProjectPath = new QLabel("Project Path: C:\\path\\to\\project", this);
        #endif

        mNameInput = new QLineEdit(this);
        mPathInput = new QLineEdit(this);
        mFileDialogButton = new QPushButton("Browse...", this);
        mCreateProjectButton = new QPushButton("Create Project", this);

        // Bind to file dialog
        connect(mFileDialogButton, &QPushButton::clicked, [this]() {
            QString dir = QFileDialog::getExistingDirectory(this, tr("Select Project Directory"), "/", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks | QFileDialog::DontUseNativeDialog);
            if (!dir.isEmpty()) {
                mPathInput->setText(dir);
            }
        });

        // Make a directory at that path with the project name (platform specific)
        connect(mCreateProjectButton, &QPushButton::clicked, [this]() {
            if (mNameInput->text().isEmpty() || mPathInput->text().isEmpty()) {
                // Tell user to fill in all fields
                QMessageBox::warning(
                    this, // Parent widget (nullptr for no parent)
                    "Missing Information", // Title of the message box
                    "Please fill in both the project name and/or project path.", // Message text
                    QMessageBox::Ok, // Standard buttons
                    QMessageBox::Ok // Default button
                );
                return;
            }

            #if !defined(__WIN32__)

            QString fullPath = mPathInput->text() + "/" + mNameInput->text();
            mkdir(fullPath.toStdString().c_str(), 0755);

            // Write a placeholder project file in the directory (XML format maybe?)
            QFile file(fullPath + "/" + mNameInput->text() + ".sierra");
            if (file.open(QIODevice::WriteOnly)) {
                QTextStream stream(&file);
                stream << "<SierraProject>\n";
                stream << "    <Version>1</Version>\n";
                stream << "    <Name>" << mNameInput->text() << "</Name>\n";
                stream << "    <Directories>\n";
                stream << "        <Assets>assets/</Assets>\n"; // Might have to be ./assets/ depending on how we handle paths
                stream << "        <Scripts>scripts/</Scripts>\n";
                stream << "        <Scenes>scenes/</Scenes>\n";
                stream << "    </Directories>\n";
                stream << "    <Metadata>\n";
                stream << "        <Created>" << QDate::currentDate().toString(Qt::ISODate) << "</Created>\n";
                stream << "        <Modified>" << QDate::currentDate().toString(Qt::ISODate) << "</Modified>\n";
                stream << "    </Metadata>\n";
                stream << "    <Preferences>\n";
                stream << "        <LastOpenedScene></LastOpenedScene>\n";
                stream << "    </Preferences>\n";
                stream << "</SierraProject>\n";
                file.close();
            }

            // These directories MUST exist. The editor will bundle assets and compile scripts from these folders.
            // ANY assets/scripts outside these folders will be IGNORED and will probably fail the build.
            mkdir((fullPath + "/assets").toStdString().c_str(), 0755);
            mkdir((fullPath + "/scripts").toStdString().c_str(), 0755);
            mkdir((fullPath + "/scenes").toStdString().c_str(), 0755);

            this->close();
            auto* mainWindow = new MainWindow(std::string(fullPath.toStdString() + "/" + mNameInput->text().toStdString() + ".sierra").c_str(), nullptr);
            mainWindow->show();

            //TODO("Forwarding the user to the main window after project creation should auto-load the corresponding project.");

            #else

            QString fullPath = mPathInput->text() + "\\" + mNameInput->text();
            _mkdir(fullPath.toStdString().c_str());

            QFile file(fullPath + "\\" + mNameInput->text() + ".sierra");
            if (file.open(QIODevice::WriteOnly)) {
                QTextStream stream(&file);
                stream << "<SierraProject>\n";
                stream << "    <Version>1</Version>\n";
                stream << "    <Name>" << mNameInput->text() << "</Name>\n";
                stream << "    <Directories>\n";
                stream << "        <Assets>assets\\</Assets>\n"; /* Or .\\assets\\ */
                stream << "        <Scripts>scripts\\</Scripts>\n";
                stream << "        <Scenes>scenes\\</Scenes>\n";
                stream << "    </Directories>\n";
                stream << "    <Metadata>\n";
                stream << "        <Created>" << QDate::currentDate().toString(Qt::ISODate) << "</Created>\n";
                stream << "        <Modified>" << QDate::currentDate().toString(Qt::ISODate) << "</Modified>\n";
                stream << "    </Metadata>\n";
                stream << "    <Preferences>\n";
                stream << "        <LastOpenedScene></LastOpenedScene>\n";
                stream << "    </Preferences>\n";
                stream << "</SierraProject>\n";
                file.close();
            }

            _mkdir((fullPath + "\\assets").toStdString().c_str());
            _mkdir((fullPath + "\\scripts").toStdString().c_str());
            _mkdir((fullPath + "\\scenes").toStdString().c_str());

            this->close();
            auto* mainWindow = new MainWindow(std::string(fullPath.toStdString() + "\\" + mNameInput->text().toStdString() + ".sierra").c_str(), nullptr);
            mainWindow->show();

            TODO("Forwarding the user to the main window after project creation should auto-load the corresponding project.");
            
            #endif
        });

        QVBoxLayout* layout = new QVBoxLayout(this);
        layout->addWidget(mTitleLabel);
        layout->addItem(titleSpacer);
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