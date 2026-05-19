#include "MainWindow.h"

#include <QApplication>
#include <QFile.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("Mission Planner");
    app.setWindowIcon(QIcon(":/icons/Avilus_Logo.svg"));

    QFile testFile(":/icons/Avilus_Logo.svg");
    qDebug() << "Resource file exists:" << testFile.exists();
    qDebug() << "Resource file size:" << testFile.size();

    MainWindow w;
    w.show();
    return app.exec();
}
