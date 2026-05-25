# Mission Planner — Bidirectional Requirements Traceability Matrix
**Project:** Mission Planner for Avilus  
**Revision:** A — 22 May 2026

Bidirectional mapping: Requirement ↔ Implementation ↔ Test.

- **Forward traceability** — start from a requirement ID, find where it is implemented and which test verifies it.
- **Backward traceability** — start from a source file or function, find which requirements it satisfies.

> **Test status key:**  
> `TEST-NNN` — automated headless Qt Test  
> `GUI` — requires a live display; covered by manual system-level test pass  
> `code audit` — verified by inspection, no runtime test applicable

---

## 1. Forward Traceability (Requirement → Code → Test)

### 1.1 System Level Requirements

| Req ID | Description (summary) | Implemented In | Verified By |
|---|---|---|---|
| SYS-001 | Operator defines mission between two points on 2D map | `MissionManager::addWaypoint()`, `MapEngine::mousePressEvent()` | TEST-023 |
| SYS-002 | Model VTOL phases: takeoff, cruise, land | `MissionManager::buildTrajectory()` | TEST-016 |
| SYS-003 | Fixed 50 m AGL cruise altitude | `Waypoint::defaultAltitude()`, `buildTrajectory()` | TEST-012, TEST-013, TEST-019 |
| SYS-004 | Validate mission before upload; reject invalid | `MissionManager::isValid()`, `validationError()` | TEST-001 to TEST-011 |
| SYS-005 | Output waypoint coordinates on upload | `MainWindow::onUpload()` | GUI |
| SYS-006 | 2D graphical interface | `MapEngine` (entire class) | GUI |
| SYS-007 | Bidirectional traceability maintained | Requirement tags in all source files | code audit |
| SYS-008 | Automated tests in headless environment | `missionPlanTest/tst_testmissionplanner.cpp` | TEST-001 to TEST-029 |
| SYS-009 | Qt6-based GUI | `CMakeLists.txt`, `main.cpp` | GUI |

### 1.2 Software Requirements Data (SRD)

| Req ID | Traces To | Implemented In | Verified By |
|---|---|---|---|
| SRD-001 | SYS-001, SYS-006 | `MapEngine::MapEngine()`, `MapEngine::drawGrid()` → SDD-020 | GUI |
| SRD-002 | SYS-001 | `MapEngine::mousePressEvent()` → SDD-021 | GUI |
| SRD-003 | SYS-006 | `MapEngine::mousePressEvent()` (line rendering) → SDD-022 | GUI |
| SRD-004 | SYS-006 | `MapEngine::refreshColors()` → SDD-023 | GUI |
| SRD-005 | SYS-006 | `MapEngine::removeLastWaypoint()` → SDD-024 | TEST-024, TEST-026 |
| SRD-006 | SYS-006 | `MapEngine::clearAll()` → SDD-025, `MissionManager::clear()` | TEST-025 |
| SRD-007 | SYS-006 | `MainWindow::updateStatus()` → SDD-051, SDD-052, SDD-053, SDD-054 | GUI |
| SRD-010 | SYS-001 | `Waypoint` struct → SDD-001 | TEST-023, TEST-029 |
| SRD-011 | SYS-002 | `MissionManager::buildTrajectory()` → SDD-012 | TEST-014, TEST-017, TEST-027, TEST-028 |
| SRD-012 | SYS-002 | `MissionManager::buildTrajectory()` → SDD-013 | TEST-015, TEST-018, TEST-027, TEST-028 |
| SRD-013 | SYS-003 | `Waypoint::defaultAltitude()` → SDD-003 | TEST-019 |
| SRD-014 | SYS-004 | `MissionManager::isValid()` → SDD-010 | TEST-001, TEST-002, TEST-027 |
| SRD-015 | SYS-004 | `MissionManager::isValid()` → SDD-011 | TEST-004 to TEST-011 |
| SRD-020 | SYS-005 | `MainWindow::onUpload()` → SDD-030, SDD-032 | GUI |
| SRD-021 | SYS-004 | `MainWindow::onUpload()` → SDD-031 | GUI |
| SRD-022 | SYS-005 | `MissionManager::buildTrajectory()` → SDD-014 | TEST-016 |

