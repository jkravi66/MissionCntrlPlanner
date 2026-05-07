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

