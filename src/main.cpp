// main.cpp
// Licensed under LGPLv2

#include <QApplication>
#include <sierra/ui/window/main_window.hpp>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    SierraEditor::UI::MainWindow window;
    window.show();

    return app.exec();
}