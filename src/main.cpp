// main.cpp
// Licensed under LGPLv2

#include <QApplication>
#include <QFile>
#include <sierra/ui/window/main_window.hpp>
#include <sierra/ui/widgets/start_window_widget.hpp>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Load the theme
    QFile file("../styles/dark.qss");
    if (file.open(QFile::ReadOnly)) {
        QApplication::setStyle("Fusion");
        app.setStyleSheet(file.readAll());
    } else {
        QApplication::setStyle("Fusion");
    }

    QCoreApplication::setApplicationName("Sierra Engine Editor"); // Might work

    //SierraEditor::UI::MainWindow mainWindow;
    //mainWindow.show();

    SierraEditor::UI::StartWindowWidget startWindow;
    startWindow.show();

    return app.exec();
}