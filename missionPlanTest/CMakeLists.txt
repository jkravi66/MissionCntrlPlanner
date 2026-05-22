# ── Qt packages ──────────────────────────────────────────────────────────────
find_package(QT NAMES Qt6 REQUIRED COMPONENTS Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Test)

# ── Executable ───────────────────────────────────────────────────────────────
qt_add_executable(testMissionPlanner tst_testmissionplanner.cpp
    ../src/MissionManager.cpp ../src/Waypoint.h)
target_include_directories(testMissionPlanner PRIVATE
    ../src
)

# ── Link libraries ───────────────────────────────────────────────────────────
target_link_libraries(testMissionPlanner PRIVATE
    Qt${QT_VERSION_MAJOR}::Test
    Qt${QT_VERSION_MAJOR}::Core     # QString
)

# ── tests ───────────────────────────────────────────────────────────
add_test(
    NAME MissionPlannerTests
    COMMAND testMissionPlanner -v2
)