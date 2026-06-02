#include <QApplication>
#include "mainwindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    // Fusion - czysty, nowoczesny wyglad identyczny na Windows/Linux/Mac
    app.setStyle("Fusion");

    MainWindow window;
    window.show();

    return app.exec();
}
