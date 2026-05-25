# Mission Planner — Requirements
**Project:** Mission Planner for Avilus  
**Author:** Ravi Jayanagara Krishnegowda  
**Revision:** A — 22 May 2026 — Initial release

---

## 1. About This Document

### 1.1 Purpose and Scope

This document defines the functional and non-functional requirements for the Mission Planner desktop application. Mission Planner is a Qt6/C++ desktop application that allows operators to interactively plan UAV flight missions on a 2D map. Users may place, remove, validate, and upload ordered waypoint sequences that are automatically expanded into typed trajectory points (TAKEOFF, CRUISE, LAND) with a fixed cruise altitude of 50 metres AGL.

### 1.2 Applicable Documents

| Document No. | Document Title |
|---|---|
| RTCA DO-178C / EUROCAE ED-12C | Software Considerations in Airborne Systems and Equipment Certification |
| EU Regulation 2019/947 | Rules and Procedures for UAV Operations |

### 1.3 Abbreviations and Acronyms

| Acronym | Meaning |
|---|---|
| 2D | Two-Dimensional |
| AGL | Above Ground Level |
| GUI | Graphical User Interface |
| N/A | Not Applicable |
| SDD | Software Design Description |
| SRD | Software Requirements Data |
| SYS | System Level Requirement |
| UAV | Unmanned Aerial Vehicle |
| VTOL | Vertical Take-Off and Landing |

### 1.4 Terms and Definitions

**Shall** — Expresses a binding requirement.

### 1.5 System Context

Mission Planner is a desktop tool. It runs on any Qt6-supported OS (Windows, Linux, macOS). The application has three parts: a main window shell, an interactive map widget, and a mission-data model.

### 1.6 Operating Environment

- Qt 6.x
- C++17 compiler
- Qt Widgets and Qt SVG modules
- Screen resolution: minimum 1400 × 650 logical pixels

---

## 2. Requirements

This section specifies the requirements for a minimal 2D UAV mission planning application. The application shall enable an operator to define and manage a flight path on a 2D map interface. The system shall support basic VTOL mission phases using a fixed cruise altitude of 50 metres AGL.

---

### 2.1 System Level Requirements

| Requirement ID | Description |
|---|---|
| SYS-001 | The system shall allow an operator to define a flight mission between two geographic points on a 2D map. |
| SYS-002 | The system shall model VTOL trajectory phases: vertical takeoff, horizontal cruise, vertical landing. |
| SYS-003 | The system shall assign a fixed cruise altitude of 50 metres AGL to all cruise-phase waypoints. |
| SYS-004 | The system shall validate a mission before uploading and reject invalid configurations. |
| SYS-005 | The system shall output planned waypoint coordinates upon mission upload. |
| SYS-006 | The system shall provide a 2D graphical interface for interactive mission planning. |
| SYS-007 | The system shall maintain bidirectional requirements traceability between requirements and code. |
| SYS-008 | The system shall include automated tests executable in a headless (no-display) environment. |
| SYS-009 | The system shall provide a Qt6-based GUI. |

---

### 2.2 Software Requirements Data (SRD)

#### 2.2.1 Map and Display

| Requirement ID | Traces To | Description |
|---|---|---|
| SRD-001 | SYS-001, SYS-006 | The map subsystem shall render a 2D coordinate canvas with a grid. |
| SRD-002 | SYS-001 | The map subsystem shall support click-to-place waypoint interaction. |
| SRD-003 | SYS-006 | The map subsystem shall draw trajectory lines connecting consecutive waypoints. |
| SRD-004 | SYS-006 | The start waypoint shall be visually distinct (green), the end waypoint red, intermediates blue. |
| SRD-005 | SYS-006 | The map subsystem shall support removal of the last placed waypoint. |
| SRD-006 | SYS-006 | The map subsystem shall support removal of all waypoints. |
| SRD-007 | SYS-006 | The map subsystem shall display the status of waypoint selection in the status bar. |

#### 2.2.2 Mission Data

| Requirement ID | Traces To | Description |
|---|---|---|
| SRD-010 | SYS-001 | The mission subsystem shall store each waypoint as: x, y, altitude, type. |
| SRD-011 | SYS-002 | The mission subsystem shall prepend a TAKEOFF waypoint at the origin coordinates. |
| SRD-012 | SYS-002 | The mission subsystem shall append a LAND waypoint at the destination coordinates. |
| SRD-013 | SYS-003 | The mission subsystem shall assign altitude = 50.0 m to all CRUISE waypoints. |
| SRD-014 | SYS-004 | The mission subsystem shall require a minimum of 2 user-placed waypoints. |
| SRD-015 | SYS-004 | The mission subsystem shall reject missions where any consecutive waypoints are separated by a Euclidean distance of less than 0.001 coordinate units. |

