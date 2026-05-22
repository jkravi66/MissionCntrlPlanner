#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "MissionManager.h"
#include "MapEngine.h"
#include <QMainWindow>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <QDockWidget>

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void onWaypointAdded(QPointF pos);
    void onWaypointRemoved();
    void onUpload();            // SDD-030, SDD-031, SDD-032, SDD-041
    void onClear();             // SDD-043

private:
    void updateStatus();        // SDD-051, SDD-052, SDD-053, SDD-054
    void setupLogPanel();       // SDD-030, SDD-031, SDD-032
    void logMessage(const QString& msg);  // SDD-030, SDD-032
    MissionManager  m_mission;
    MapEngine*      m_mapEngine;
    QLabel*         m_status;
    QDockWidget*    m_logDock;
    QTextEdit*      m_logView;
};

#endif // MAINWINDOW_H
