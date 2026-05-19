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

    bool    isValid() const;           // [LLR-010, LLR-011]
    QString validationError() const;

    std::vector<Waypoint> buildTrajectory() const;  // [LLR-012, LLR-013]

    static constexpr double DUPLICATE_THRESHOLD = 0.001;

private:
    std::vector<std::pair<double,double>> m_points;
};

#endif // MISSIONMANAGER_H
