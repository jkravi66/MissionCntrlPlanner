#ifndef MISSIONMANAGER_H
#define MISSIONMANAGER_H

#include "Waypoint.h"
#include <vector>
#include <QString>


// check the treacebality
class MissionManager
{
public:
    MissionManager() = default;

    void addWaypoint(double x, double y);
    void removeLastWaypoint();
    void clear();

    const std::vector<std::pair<double,double>>& rawPoints() const;

    bool    isValid() const;                        // [SDD-010 , SDD-011 ]
    QString validationError() const;                // [SDD-014]

    std::vector<Waypoint> buildTrajectory() const;  // [SDD-012, SDD-013]

    static constexpr double DUPLICATE_THRESHOLD = 0.001;   // [SDD-011]

private:
    std::vector<std::pair<double,double>> m_points;
};

#endif // MISSIONMANAGER_H
