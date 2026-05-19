#include "MapEngine.h"

#include "MapEngine.h"
#include <QMouseEvent>
#include <QPen>
#include <QBrush>

MapCanvas::MapCanvas(QWidget* parent)
    : QGraphicsView(parent)
    , m_scene(new QGraphicsScene(this))
{
    setScene(m_scene);
    setRenderHint(QPainter::Antialiasing);
    setBackgroundBrush(QBrush(QColor(28, 28, 36)));
    m_scene->setSceneRect(-500, -500, 1000, 1000);
    drawGrid();
}

void MapCanvas::drawGrid() {
    QPen minor(QColor(55, 60, 75), 0.5);
    QPen major(QColor(90, 100, 120), 1.0);

    for (int v = -500; v <= 500; v += 50) {
        bool isAxis = (v == 0);
        m_scene->addLine(v, -500, v,  500, isAxis ? major : minor)->setZValue(-1);
        m_scene->addLine(-500, v,  500, v, isAxis ? major : minor)->setZValue(-1);
    }
}

void MapCanvas::mousePressEvent(QMouseEvent* event) {
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

void MapCanvas::wheelEvent(QWheelEvent* event) {
    double factor = (event->angleDelta().y() > 0) ? 1.15 : 0.87;
    scale(factor, factor);
}

void MapCanvas::removeLastWaypoint() {
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

void MapCanvas::clearAll() {
    while (!m_markers.empty())
        removeLastWaypoint();
}

int MapCanvas::waypointCount() const {
    return static_cast<int>(m_markers.size());
}

void MapCanvas::redrawLines() {
    for (auto* l : m_lines) { m_scene->removeItem(l); delete l; }
    m_lines.clear();

    for (size_t i = 0; i + 1 < m_markers.size(); ++i) {
        QPointF a = m_markers[i]->rect().center();
        QPointF b = m_markers[i+1]->rect().center();
        auto* line = m_scene->addLine(a.x(), a.y(), b.x(), b.y(),
                                      QPen(QColor(255, 210, 60), 1.5, Qt::DashLine));
        line->setZValue(1);
        m_lines.push_back(line);
    }
}

void MapCanvas::refreshColors() {
    if (m_markers.empty()) return;
    for (size_t i = 0; i < m_markers.size(); ++i) {
        QColor c;
        if      (i == 0)                        c = QColor(Qt::green);
        else if (i == m_markers.size() - 1)     c = QColor(Qt::red);
        else                                     c = QColor(Qt::blue);
        m_markers[i]->setBrush(QBrush(c));
    }
}