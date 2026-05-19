#include "MissionManager.h"
#include <cmath>

void MissionManager::addWaypoint(double x, double y) {
    m_points.emplace_back(x, y);
}

void MissionManager::removeLastWaypoint() {
    if (!m_points.empty())
        m_points.pop_back();
}

void MissionManager::clear() {
    m_points.clear();
}

const std::vector<std::pair<double,double>>& MissionManager::rawPoints() const {
    return m_points;
}

// [SDD-010 , SDD-011 ]
bool MissionManager::isValid() const {
    if (m_points.size() < 2)
        return false;

    for (size_t i = 0; i + 1 < m_points.size(); ++i) {
        double dx = m_points[i].first  - m_points[i+1].first;
        double dy = m_points[i].second - m_points[i+1].second;
        if (std::sqrt(dx*dx + dy*dy) < DUPLICATE_THRESHOLD)  // check if the same point
            return false;
    }
    return true;
}

// [SDD-014]
QString MissionManager::validationError() const {
    if (m_points.size() < 2)
        return QString("Need at least 2 waypoints, have %1.").arg(m_points.size());

    for (size_t i = 0; i + 1 < m_points.size(); ++i) {
        double dx = m_points[i].first  - m_points[i+1].first;
        double dy = m_points[i].second - m_points[i+1].second;
        if (std::sqrt(dx*dx + dy*dy) < DUPLICATE_THRESHOLD)  // check if the same point
            return QString("Waypoints %1 and %2 are too close together.").arg(i).arg(i+1);
    }
    return {};
}

// [SDD-012, SDD-013]
std::vector<Waypoint> MissionManager::buildTrajectory() const {
    std::vector<Waypoint> traj;
    if (m_points.empty()) return traj;

    int idx = 0;

    // TAKEOFF at first point, ground level
    traj.emplace_back(idx++, WaypointType::TAKEOFF,
                      m_points.front().first, m_points.front().second);

    // All user points become CRUISE at 50m
    for (const auto& p : m_points)
        traj.emplace_back(idx++, WaypointType::CRUISE, p.first, p.second);

    // LAND at last point, ground level
    traj.emplace_back(idx++, WaypointType::LAND,
                      m_points.back().first, m_points.back().second);

    return traj;
}
