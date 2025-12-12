// main.cpp
// Licensed under LGPLv2

#include <QApplication>
#include <sierra/ui/window/main_window.hpp>
#include <sierra/ui/widgets/start_window_widget.hpp>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QCoreApplication::setApplicationName("Sierra Engine Editor"); // Might work

    //SierraEditor::UI::MainWindow mainWindow;
    //mainWindow.show();

    SierraEditor::UI::StartWindowWidget startWindow;
    startWindow.show();

    return app.exec();
}