#### 2.2.3 Mission Upload

| Requirement ID | Traces To | Description |
|---|---|---|
| SRD-020 | SYS-005 | The upload subsystem shall print each waypoint's index, type, x, y, and altitude. |
| SRD-021 | SYS-004 | The upload subsystem shall validate the mission before output and abort with an error message if invalid. |
| SRD-022 | SYS-005 | Waypoints shall be output in mission execution order: TAKEOFF → CRUISE(s) → LAND. |

---

### 2.3 Software Design Description (SDD)

#### 2.3.1 Waypoint Data Model

| Requirement ID | Traces To | Description |
|---|---|---|
| SDD-001 | SRD-010 | `Waypoint` shall be a struct with fields: index, type, x, y, altitude. |
| SDD-002 | SRD-011, SRD-012 | `WaypointType` shall be an enum class with values: TAKEOFF, CRUISE, LAND. |
| SDD-003 | SRD-013 | CRUISE waypoints shall initialise altitude to 50.0 on construction. |
| SDD-004 | SRD-011 | TAKEOFF waypoints shall carry altitude = 0.0 (ground-level departure). |
| SDD-005 | SRD-012 | LAND waypoints shall carry altitude = 0.0 (ground-level arrival). |

#### 2.3.2 Mission Manager

| Requirement ID | Traces To | Description |
|---|---|---|
| SDD-010 | SRD-014 | `isValid()` shall return false if the user waypoint count is less than 2. |
| SDD-011 | SRD-015 | `isValid()` shall return false if any consecutive waypoints are within 0.001 units. |
| SDD-012 | SRD-011 | `buildTrajectory()` shall insert a TAKEOFF waypoint at index 0 using the first user waypoint coordinates. |
| SDD-013 | SRD-012 | `buildTrajectory()` shall append a LAND waypoint at the last index using the last user waypoint coordinates. |
| SDD-014 | SRD-014, SRD-015 | `validationError()` shall return a human-readable description of why `isValid()` returned false, or an empty string when valid. |

#### 2.3.3 Map Engine

| Requirement ID | Traces To | Description |
|---|---|---|
| SDD-020 | SRD-001 | `MapEngine` shall extend `QGraphicsView` and own a `QGraphicsScene` rendered with a background grid. |
| SDD-021 | SRD-002 | `mousePressEvent()` shall convert screen coordinates to scene coordinates and emit `waypointAdded(QPointF)`. `wheelEvent()` shall support scroll-wheel zoom. |
| SDD-022 | SRD-003 | The engine shall render a `QGraphicsLineItem` between each consecutive waypoint pair. |
| SDD-023 | SRD-004 | First marker = green, last marker = red, intermediate markers = blue. |
| SDD-024 | SRD-005 | `removeLastWaypoint()` shall remove the last marker and redraw trajectory lines, then emit `waypointRemoved()`. |
| SDD-025 | SRD-006 | `clearAll()` shall remove all markers and all trajectory lines. |

#### 2.3.4 Upload Data

| Requirement ID | Traces To | Description |
|---|---|---|
| SDD-030 | SRD-020 | Output format per waypoint: `[IDX] TYPE  x=<val>  y=<val>  alt=<val>m` printed in the Mission Log panel. |
| SDD-031 | SRD-021 | If `isValid()` is false, `onUpload()` shall display the failure reason in red and return immediately without printing waypoints. |
| SDD-032 | SRD-020 | The subsystem shall print a "Mission Upload" header and a total-waypoints-uploaded footer. |

#### 2.3.5 Toolbar

| Requirement ID | Traces To | Description |
|---|---|---|
| SDD-041 | SRD-020 | The toolbar shall provide an "Upload Mission" action that triggers mission validation and upload. |
| SDD-042 | SRD-005 | The toolbar shall provide a "Remove Last" action that removes the most recently placed waypoint. |
| SDD-043 | SRD-006 | The toolbar shall provide a "Clear All" action that removes all waypoints and resets the mission. |

#### 2.3.6 Status Bar

| Requirement ID | Traces To | Description |
|---|---|---|
| SDD-051 | SRD-007 | The status bar shall display "Click the map to place waypoints." when no waypoints are present. |
| SDD-052 | SRD-007 | The status bar shall display the waypoint count and an error message when the mission is invalid. |
| SDD-053 | SRD-007 | The status bar shall display the waypoint count and "Mission valid. Ready to upload." when the mission is valid. |
| SDD-054 | SRD-007 | The status bar shall update after every add, remove, or clear operation. |

---

## 3. Test Requirements

All tests are implemented using the Qt Test framework in `missionPlanTest/tst_testmissionplanner.cpp`. Tests cover the `MissionManager` and `Waypoint` domain logic and are headless (no display required), satisfying SYS-008.

