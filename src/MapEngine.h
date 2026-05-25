#ifndef MAPENGINE_H
#define MAPENGINE_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QPointF>
#include <vector>

// MapEngine
class MapEngine : public QGraphicsView {
    Q_OBJECT

public:
    explicit MapEngine(QWidget* parent = nullptr);

    void removeLastWaypoint();   // SDD-024
    void clearAll();             // SDD-025

signals:
    void waypointAdded(QPointF pos);
    void waypointRemoved();

protected:
    void mousePressEvent(QMouseEvent* event) override;  // SDD-021, SDD-022
    void wheelEvent(QWheelEvent* event) override;       // SDD-021

private:
    void drawGrid();            // SDD-020
    void refreshColors();       // SDD-023

    QGraphicsScene*                    m_scene;
    std::vector<QGraphicsEllipseItem*> m_markers;
    std::vector<QGraphicsLineItem*>    m_lines;

    static constexpr double MARKER_R = 6.0;
};
#endif // MAPENGINE_H
