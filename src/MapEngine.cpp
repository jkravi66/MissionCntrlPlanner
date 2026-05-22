#include "MapEngine.h"
#include <QMouseEvent>
#include <QPen>
#include <QBrush>

// ---------------------------------------------------------------------------
// [SDD-020] → SRD-001, SYS-006
// MapEngine extends QGraphicsView and owns a QGraphicsScene.
// The constructor draws the background grid and sets up the scene rectangle.
// ---------------------------------------------------------------------------
MapEngine::MapEngine(QWidget* parent)
    : QGraphicsView(parent)
    , m_scene(new QGraphicsScene(this))
{
    setScene(m_scene);
    setRenderHint(QPainter::Antialiasing);
    setBackgroundBrush(QBrush(QColor(28, 28, 36)));
    m_scene->setSceneRect(-500, -500, 1000, 1000);
    drawGrid();
}

// ---------------------------------------------------------------------------
// [SDD-020] → SRD-001  Render a 2D canvas with a visible grid.
// ---------------------------------------------------------------------------
void MapEngine::drawGrid() {
    QPen minor(QColor(55, 60, 75), 0.5);
    QPen major(QColor(90, 100, 120), 1.0);

    for (int v = -500; v <= 500; v += 50) {
        bool isAxis = (v == 0);
        m_scene->addLine(v, -500, v,  500, isAxis ? major : minor)->setZValue(-1);
        m_scene->addLine(-500, v,  500, v, isAxis ? major : minor)->setZValue(-1);
    }
}

// ---------------------------------------------------------------------------
// [SDD-021, SDD-022] → SRD-002, SRD-003, SYS-001
// mousePressEvent converts screen coordinates to scene coordinates and
// places a circular marker, then emits waypointAdded(QPointF).
// ---------------------------------------------------------------------------
void MapEngine::mousePressEvent(QMouseEvent* event) {
    if (event->button() != Qt::LeftButton) {
        QGraphicsView::mousePressEvent(event);
        return;
    }

    QPointF pos = mapToScene(event->pos());

    auto* marker = m_scene->addEllipse(
        pos.x() - MARKER_R, pos.y() - MARKER_R,
        MARKER_R * 2, MARKER_R * 2,
        QPen(Qt::NoPen), QBrush(Qt::blue)
        );
    marker->setZValue(2);
    m_markers.push_back(marker);

    if (m_markers.size() >= 2) {
        QPointF prev = m_markers[m_markers.size()-2]->rect().center();
        auto* line = m_scene->addLine(
            prev.x(), prev.y(), pos.x(), pos.y(),
            QPen(QColor(255, 210, 60), 1.5, Qt::DashLine)
            );
        line->setZValue(1);
        m_lines.push_back(line);
    }

    refreshColors();
    emit waypointAdded(pos);
}

// ---------------------------------------------------------------------------
// [SDD-021] → SRD-002  Support mouse-wheel zoom on the 2D canvas.
// ---------------------------------------------------------------------------
void MapEngine::wheelEvent(QWheelEvent* event) {
    double factor = (event->angleDelta().y() > 0) ? 1.15 : 0.87;
    scale(factor, factor);
}

// ---------------------------------------------------------------------------
// [SDD-024] → SRD-005
// removeLastWaypoint removes the last marker ellipse and the connecting
// trajectory line, then refreshes marker colours and emits waypointRemoved().
// ---------------------------------------------------------------------------
void MapEngine::removeLastWaypoint() {
    if (m_markers.empty()) return;

    if (!m_lines.empty()) {
        m_scene->removeItem(m_lines.back());
        delete m_lines.back();
        m_lines.pop_back();
    }

    m_scene->removeItem(m_markers.back());
    delete m_markers.back();
    m_markers.pop_back();

    refreshColors();
    emit waypointRemoved();
}

// ---------------------------------------------------------------------------
// [SDD-025] → SRD-006
// clearAll removes all markers and trajectory lines by delegating to
// removeLastWaypoint() until the scene is empty.
// ---------------------------------------------------------------------------
void MapEngine::clearAll() {
    while (!m_markers.empty())
        removeLastWaypoint();
}

// ---------------------------------------------------------------------------
// [SDD-023] → SRD-004
// refreshColors colour-codes markers: first = green (start), last = red (end),
// all others = blue (intermediate).
// ---------------------------------------------------------------------------
void MapEngine::refreshColors() {
    if (m_markers.empty()) return;
    for (size_t i = 0; i < m_markers.size(); ++i) {
        QColor c;
        if      (i == 0)                        c = QColor(Qt::green);
        else if (i == m_markers.size() - 1)     c = QColor(Qt::red);
        else                                    c = QColor(Qt::blue);
        m_markers[i]->setBrush(QBrush(c));
    }
}

// ---------------------------------------------------------------------------