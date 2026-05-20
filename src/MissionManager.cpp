#include "MissionManager.h"
#include <cmath>

// ---------------------------------------------------------------------------
// addWaypoint [SRD-010] Store a new user-placed waypoint.
// ---------------------------------------------------------------------------
void MissionManager::addWaypoint(double x, double y) {
    m_points.emplace_back(x, y);
}

// ---------------------------------------------------------------------------
// removeLastWaypoint [SRD-005] Remove the most recently placed waypoint.
// ---------------------------------------------------------------------------
void MissionManager::removeLastWaypoint() {
    if (!m_points.empty())
        m_points.pop_back();
}

// ---------------------------------------------------------------------------
// clear [SRD-006] Remove all waypoints.
// ---------------------------------------------------------------------------
void MissionManager::clear() {
    m_points.clear();
}

const std::vector<std::pair<double,double>>& MissionManager::rawPoints() const {
    return m_points;
}

// ---------------------------------------------------------------------------
// [SDD-010] → SRD-014  Reject if fewer than 2 waypoints.
// [SDD-011] → SRD-015  Reject if any consecutive pair is within DUPLICATE_THRESHOLD.
// ---------------------------------------------------------------------------
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

// ---------------------------------------------------------------------------
// [SDD-014] → SRD-022  log the error message
// Returns a error message of why isValid() returned false,
// or an empty string when the mission is valid.
// ---------------------------------------------------------------------------
QString MissionManager::validationError() const {
    if (m_points.size() < 2)
        return QString("Need at least 2 waypoints, have %1.").arg(m_points.size());

    for (size_t i = 0; i + 1 < m_points.size(); ++i) {
        double dx = m_points[i].first  - m_points[i+1].first;
        double dy = m_points[i].second - m_points[i+1].second;
        if (std::sqrt(dx*dx + dy*dy) < DUPLICATE_THRESHOLD)
            return QString("Waypoints %1 and %2 are too close together.").arg(i).arg(i+1);
    }
    return {};
}

// ---------------------------------------------------------------------------
// [SDD-012] → SRD-011  Insert TAKEOFF at index 0 using the first raw-point coordinates.
// [SDD-013] → SRD-012  Append LAND at the last index using the last raw-point coordinates.
// All intermediate raw points become CRUISE waypoints at altitude = 50 m (SDD-003 / SRD-013).
// ---------------------------------------------------------------------------
std::vector<Waypoint> MissionManager::buildTrajectory() const {
    std::vector<Waypoint> traj;
    if (m_points.empty()) return traj;

    int idx = 0;

    // [SDD-012] TAKEOFF at first point, ground level (alt = 0.0)
    traj.emplace_back(idx++, WaypointType::TAKEOFF,
                      m_points.front().first, m_points.front().second);

    // [SDD-003] All user points become CRUISE waypoints at 50 m AGL
    for (const auto& p : m_points)
        traj.emplace_back(idx++, WaypointType::CRUISE, p.first, p.second);

    // [SDD-013] LAND at last point, ground level (alt = 0.0)
    traj.emplace_back(idx++, WaypointType::LAND,
                      m_points.back().first, m_points.back().second);

    return traj;
}

// ---------------------------------------------------------------------------