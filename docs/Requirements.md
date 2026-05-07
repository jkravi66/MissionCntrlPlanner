# Software Requirements Specification
# Mission Planner – 2D

Version: 1.0
Date: 07-05-2026
Status: --

## 1. Purpose

This defines the requirements for a minimal 2D UAV mission planning application. The system allows an operator to plan a flight path between two points on a 2D map canvas, supporting VTOL (Vertical Take-Off and Landing) trajectory phases at a fixed cruise altitude of 50m.

## 2. System Requirements (SYS)

| ID      | Requirement
|---------|---------------------------------------------------------------------------------------------------
| SYS-001 | The system shall allow an operator to define a flight mission between two geographic points on a 2D map
| SYS-002 | The system shall model VTOL trajectory phases: vertical takeoff, horizontal cruise, vertical landing
| SYS-003 | The system shall assign a fixed cruise altitude of 50 metres AGL to all cruise-phase waypoints
| SYS-004 | The system shall validate a mission before upload and reject invalid configurations
| SYS-005 | The system shall output planned waypoint coordinates to stdout upon mission upload
| SYS-006 | The system shall provide a 2D graphical interface for interactive mission planning
| SYS-007 | The system shall maintain bidirectional requirements traceability between requirements and code
| SYS-008 | The system shall include automated tests executable in a headless (no-display) environment
| SYS-009 | The system shall provide a Qt6-based interface (GUI).
| SYS-010 | The system shall be maintained using Git.

## 3. Software Requirements Document (SRD)

### 3.1 Map & Display

| ID      | Parent          | Requirement
|---------|-----------------|------------------------------------------------------------------------------------------------
| SRD-001 | SYS-001,SYS-006 | The map subsystem shall render a 2D coordinate canvas with a visible grid
| SRD-002 | SYS-001         | The map subsystem shall support click-to-place waypoint interaction
| SRD-003 | SYS-006         | The map subsystem shall draw trajectory lines connecting consecutive waypoints
| SRD-004 | SYS-006         | The start waypoint shall be visually distinct (green), the end waypoint red, intermediates blue
| SRD-005 | SYS-006         | The map subsystem shall support removal of the last placed waypoint
| SRD-005 | SYS-006         | The map subsystem shall support removal of the all waypoints

### 3.2 Mission Data

| ID      | Parent          | Requirement
|---------|-----------------|------------------------------------------------------------------------------------------------
| SRD-010 | SYS-001         | The mission subsystem shall store each waypoint as (x, y, altitude, type)
| SRD-011 | SYS-002         | The mission subsystem shall prepend a TAKEOFF waypoint at origin coordinates
| SRD-012 | SYS-002         | The mission subsystem shall append a LAND waypoint at destination coordinates
| SRD-013 | SYS-003         | The mission subsystem shall assign altitude = 50.0m to all CRUISE waypoints
| SRD-014 | SYS-004         | The mission subsystem shall require a minimum of 2 user-placed waypoints
| SRD-015 | SYS-004         | The mission subsystem shall reject missions where consecutive waypoints are at identical positions

### 3.3 Mission Upload

| ID      | Parent          | Requirement
|---------|-----------------|------------------------------------------------------------------------------------------------
| SRD-020 | SYS-005         | The upload subsystem shall print each waypoint index, type, x, y, and altitude
| SRD-021 | SYS-004         | The upload subsystem shall validate the mission before output and abort with an error if invalid
| SRD-022 | SYS-005         | Waypoints shall be output in mission execution order: TAKEOFF → CRUISE(s) → LAND

## 4. Software Design Description (SDD)

### 4.1 Waypoint Data Model

| ID      | Parent          | Requirement
|---------|-----------------|------------------------------------------------------------------------------------------------
| SDD-001 | SRD-010         | `Waypoint` shall be a struct with fields: index (int), type (WaypointType), x (double), y (double), altitude (double)
| SDD-002 | SRD-011,SRD-012 | `WaypointType` shall be an enum class with values: TAKEOFF, CRUISE, LAND
| SDD-003 | SRD-013         | CRUISE waypoints shall initialise altitude to 50.0 on construction
| SDD-004 | SRD-011         | TAKEOFF waypoints shall carry altitude = 0.0 (ground-level departure)
| SDD-005 | SRD-012         | LAND waypoints shall carry altitude = 0.0 (ground-level arrival)

### 4.2 Mission Manager

| ID      | Parent          | Requirement
|---------|-----------------|------------------------------------------------------------------------------------------------
| SDD-010 | SRD-014         | `isValid()` shall return false if the user waypoint count is less than 2
| SDD-011 | SRD-015         | `isValid()` shall return false if any consecutive waypoints are within 0.001 units
| SDD-012 | SRD-011         | `buildTrajectory()` shall insert TAKEOFF at index 0 using the first user waypoint coordinates
| SDD-013 | SRD-012         | `buildTrajectory()` shall append LAND at the last index using the last user waypoint coordinates
| SDD-014 | SRD-022         | `uploadMission()` shall call buildTrajectory() and print each waypoint in order

### 4.3 Map Canvas

| ID      | Parent          | Requirement
|---------|-----------------|------------------------------------------------------------------------------------------------
| SDD-020 | SRD-001         | `MapCanvas` shall extend QGraphicsView and own a QGraphicsScene
| SDD-021 | SRD-002         | `mousePressEvent()` shall convert screen coords to scene coords and emit waypointAdded(QPointF)
| SDD-022 | SRD-003         | `MapCanvas` shall render a QGraphicsLineItem between each consecutive waypoint pair
| SDD-023 | SRD-004         | First marker = green, last marker = red, intermediate markers = blue
| SDD-024 | SRD-005         | `removeLastWaypoint()` shall remove the last marker and redraw trajectory lines
| SDD-025 | SRD-006         | `clearAll()` shall remove all marker and remove trajectory lines

### 4.4 Upload Output

| ID      | Parent          | Requirement
|---------|-----------------|------------------------------------------------------------------------------------------------
| SDD-030 | SRD-020         | Output format: `[IDX] TYPE  x=<val>  y=<val>  alt=<val>m` printed via std::cout
| SDD-031 | SRD-021         | If isValid() is false, uploadMission() shall print the failure reason and return immediately


## 5 Tests

| ID      | Parent          | Requirement
|---------|-----------------|------------------------------------------------------------------------------------------------
 - Add test requirements
 - add tracebality 