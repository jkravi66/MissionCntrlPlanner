#ifndef WAYPOINT_H
#define WAYPOINT_H

#pragma once

// [SDD-002]
enum class WaypointType {
    TAKEOFF,
    CRUISE,
    LAND
};

// [SDD-002]
inline const char* typeToString(WaypointType t) {
    switch(t) {
    case WaypointType::TAKEOFF: return "TAKEOFF";
    case WaypointType::CRUISE:  return "CRUISE";
    case WaypointType::LAND:    return "LAND";
    }
    return "UNKNOWN";
}

// [SDD-001, SDD-003, SDD-004, SDD-005]
struct Waypoint {
    int          index;
    WaypointType type;
    double       x;
    double       y;
    double       altitude;

    static double defaultAltitude(WaypointType t) {
        return (t == WaypointType::CRUISE) ? 50.0 : 0.0;
    }

    Waypoint(int i, WaypointType t, double px, double py)
        : index(i), type(t), x(px), y(py), altitude(defaultAltitude(t))
    {}
};

#endif // WAYPOINT_H