### 1.3 Software Design Description (SDD)

| Req ID | Traces To | Implemented In | Verified By |
|---|---|---|---|
| SDD-001 | SRD-010 | `Waypoint.h` — `struct Waypoint` | TEST-029 |
| SDD-002 | SRD-011, SRD-012 | `Waypoint.h` — `enum class WaypointType` | TEST-012 to TEST-019 |
| SDD-003 | SRD-013 | `Waypoint::defaultAltitude(CRUISE)` returns 50.0 | TEST-012, TEST-019 |
| SDD-004 | SRD-011 | `Waypoint::defaultAltitude(TAKEOFF)` returns 0.0 | TEST-013, TEST-017 |
| SDD-005 | SRD-012 | `Waypoint::defaultAltitude(LAND)` returns 0.0 | TEST-013, TEST-018 |
| SDD-010 | SRD-014 | `MissionManager::isValid()` — size < 2 guard | TEST-001, TEST-002, TEST-003 |
| SDD-011 | SRD-015 | `MissionManager::isValid()` — duplicate threshold check | TEST-004 to TEST-011 |
| SDD-012 | SRD-011 | `MissionManager::buildTrajectory()` — TAKEOFF prepend | TEST-014, TEST-016, TEST-027, TEST-028, TEST-029 |
| SDD-013 | SRD-012 | `MissionManager::buildTrajectory()` — LAND append | TEST-015, TEST-016, TEST-027, TEST-028, TEST-029 |
| SDD-014 | SRD-014, SRD-015 | `MissionManager::validationError()` | TEST-020, TEST-021, TEST-022 |
| SDD-020 | SRD-001 | `MapEngine` ctor, `drawGrid()` | GUI |
| SDD-021 | SRD-002 | `MapEngine::mousePressEvent()`, `wheelEvent()` | GUI |
| SDD-022 | SRD-003 | `MapEngine::mousePressEvent()` — line rendering | GUI |
| SDD-023 | SRD-004 | `MapEngine::refreshColors()` | GUI |
| SDD-024 | SRD-005 | `MapEngine::removeLastWaypoint()` | TEST-024, TEST-026 |
| SDD-025 | SRD-006 | `MapEngine::clearAll()` | TEST-025 |
| SDD-030 | SRD-020 | `MainWindow::onUpload()` — per-waypoint log line | GUI |
| SDD-031 | SRD-021 | `MainWindow::onUpload()` — early-exit on invalid | GUI |
| SDD-032 | SRD-020 | `MainWindow::onUpload()` — header and footer lines | GUI |
| SDD-041 | SRD-020 | `MainWindow` ctor — "Upload Mission" toolbar action | GUI |
| SDD-042 | SRD-005 | `MainWindow` ctor — "Remove Last" toolbar action | GUI |
| SDD-043 | SRD-006 | `MainWindow::onClear()` | GUI |
| SDD-051 | SRD-007 | `MainWindow::updateStatus()` — empty-mission message | GUI |
| SDD-052 | SRD-007 | `MainWindow::updateStatus()` — invalid-mission message | GUI |
| SDD-053 | SRD-007 | `MainWindow::updateStatus()` — valid-mission message | GUI |
| SDD-054 | SRD-007 | `MainWindow::updateStatus()` — called after every mutation | GUI |

---

## 2. Backward Traceability (Code → Requirement)

