#ifndef MISSIONMANAGER_H
#define MISSIONMANAGER_H

#include "Waypoint.h"
#include <vector>
#include <QString>

// MissionManager owns the raw user-placed waypoint list and provides
// the domain-logic operations: validation, trajectory building, and error reporting.
class MissionManager
{
public:
    MissionManager() = default;

    // [SRD-010] Store a new user-placed waypoint at (x, y).
    void addWaypoint(double x, double y);

    // [SRD-005] Remove the most recently placed waypoint.
    void removeLastWaypoint();

    // [SRD-006] Remove all waypoints.
    void clear();

    // Read-only access to the raw point list (used by the UI for display).
    const std::vector<std::pair<double,double>>& rawPoints() const;

    bool    isValid() const;                              // [SDD-010, SDD-011]

    QString validationError() const;                      // [SDD-014] → SRD-022

    // Expands the raw points into a full typed trajectory:
    std::vector<Waypoint> buildTrajectory() const;        // [SDD-012, SDD-013]


    static constexpr double DUPLICATE_THRESHOLD = 0.001;  // [SDD-011] → SRD-015

private:
    std::vector<std::pair<double,double>> m_points;
};

#endif // MISSIONMANAGER_H
