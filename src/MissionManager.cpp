#include "MissionManager.h"
#include <cmath>
#include <iostream>
#include <iomanip>

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


// [LLR-010, LLR-011]
bool MissionManager::isValid() const {
    if (m_points.size() < 2)
        return false;

    for (size_t i = 0; i + 1 < m_points.size(); ++i) {
        double dx = m_points[i].first  - m_points[i+1].first;
        double dy = m_points[i].second - m_points[i+1].second;
        if (std::sqrt(dx*dx + dy*dy) < DUPLICATE_THRESHOLD)
            return false;
    }
    return true;
}

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
