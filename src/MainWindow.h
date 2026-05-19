#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "MissionManager.h"
#include "MapEngine.h"
#include <QMainWindow>
#include <QLabel>

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void onWaypointAdded(QPointF pos);
    void onWaypointRemoved();
    void onUpload();
    void onClear();

private:
    void updateStatus();
    MissionManager m_mission;
    MapEngine*     m_mapengine;
    QLabel*        m_status;
};

#endif // MAINWINDOW_H
