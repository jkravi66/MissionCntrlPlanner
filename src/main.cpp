#include "MainWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("MCT Mission Planner");
    MainWindow w;
    w.show();
    return app.exec();
}
