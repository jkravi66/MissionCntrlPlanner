#include "MainWindow.h"
#include <QToolBar>
#include <QAction>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QWidget>
#include <QPushButton>
#include <QDateTime>
#include <QScrollBar>
#include <QIcon>

// ---------------------------------------------------------------------------
// MainWindow
// ---------------------------------------------------------------------------
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle("Mission Planner");
    resize(1400, 650);

    QIcon icon(":/icons/Avilus_Logo.svg");
    setWindowIcon(icon);

    // Central widget
    m_mapengine = new MapEngine(this);
    setCentralWidget(m_mapengine);

    connect(m_mapengine, &MapEngine::waypointAdded,   this, &MainWindow::onWaypointAdded);
    connect(m_mapengine, &MapEngine::waypointRemoved, this, &MainWindow::onWaypointRemoved);

    // Toolbar [SDD-041, SDD-042, SDD-043]
    QToolBar* toolBar = addToolBar("Mission");
    toolBar->setMovable(false);

    // [SDD-041] "Upload Mission" triggers validation and upload.
    auto* upload = toolBar->addAction("Upload Mission");
    connect(upload, &QAction::triggered, this, &MainWindow::onUpload);

    toolBar->addSeparator();

    // [SDD-042] "Remove Last" removes the most recently placed waypoint.
    auto* undo = toolBar->addAction("Remove Last");
    connect(undo, &QAction::triggered, m_mapengine, &MapEngine::removeLastWaypoint);

    // [SDD-043] "Clear All" removes all waypoints and resets the mission.
    auto* clear = toolBar->addAction("Clear All");
    connect(clear, &QAction::triggered, this, &MainWindow::onClear);

    // Status bar [SDD-051] Initial "no waypoints" message.
    m_status = new QLabel("Click the map to place waypoints.");
    statusBar()->addWidget(m_status);

    // Log panel
    setupLogPanel();

    logMessage("Mission Planner started. Click the map to place waypoints.");
}

