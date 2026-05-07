#include "MainWindow.h"
#include <QToolBar>
#include <QAction>
#include <QStatusBar>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle("MCT – 2D Mission Planner");
    resize(900, 650);

    m_canvas = new MapCanvas(this);
    setCentralWidget(m_canvas);

    connect(m_canvas, &MapCanvas::waypointAdded,   this, &MainWindow::onWaypointAdded);
    connect(m_canvas, &MapCanvas::waypointRemoved, this, &MainWindow::onWaypointRemoved);

    QToolBar* tb = addToolBar("Mission");
    tb->setMovable(false);

    auto* upload = tb->addAction("Upload Mission");
    connect(upload, &QAction::triggered, this, &MainWindow::onUpload);

    tb->addSeparator();

    auto* undo = tb->addAction("Remove Last");
    connect(undo, &QAction::triggered, m_canvas, &MapCanvas::removeLastWaypoint);

    auto* clear = tb->addAction("Clear All");
    connect(clear, &QAction::triggered, this, &MainWindow::onClear);

    m_status = new QLabel("Click the map to place waypoints.");
    statusBar()->addWidget(m_status);
}

void MainWindow::onWaypointAdded(QPointF pos) {
    m_mission.addWaypoint(pos.x(), pos.y());
    updateStatus();
}

void MainWindow::onWaypointRemoved() {
    m_mission.removeLastWaypoint();
    updateStatus();
}

void MainWindow::onUpload() {
    m_mission.uploadMission();
}

void MainWindow::onClear() {
    m_canvas->clearAll();
    m_mission.clear();
    updateStatus();
}

void MainWindow::updateStatus() {
    int n = (int)m_mission.rawPoints().size();
    if (n == 0)
        m_status->setText("Click the map to place waypoints.");
    else if (!m_mission.isValid())
        m_status->setText(QString("Waypoints: %1 | %2").arg(n).arg(m_mission.validationError()));
    else
        m_status->setText(QString("Waypoints: %1 | Mission valid. Ready to upload.").arg(n));
}