| Component / Function | File | Satisfies |
|---|---|---|
| `struct Waypoint` | `src/Waypoint.h` | SDD-001, SRD-010 |
| `enum class WaypointType` | `src/Waypoint.h` | SDD-002, SRD-011, SRD-012 |
| `Waypoint::defaultAltitude()` | `src/Waypoint.h` | SDD-003, SDD-004, SDD-005, SRD-013 |
| `MissionManager::addWaypoint()` | `src/MissionManager.cpp` | SRD-010, SYS-001 |
| `MissionManager::removeLastWaypoint()` | `src/MissionManager.cpp` | SRD-005, SDD-024 |
| `MissionManager::clear()` | `src/MissionManager.cpp` | SRD-006 |
| `MissionManager::isValid()` | `src/MissionManager.cpp` | SDD-010, SDD-011, SRD-014, SRD-015, SYS-004 |
| `MissionManager::validationError()` | `src/MissionManager.cpp` | SDD-014, SRD-022 |
| `MissionManager::buildTrajectory()` | `src/MissionManager.cpp` | SDD-012, SDD-013, SRD-011, SRD-012, SYS-002 |
| `MissionManager::rawPoints()` | `src/MissionManager.cpp` | SRD-010 |
| `MissionManager::DUPLICATE_THRESHOLD` | `src/MissionManager.h` | SDD-011, SRD-015 |
| `MapEngine::MapEngine()`, `drawGrid()` | `src/MapEngine.cpp` | SDD-020, SRD-001, SYS-006 |
| `MapEngine::mousePressEvent()` | `src/MapEngine.cpp` | SDD-021, SDD-022, SRD-002, SRD-003, SYS-001 |
| `MapEngine::wheelEvent()` | `src/MapEngine.cpp` | SDD-021 |
| `MapEngine::refreshColors()` | `src/MapEngine.cpp` | SDD-023, SRD-004 |
| `MapEngine::removeLastWaypoint()` | `src/MapEngine.cpp` | SDD-024, SRD-005 |
| `MapEngine::clearAll()` | `src/MapEngine.cpp` | SDD-025, SRD-006 |
| `MainWindow::MainWindow()` — toolbar setup | `src/MainWindow.cpp` | SDD-041, SDD-042, SDD-043 |
| `MainWindow::setupLogPanel()` | `src/MainWindow.cpp` | SDD-030, SDD-031, SDD-032 |
| `MainWindow::logMessage()` | `src/MainWindow.cpp` | SDD-030, SDD-032 |
| `MainWindow::onWaypointAdded()` | `src/MainWindow.cpp` | SDD-001, SRD-010 |
| `MainWindow::onWaypointRemoved()` | `src/MainWindow.cpp` | SDD-001, SRD-005 |
| `MainWindow::onUpload()` | `src/MainWindow.cpp` | SDD-030, SDD-031, SDD-032, SDD-041, SRD-020, SRD-021, SYS-005 |
| `MainWindow::onClear()` | `src/MainWindow.cpp` | SDD-043, SRD-006 |
| `MainWindow::updateStatus()` | `src/MainWindow.cpp` | SDD-051, SDD-052, SDD-053, SDD-054, SRD-007 |
| `main()` | `src/main.cpp` | SYS-009 |
| `tst_testmissionplanner.cpp` (all tests) | `missionPlanTest/` | SYS-008, TEST-001 to TEST-029 |
| `missionPlanTest/CMakeLists.txt` | `missionPlanTest/` | SYS-008 |

---

## 3. Test Coverage Summary

| Layer | Requirements | Automated Tests | GUI Tests |
|---|---|---|---|
| Domain — `MissionManager` | SRD-005, SRD-006, SRD-010 to SRD-015, SRD-022 | TEST-001 to TEST-029 | — |
| Domain — `Waypoint` | SDD-001 to SDD-005, SRD-013 | TEST-012, TEST-013, TEST-029 | — |
| Map UI — `MapEngine` | SDD-020 to SDD-025, SRD-001 to SRD-006 | — | SRD-001 to SRD-006 |
| Shell — `MainWindow` | SDD-030 to SDD-054, SRD-007, SRD-020, SRD-021 | — | All SDD-03x, SDD-04x, SDD-05x |
| System | SYS-001 to SYS-009 | SYS-004, SYS-008 (partial) | SYS-001, SYS-002, SYS-005, SYS-006, SYS-009 |