// ---------------------------------------------------------------------------
// SDD-030, SDD-031, SDD-032
// setupLogPanel - Builds a QDockWidget on the right side containing a
// read-only text area and a Clear Log button at the bottom.
// ---------------------------------------------------------------------------
void MainWindow::setupLogPanel() {
    m_logDock = new QDockWidget("Mission Log", this);
    m_logDock->setAllowedAreas(Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
    m_logDock->setMinimumWidth(480);
    m_logDock->setFeatures(
        QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable
        );

    QWidget*     container = new QWidget(m_logDock);
    QVBoxLayout* layout    = new QVBoxLayout(container);
    layout->setContentsMargins(4, 4, 4, 4);
    layout->setSpacing(4);

    // Read-only text area for mission log output.
    m_logView = new QTextEdit(container);
    m_logView->setReadOnly(true);
    m_logView->setFont(QFont("Courier New", 9));
    m_logView->setStyleSheet(
        "QTextEdit {"
        "  background-color: #454560;"
        "  color: #d4d4d4;"
        "  border: 1px solid #3a3a50;"
        "}"
        );

    QPushButton* clearLog = new QPushButton("Clear Log", container);
    clearLog->setFixedHeight(26);
    connect(clearLog, &QPushButton::clicked,
            m_logView, &QTextEdit::clear);

    layout->addWidget(m_logView);
    layout->addWidget(clearLog);
    container->setLayout(layout);

    m_logDock->setWidget(container);
    addDockWidget(Qt::RightDockWidgetArea, m_logDock);
}

// ---------------------------------------------------------------------------
// logMessage — appends a timestamped, HTML-coloured line to the log pane.
// ---------------------------------------------------------------------------
void MainWindow::logMessage(const QString& msg) {
    QString time = QDateTime::currentDateTime().toString("hh:mm:ss");
    m_logView->append(
        QString("<span style='color:#6a9fb5;'>[%1]</span> %2")
            .arg(time, msg.toHtmlEscaped())
        );
    m_logView->verticalScrollBar()->setValue(
        m_logView->verticalScrollBar()->maximum()
        );
}

// ---------------------------------------------------------------------------
// Slots
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// onWaypointAdded
// [SRD-010]  Store the newly placed waypoint in MissionManager and log it.
// ---------------------------------------------------------------------------
void MainWindow::onWaypointAdded(QPointF pos) {
    m_mission.addWaypoint(pos.x(), pos.y());

    int n = (int)m_mission.rawPoints().size();
    logMessage(QString("Waypoint %1 placed — x=%2  y=%3")
                   .arg(n)
                   .arg(pos.x(), 0, 'f', 2)
                   .arg(pos.y(), 0, 'f', 2));

    updateStatus();
}

// ---------------------------------------------------------------------------
// onWaypointRemoved
// [SRD-005]  Remove the last waypoint from MissionManager and log it.
// ---------------------------------------------------------------------------
void MainWindow::onWaypointRemoved() {
    m_mission.removeLastWaypoint();
    logMessage("Last waypoint removed.");
    updateStatus();
}

// ---------------------------------------------------------------------------
// onUpload
// [SDD-030, SDD-031, SDD-032]
// onUpload validates the mission (SDD-031), then builds and prints the
// full typed trajectory in execution order (SDD-030, SDD-032).
// ---------------------------------------------------------------------------
void MainWindow::onUpload() {
    // [SDD-031] Abort with error message if mission is invalid.
    if (!m_mission.isValid()) {
        m_logView->append(
            QString("<span style='color:#e06c75;'>[ERROR] %1</span>")
                .arg(m_mission.validationError().toHtmlEscaped())
            );
        return;
    }

    auto traj = m_mission.buildTrajectory();

    // [SDD-032] Print "Mission Upload" header.
    m_logView->append(
        "<span style='color:#98c379;'>--- Mission Upload ---</span>"
        );

    // [SDD-030] Print each waypoint: [IDX] TYPE  x=<val>  y=<val>  alt=<val>m
    for (const Waypoint& wp : traj) {
        QString color;
        switch (wp.type) {
        case WaypointType::TAKEOFF: color = "#e5c07b"; break; //Green
        case WaypointType::CRUISE:  color = "#FAFAFA"; break; //Blue
        case WaypointType::LAND:    color = "#e5c07b"; break; //Red
        }

        QString line = QString("[%1] %2  x=%3  y=%4  alt=%5m")
                           .arg(wp.index,  2)
                           .arg(QString(typeToString(wp.type)), -8)
                           .arg(wp.x,      8, 'f', 3)
                           .arg(wp.y,      8, 'f', 3)
                           .arg(wp.altitude, 0, 'f', 1);

        m_logView->append(
            QString("<span style='color:%1; font-family:Courier New;'>%2</span>")
                .arg(color, line.toHtmlEscaped())
            );
    }

    // [SDD-032] Print total count footer.
    m_logView->append(
        QString("<span style='color:#98c379;'>--- %1 waypoints uploaded ---</span>")
            .arg(traj.size())
        );

    m_logView->verticalScrollBar()->setValue(
        m_logView->verticalScrollBar()->maximum()
        );

    updateStatus();
}

// ---------------------------------------------------------------------------
// onClear
// [SDD-043] → SRD-006  Clear All: removes all waypoints from map and mission model.
// ---------------------------------------------------------------------------
void MainWindow::onClear() {
    m_mapengine->clearAll();
    m_mission.clear();
    logMessage("Mission cleared.");
    updateStatus();
}

// ---------------------------------------------------------------------------
// updateStatus — reflects current waypoint count and validity in the status bar.
// [SDD-051] → No waypoints: prompt message.
// [SDD-052] → Invalid mission: count + error text.
// [SDD-053] → Valid mission: count + "Ready to upload."
// [SDD-054] → Called after every add, remove, and clear operation.
// ---------------------------------------------------------------------------
void MainWindow::updateStatus() {
    int n = (int)m_mission.rawPoints().size();
    if (n == 0)
        m_status->setText("Click the map to place waypoints.");                         // [SDD-051]
    else if (!m_mission.isValid())
        m_status->setText(QString("Waypoints: %1 | %2").arg(n)
                              .arg(m_mission.validationError()));                       // [SDD-052]
    else
        m_status->setText(QString("Waypoints: %1 | Mission valid. Ready to upload.")    // [SDD-053]
                              .arg(n));
}