Tests that require a live Qt display (MapEngine, MainWindow GUI behaviour) are identified as GUI tests and are not included in the automated headless suite. They would be covered by a separate system-level GUI test pass.

| Test ID | Traces To | Type | Description |
|---|---|---|---|
| TEST-001 | SDD-010, SRD-014 | Nominal | Verify `isValid()` returns false when there are 0 waypoints. |
| TEST-002 | SDD-010, SRD-014 | Nominal | Verify `isValid()` returns false when there is exactly 1 waypoint. |
| TEST-003 | SDD-010 | Robust | Verify `isValid()` returns true for a valid 2-waypoint mission. |
| TEST-004 | SDD-011, SRD-015 | Nominal | Verify `isValid()` returns false when consecutive waypoints are identical. |
| TEST-005 | SDD-011, SRD-015 | Nominal | Verify `isValid()` returns false when consecutive waypoints are within 0.001 units. |
| TEST-006 | SDD-011, SRD-015 | Robust | Verify `isValid()` returns true when waypoint x-distance is exactly at the duplicate threshold (boundary — accepted by `<` check). |
| TEST-007 | SDD-011, SRD-015 | Robust | Verify `isValid()` returns true when waypoint x-distance is just above the threshold (+0.0001). |
| TEST-008 | SDD-011, SRD-015 | Robust | Verify `isValid()` returns false when waypoint x-distance is just below the threshold (−0.0001). |
| TEST-009 | SDD-011, SRD-015 | Robust | Verify `isValid()` returns true when waypoint y-distance is exactly at the duplicate threshold. |
| TEST-010 | SDD-011, SRD-015 | Robust | Verify `isValid()` returns true when waypoint y-distance is just above the threshold (+0.0001). |
| TEST-011 | SDD-011, SRD-015 | Robust | Verify `isValid()` returns false when waypoint y-distance is just below the threshold (−0.0001). |
| TEST-012 | SDD-003 | Nominal | Verify `Waypoint::defaultAltitude(CRUISE)` returns 50.0. |
| TEST-013 | SDD-004, SDD-005 | Nominal | Verify `Waypoint::defaultAltitude(TAKEOFF)` and `(LAND)` both return 0.0. |
| TEST-014 | SDD-012, SRD-011 | Nominal | Verify `buildTrajectory()` places TAKEOFF at index 0 with the first raw-point coordinates. |
| TEST-015 | SDD-013, SRD-012 | Nominal | Verify `buildTrajectory()` appends LAND at the last index with the last raw-point coordinates. |
| TEST-016 | SDD-012, SDD-013, SRD-022 | Nominal | Verify `buildTrajectory()` output order: TAKEOFF → CRUISE(s) → LAND. |
| TEST-017 | SDD-004, SRD-011 | Nominal | Verify the TAKEOFF waypoint carries altitude = 0.0 m. |
| TEST-018 | SDD-005, SRD-012 | Nominal | Verify the LAND waypoint carries altitude = 0.0 m. |
| TEST-019 | SDD-003, SRD-013 | Nominal | Verify all CRUISE waypoints carry altitude = 50.0 m. |
| TEST-020 | SDD-014 | Nominal | Verify `validationError()` returns a non-empty string for too few waypoints. |
| TEST-021 | SDD-014 | Nominal | Verify `validationError()` returns a non-empty string for duplicate waypoints. |
| TEST-022 | SDD-014 | Nominal | Verify `validationError()` returns an empty string when the mission is valid. |
| TEST-023 | SRD-010 | Nominal | Verify `addWaypoint()` increases `rawPoints()` count by 1. |
| TEST-024 | SRD-005, SDD-024 | Nominal | Verify `removeLastWaypoint()` decreases `rawPoints()` count by 1. |
| TEST-025 | SRD-006, SDD-025 | Nominal | Verify `clear()` empties the mission to 0 waypoints. |
| TEST-026 | SRD-005, SDD-024 | Robust | Verify `removeLastWaypoint()` on an empty list is a no-op and does not crash or alter the count. |
| TEST-027 | SDD-012, SDD-013, SRD-011, SRD-012, SRD-014 | Nominal | Verify `buildTrajectory()` on a minimum valid mission (2 raw points) produces exactly 4 waypoints: TAKEOFF + CRUISE + CRUISE + LAND. |
| TEST-028 | SDD-012, SDD-013, SRD-011, SRD-012 | Nominal | Verify TAKEOFF is co-located with CRUISE[0] and LAND is co-located with the last CRUISE (intended VTOL vertical ascent/descent in place). |
| TEST-029 | SRD-010, SDD-001, SDD-012, SDD-013 | Robust | Verify negative x/y coordinates are stored and propagated unchanged through TAKEOFF, CRUISE, and LAND waypoints. |
