#ifndef MAPENGINE_H
#define MAPENGINE_H


#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QPointF>
#include <vector>

class MapEngine : public QGraphicsView {
    Q_OBJECT

public:
    explicit MapEngine(QWidget* parent = nullptr);

    void removeLastWaypoint();
    void clearAll();
    int  waypointCount() const;

signals:
    void waypointAdded(QPointF pos);
    void waypointRemoved();

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

private:
    void drawGrid();
    void redrawLines();
    void refreshColors();

    QGraphicsScene*                    m_scene;
    std::vector<QGraphicsEllipseItem*> m_markers;
    std::vector<QGraphicsLineItem*>    m_lines;

    static constexpr double MARKER_R = 6.0;
};
#endif // MAPENGINE_H
