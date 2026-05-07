# Requirements – Mission Planner

## System Level

SYS-001 – Operator can place waypoints on a 2D map by clicking  
SYS-002 – VTOL phases: takeoff vertical, cruise horizontal at 50m, land vertical  
SYS-003 – Fixed cruise altitude is 50 metres AGL  
SYS-004 – Mission must be validated before upload  
SYS-005 – On upload, all waypoints printed to stdout  
SYS-006 – Application has a GUI with a map canvas  
SYS-007 – Every requirement is traceable to code and to a test  
SYS-008 – Tests run headless (no monitor required)  

## Low Level

LLR-001 – Waypoint is a struct: index, type, x, y, altitude  
LLR-002 – WaypointType enum: TAKEOFF, CRUISE, LAND  
LLR-003 – CRUISE altitude = 50.0, TAKEOFF and LAND = 0.0  
LLR-010 – isValid() returns false if fewer than 2 waypoints  
LLR-011 – isValid() returns false if consecutive points overlap  
LLR-012 – buildTrajectory() first item is always TAKEOFF  
LLR-013 – buildTrajectory() last item is always LAND  
LLR-020 – MapCanvas extends QGraphicsView  
LLR-021 – Click on canvas emits waypointAdded signal  
LLR-040 – Tests use QT_QPA_PLATFORM=offscreen