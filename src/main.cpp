// main.cpp
// Licensed under LGPLv2.1

#include <QApplication>
#include <QFile>
#include <sierra/ui/window/main_window.hpp>
#include <sierra/ui/widgets/start_window_widget.hpp>
#include <cxxopts.hpp>

int main(int argc, char *argv[]) {
    cxxopts::Options options("SierraEngineEditor", "Sierra Engine Editor Application");
    options.add_options()
        ("h,help", "Print help")
        ("p,project", "Project to load on open", cxxopts::value<std::string>()->default_value(""));

    auto optionsObj = options.parse(argc, argv);
    if (optionsObj.count("help")) {
        std::cout << options.help() << std::endl;
        std::cout << "This software is licensed under the LGPLv2.1-only license.\n";
        std::cout << "Copyright (C) 2025, The Sierra Engine Editor Contributors.\n";
        std::cout << "This software is provided under ABSOLUTELY NO WARRANTY, to the extent permitted by law.\n";
        return 0;
    }

    std::string project = optionsObj["project"].as<std::string>();

    QApplication app(argc, argv);

    // Load the theme
    TODO("Make theme load dynamically");
    QFile file("../styles/dark.qss");
    if (file.open(QFile::ReadOnly)) {
        QApplication::setStyle("Fusion");
        app.setStyleSheet(file.readAll());
    } else {
        QApplication::setStyle("Fusion");
    }

    QCoreApplication::setApplicationName("Sierra Engine Editor"); // Might work

    if (project != "") {
        SierraEditor::UI::MainWindow mainWindow = SierraEditor::UI::MainWindow(project.c_str());
        mainWindow.show();

        return app.exec();
    } else {
        SierraEditor::UI::StartWindowWidget startWindow;
        startWindow.show();

        return app.exec();
    }

    //return app.exec();
}