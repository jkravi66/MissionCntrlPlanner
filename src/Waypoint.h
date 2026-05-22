#ifndef WAYPOINT_H
#define WAYPOINT_H

// SDD-002
// WaypointType enum defines the three mission phases for a VTOL trajectory.
enum class WaypointType {
    TAKEOFF,
    CRUISE,
    LAND
};

// SDD-002
inline const char* typeToString(WaypointType t) {
    switch(t) {
    case WaypointType::TAKEOFF: return "TAKEOFF";
    case WaypointType::CRUISE:  return "CRUISE";
    case WaypointType::LAND:    return "LAND";
	default:                    return "UNKNOWN";
    }
}

// SDD-001
// SDD-003 CRUISE altitude initialised to 50.0 m
// SDD-004 TAKEOFF altitude initialised to 0.0 m (ground-level departure)
// SDD-005 LAND altitude initialised to 0.0 m (ground-level arrival)
struct Waypoint {
    int          index;
    WaypointType type;
    double       x;
    double       y;
    double       altitude;

    // SDD-003, SDD-004, SDD-005
    static double defaultAltitude(WaypointType t) {
        return (t == WaypointType::CRUISE) ? 50.0 : 0.0;
    }

    Waypoint(int i, WaypointType t, double px, double py)
        : index(i), type(t), x(px), y(py), altitude(defaultAltitude(t))
    {}
};

#endif // WAYPOINT_H