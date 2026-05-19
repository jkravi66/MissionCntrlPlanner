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

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle("Mission Planner");
    resize(1400, 650);

    QIcon icon(":/icons/Avilus_Logo.svg");
    if (icon.isNull()) {
        qDebug() << "Icon failed to load — check .qrc path and SVG module";
    } else {
        setWindowIcon(icon);
        qDebug() << "Icon loaded successfully";
    }

    // --- centre ---
    m_mapengine = new MapEngine(this);
    setCentralWidget(m_mapengine);

    connect(m_mapengine, &MapEngine::waypointAdded,   this, &MainWindow::onWaypointAdded);
    connect(m_mapengine, &MapEngine::waypointRemoved, this, &MainWindow::onWaypointRemoved);

    // --- Toolbar ---
    QToolBar* tb = addToolBar("Mission");
    tb->setMovable(false);

    auto* upload = tb->addAction("Upload Mission");
    connect(upload, &QAction::triggered, this, &MainWindow::onUpload);

    tb->addSeparator();

    auto* undo = tb->addAction("Remove Last");
    connect(undo, &QAction::triggered, m_mapengine, &MapEngine::removeLastWaypoint);

    auto* clear = tb->addAction("Clear All");
    connect(clear, &QAction::triggered, this, &MainWindow::onClear);

    // --- Status bar ---
    m_status = new QLabel("Click the map to place waypoints.");
    statusBar()->addWidget(m_status);

    // --- Log panel  ---
    setupLogPanel();

    logMessage("Mission Planner started. Click the map to place waypoints.");
}

// ---------------------------------------------------------------------------
// Log panel setup
// Builds a QDockWidget on the right side containing a read-only text area
// and a Clear Log button at the bottom.
// ---------------------------------------------------------------------------
void MainWindow::setupLogPanel() {
    m_logDock = new QDockWidget("Mission Log", this);
    m_logDock->setAllowedAreas(Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
    m_logDock->setMinimumWidth(480);
    m_logDock->setFeatures(
        QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable
        );

    // Container widget inside the dock
    QWidget*     container = new QWidget(m_logDock);
    QVBoxLayout* layout    = new QVBoxLayout(container);
    layout->setContentsMargins(4, 4, 4, 4);
    layout->setSpacing(4);

    // Text area — read only so the user can scroll and copy but not edit
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

    // Clear log button at the bottom
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
// logMessage — appends a timestamped line to the log pane.
// Uses HTML so we can colour different types of messages.
// ---------------------------------------------------------------------------
void MainWindow::logMessage(const QString& msg) {
    QString time = QDateTime::currentDateTime().toString("hh:mm:ss");
    m_logView->append(
        QString("<span style='color:#6a9fb5;'>[%1]</span> %2")
            .arg(time, msg.toHtmlEscaped())
        );
    // Auto-scroll to bottom
    m_logView->verticalScrollBar()->setValue(
        m_logView->verticalScrollBar()->maximum()
        );
}

// ---------------------------------------------------------------------------
// Slots
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

void MainWindow::onWaypointRemoved() {
    m_mission.removeLastWaypoint();
    logMessage("Last waypoint removed.");
    updateStatus();
}

// [SDD-030, SDD-031]
void MainWindow::onUpload() {
    if (!m_mission.isValid()) {
        // Log the error in red
        m_logView->append(
            QString("<span style='color:#e06c75;'>[ERROR] %1</span>")
                .arg(m_mission.validationError().toHtmlEscaped())
            );
        return;
    }

    // Build trajectory and log each waypoint to the pane
    auto traj = m_mission.buildTrajectory();

    m_logView->append(
        "<span style='color:#98c379;'>--- Mission Upload ---</span>"
        );

    for (const Waypoint& wp : traj) {
        // Colour code each phase
        QString color;
        switch (wp.type) {
        case WaypointType::TAKEOFF: color = "#e5c07b"; break;
        case WaypointType::CRUISE:  color = "#FAFAFA"; break;
        case WaypointType::LAND:    color = "#e5c07b"; break;
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

    m_logView->append(
        QString("<span style='color:#98c379;'>--- %1 waypoints uploaded ---</span>")
            .arg(traj.size())
        );

    // Scroll to bottom after upload
    m_logView->verticalScrollBar()->setValue(
        m_logView->verticalScrollBar()->maximum()
        );

    updateStatus();
}

void MainWindow::onClear() {
    m_mapengine->clearAll();
    m_mission.clear();
    logMessage("Mission cleared.");
    updateStatus();
}

// ---------------------------------------------------------------------------
// Status bar
// ---------------------------------------------------------------------------
void MainWindow::updateStatus() {
    int n = (int)m_mission.rawPoints().size();
    if (n == 0)
        m_status->setText("Click the map to place waypoints.");
    else if (!m_mission.isValid())
        m_status->setText(QString("Waypoints: %1 | %2").arg(n).arg(m_mission.validationError()));
    else
        m_status->setText(QString("Waypoints: %1 | Mission valid. Ready to upload.").arg(n));
}