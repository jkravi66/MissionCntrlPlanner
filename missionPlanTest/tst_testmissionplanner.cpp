#include <QtTest/QtTest>
#include "../src/MissionManager.h"
#include "../src/Waypoint.h"

class testMissionPlanner : public QObject
{
    Q_OBJECT

public:
    testMissionPlanner();
    ~testMissionPlanner() override;

private slots:

    // -------------------------------------------------------------------------
    //  Test ID         : TEST-001
    //  Verifies        : SDD-010, SRD-014
    //  Test Type       : Normal
    //  Test Description: This test verifies isValid() returns false when 0 waypoints are present
    // -------------------------------------------------------------------------
    void test_001_isValid_zeroWaypoints()
    {
        MissionManager mm;
        QVERIFY(!mm.isValid());
    }

    // -------------------------------------------------------------------------
    //  Test ID         : TEST-002
    //  Verifies        : SDD-010, SRD-014
    //  Test Type       : Normal
    //  Test Description: This test verifies isValid() returns false when exactly 1 waypoint is present
    // -------------------------------------------------------------------------
    void test_002_isValid_oneWaypoint()
    {
        MissionManager mm;
        mm.addWaypoint(10.0, 20.0);
        QVERIFY(!mm.isValid());
    }

    // -------------------------------------------------------------------------
    //  Test ID         : TEST-003
    //  Verifies        : SDD-010
    //  Test Type       : Normal
    //  Test Description: This test verifies isValid() returns true for a valid 2-waypoint mission
    // -------------------------------------------------------------------------
    void test_003_isValid_twoDistinctWaypoints()
    {
        MissionManager mm;
        mm.addWaypoint(0.0, 0.0);
        mm.addWaypoint(1.0, 1.0);
        QVERIFY(mm.isValid());
    }

    // -------------------------------------------------------------------------
    //  Test ID         : TEST-004
    //  Verifies        : SDD-011, SRD-015
    //  Test Type       : Normal
    //  Test Description: This test verifies isValid() returns false when consecutive waypoints are identical (distance = 0)
    // -------------------------------------------------------------------------
    void test_004_isValid_exactDuplicatePoints()
    {
        MissionManager mm;
        mm.addWaypoint(5.0, 5.0);
        mm.addWaypoint(5.0, 5.0);   // identical — distance = 0
        QVERIFY(!mm.isValid());
    }

    // -------------------------------------------------------------------------
    //  Test ID         : TEST-005
    //  Verifies        : SDD-011, SRD-015
    //  Test Type       : Normal
    //  Test Description: This test verifies isValid() false when consecutive waypoints are within 0.001 units
    //
    void test_005_isValid_nearDuplicatePoints()
    {
        MissionManager mm;
        mm.addWaypoint(0.0, 0.0);
        mm.addWaypoint(0.0005, 0.0005);  // dist ≈ 0.000707 < 0.001
        QVERIFY(!mm.isValid());
    }

    // -------------------------------------------------------------------------
    //  Test ID         : TEST-006
    //  Verifies        : SDD-011, SRD-015
    //  Test Type       : Normal
    //  Test Description: This test verifies isValid() returns true when waypoints ‘x’ distance exactly at DUPLICATE_THRESHOLD
    //
    void test_006_isValid_boundaryThresholdx_rejected()
    {
        MissionManager mm;
        mm.addWaypoint(0.0, 0.0);
        // Distance = DUPLICATE_THRESHOLD exactly → still invalid (< check)
        mm.addWaypoint(MissionManager::DUPLICATE_THRESHOLD, 0.0);
        QVERIFY(mm.isValid());
    }

    // -------------------------------------------------------------------------
    //  Test ID         : TEST-007
    //  Verifies        : SDD-011, SRD-015
    //  Test Type       : Robust
    //  Test Description: This test verifies isValid() returns true when waypoints ‘x’ distance just above (0,0001 tolerance) at DUPLICATE_THRESHOLD.
    //
    void test_007_isValid_justAboveThresholdx_accepted()
    {
        MissionManager mm;
        mm.addWaypoint(0.0, 0.0);
        mm.addWaypoint(MissionManager::DUPLICATE_THRESHOLD + 0.0001, 0.0);
        QVERIFY(mm.isValid());
    }

    // -------------------------------------------------------------------------
    //  Test ID         : TEST-008
    //  Verifies        : SDD-011, SRD-015
    //  Test Type       : Robust
    //  Test Description: This test verifies isValid() returns false when waypoints ‘x’ distance just below (0,0001 tolerance) at DUPLICATE_THRESHOLD
    // -------------------------------------------------------------------------
    void test_008_isValid_justBelowThresholdx_rejected()
    {
        MissionManager mm;
        mm.addWaypoint(0.0, 0.0);
        mm.addWaypoint(MissionManager::DUPLICATE_THRESHOLD - 0.0001, 0.0);
        QVERIFY(!mm.isValid());
    }

    // -------------------------------------------------------------------------
    //  Test ID         : TEST-009
    //  Verifies        : SDD-011, SRD-015
    //  Test Type       : Normal
    //  Test Description: This test verifies isValid() returns true when waypoints ‘y’ distance exactly at DUPLICATE_THRESHOLD
    // -------------------------------------------------------------------------
    void test_009_isValid_boundaryThresholdy_rejected()
    {
        MissionManager mm;
        mm.addWaypoint(0.0, 0.0);
        // Distance = DUPLICATE_THRESHOLD exactly → still invalid (< check)
        mm.addWaypoint(0.0, MissionManager::DUPLICATE_THRESHOLD);
        QVERIFY(mm.isValid());
    }

    // -------------------------------------------------------------------------
    //  Test ID         : TEST-010
    //  Verifies        : SDD-011, SRD-015
    //  Test Type       : Robust
    //  Test Description: This test verifies isValid() returns true when waypoints ‘y’ distance just above (0,0001 tolerance) at DUPLICATE_THRESHOLD.
    // -------------------------------------------------------------------------
    void test_010_isValid_justAboveThresholdy_accepted()
    {
        MissionManager mm;
        mm.addWaypoint(0.0, 0.0);
        mm.addWaypoint(0.0, MissionManager::DUPLICATE_THRESHOLD + 0.0001);
        QVERIFY(mm.isValid());
    }

    // -------------------------------------------------------------------------
    //  Test ID         : TEST-011
    //  Verifies        : SDD-011, SRD-015
    //  Test Type       : Robust
    //  Test Description: This test verifies isValid() returns false when waypoints ‘y’ distance just below (0,0001 tolerance) at DUPLICATE_THRESHOLD
    // -------------------------------------------------------------------------
    void test_011_isValid_justBelowThresholdy_rejected()
    {
        MissionManager mm;
        mm.addWaypoint(0.0, 0.0);
        mm.addWaypoint(0.0, MissionManager::DUPLICATE_THRESHOLD - 0.0001);
        QVERIFY(!mm.isValid());
    }

    // -------------------------------------------------------------------------
    //  Test ID         : TEST-012
    //  Verifies        : SDD-003
    //  Test Type       : Normal
    //  Test Description: This test verifies Waypoint::defaultAltitude(CRUISE) returns 50.0
    // -------------------------------------------------------------------------
    void test_012_waypointDefaultAltitude_cruise()
    {
        QCOMPARE(Waypoint::defaultAltitude(WaypointType::CRUISE), 50.0);
    }

    // -------------------------------------------------------------------------
    //  Test ID         : TEST-013
    //  Verifies        : SDD-004, SDD-005
    //  Test Type       : Normal
    //  Test Description: This test verifies Waypoint::defaultAltitude(TAKEOFF) and (LAND) both return 0.0
    // -------------------------------------------------------------------------
    void test_013_waypointDefaultAltitude_takeoffLand()
    {
        QCOMPARE(Waypoint::defaultAltitude(WaypointType::TAKEOFF), 0.0);
        QCOMPARE(Waypoint::defaultAltitude(WaypointType::LAND),    0.0);
    }

    // -------------------------------------------------------------------------
    //  Test ID         : TEST-014
    //  Verifies        : SDD-012, SRD-011
    //  Test Type       : Normal
    //  Test Description: This test verifies buildTrajectory() places TAKEOFF at index 0 with first raw-point coordinates
    // -------------------------------------------------------------------------
    void test_014_buildTrajectory_takeoffFirst()
    {
        MissionManager mm;
        mm.addWaypoint(3.0, 4.0);
        mm.addWaypoint(7.0, 8.0);

        auto traj = mm.buildTrajectory();
        QVERIFY(!traj.empty());
        QCOMPARE(traj.front().type, WaypointType::TAKEOFF);
        QCOMPARE(traj.front().index, 0);
        QCOMPARE(traj.front().x, 3.0);
        QCOMPARE(traj.front().y, 4.0);
    }

    // -------------------------------------------------------------------------
    //  Test ID         : TEST-015
    //  Verifies        : SDD-013, SRD-012
    //  Test Type       : Normal
    //  Test Description: This test verifies buildTrajectory() appends LAND at the last index with last raw-point coordinates
    // -------------------------------------------------------------------------
    void test_015_buildTrajectory_landLast()
    {
        MissionManager mm;
        mm.addWaypoint(3.0, 4.0);
        mm.addWaypoint(7.0, 8.0);

        auto traj = mm.buildTrajectory();
        QVERIFY(!traj.empty());
        QCOMPARE(traj.back().type, WaypointType::LAND);
        QCOMPARE(traj.back().x, 7.0);
        QCOMPARE(traj.back().y, 8.0);
    }

    // -------------------------------------------------------------------------
    //  Test ID         : TEST-016
    //  Verifies        : SDD-012, SDD-013, SRD-022
    //  Test Type       : Normal
    //  Test Description: This test verifies buildTrajectory() output order as: TAKEOFF → CRUISE(s) → LAND.
    // -------------------------------------------------------------------------
    void test_016_buildTrajectory_order()
    {
        MissionManager mm;
        mm.addWaypoint(0.0, 0.0);
        mm.addWaypoint(5.0, 0.0);
        mm.addWaypoint(10.0, 0.0);

        auto traj = mm.buildTrajectory();

        // Minimum expected size: 1 TAKEOFF + 3 CRUISE + 1 LAND = 5
        QCOMPARE((int)traj.size(), 5);
        QCOMPARE(traj[0].type, WaypointType::TAKEOFF);
        QCOMPARE(traj[1].type, WaypointType::CRUISE);
        QCOMPARE(traj[2].type, WaypointType::CRUISE);
        QCOMPARE(traj[3].type, WaypointType::CRUISE);
        QCOMPARE(traj[4].type, WaypointType::LAND);
    }

    // -------------------------------------------------------------------------
    //  Test ID         : TEST-017
    //  Verifies        : SDD-004, SRD-011
    //  Test Type       : Normal
    //  Test Description: This test verifies TAKEOFF waypoint shall have altitude = 0.0 m.
    // -------------------------------------------------------------------------
    void test_017_buildTrajectory_takeoffAltitude()
    {
        MissionManager mm;
        mm.addWaypoint(0.0, 0.0);
        mm.addWaypoint(5.0, 5.0);

        auto traj = mm.buildTrajectory();
        QCOMPARE(traj.front().type, WaypointType::TAKEOFF);
        QCOMPARE(traj.front().altitude, 0.0);
    }

    // -------------------------------------------------------------------------
    //  Test ID         : TEST-018
    //  Verifies        : SDD-005, SRD-012
    //  Test Type       : Normal
    //  Test Description: This test verifies LAND waypoint shall have altitude = 0.0 m.
    // -------------------------------------------------------------------------
    void test_018_buildTrajectory_landAltitude()
    {
        MissionManager mm;
        mm.addWaypoint(0.0, 0.0);
        mm.addWaypoint(5.0, 5.0);

        auto traj = mm.buildTrajectory();
        QCOMPARE(traj.back().type, WaypointType::LAND);
        QCOMPARE(traj.back().altitude, 0.0);
    }

    // -------------------------------------------------------------------------
    //  Test ID         : TEST-019
    //  Verifies        : SDD-003, SRD-013
    //  Test Type       : Normal
    //  Test Description: This test verifies All CRUISE waypoints shall have altitude = 50.0 m.
    // -------------------------------------------------------------------------
    void test_019_buildTrajectory_cruiseAltitude()
    {
        MissionManager mm;
        mm.addWaypoint(0.0, 0.0);
        mm.addWaypoint(10.0, 0.0);
        mm.addWaypoint(20.0, 0.0);

        auto traj = mm.buildTrajectory();
        for (const auto& wp : traj) {
            if (wp.type == WaypointType::CRUISE) {
                QCOMPARE(wp.altitude, 50.0);
            }
        }
    }

    // -------------------------------------------------------------------------
    //  Test ID         : TEST-020
    //  Verifies        : SDD-014, SRD-022
    //  Test Type       : Normal
    //  Test Description: This test verifies validationError() returns a non-empty string for too few waypoints
    // -------------------------------------------------------------------------
    void test_020_validationError_tooFewPoints()
    {
        MissionManager mm;
        QVERIFY(!mm.validationError().isEmpty());

        mm.addWaypoint(1.0, 1.0);
        QVERIFY(!mm.validationError().isEmpty());
    }

    // -------------------------------------------------------------------------
    //  Test ID         : TEST-021
    //  Verifies        : SDD-014, SRD-022
    //  Test Type       : Normal
    //  Test Description: This test verifies validationError() returns a non-empty string for duplicate waypoints
    // -------------------------------------------------------------------------
    void test_021_validationError_duplicatePoints()
    {
        MissionManager mm;
        mm.addWaypoint(2.0, 2.0);
        mm.addWaypoint(2.0, 2.0);
        QVERIFY(!mm.validationError().isEmpty());
    }

    // -------------------------------------------------------------------------
    //  Test ID         : TEST-022
    //  Verifies        : SDD-014
    //  Test Type       : Normal
    //  Test Description: This test verifies validationError() returns empty string when mission is valid
    // -------------------------------------------------------------------------
    void test_022_validationError_emptyWhenValid()
    {
        MissionManager mm;
        mm.addWaypoint(0.0, 0.0);
        mm.addWaypoint(5.0, 5.0);
        QVERIFY(mm.validationError().isEmpty());
    }


    // -------------------------------------------------------------------------
    //  Test ID         : TEST-023
    //  Verifies        : SRD-010
    //  Test Type       : Normal
    //  Test Description: This test verifies addWaypoint() increase the raw point count by 1.
    // -------------------------------------------------------------------------
    void test_023_addWaypoint_incrementsCount()
    {
        MissionManager mm;
        QCOMPARE((int)mm.rawPoints().size(), 0);
        mm.addWaypoint(1.0, 2.0);
        QCOMPARE((int)mm.rawPoints().size(), 1);
        mm.addWaypoint(3.0, 4.0);
        QCOMPARE((int)mm.rawPoints().size(), 2);
    }

    // -------------------------------------------------------------------------
    //  Test ID         : TEST-024
    //  Verifies        : SRD-005
    //  Test Type       : Normal
    //  Test Description: This test verifies removeLastWaypoint() decrease the raw point count by 1.
    // -------------------------------------------------------------------------
    void test_024_removeLastWaypoint_decrementsCount()
    {
        MissionManager mm;
        mm.addWaypoint(1.0, 1.0);
        mm.addWaypoint(2.0, 2.0);
        mm.removeLastWaypoint();
        QCOMPARE((int)mm.rawPoints().size(), 1);
    }

    // -------------------------------------------------------------------------
    //  Test ID         : TEST-025
    //  Verifies        : SRD-006
    //  Test Type       : Normal
    //  Test Description: This test verifies clear() shall empty all waypoints.
    // -------------------------------------------------------------------------
    void test_025_clear_emptiesMission()
    {
        MissionManager mm;
        mm.addWaypoint(1.0, 1.0);
        mm.addWaypoint(2.0, 2.0);
        mm.addWaypoint(3.0, 3.0);
        mm.clear();
        QCOMPARE((int)mm.rawPoints().size(), 0);
        QVERIFY(!mm.isValid());
    }

};

testMissionPlanner::testMissionPlanner() {}

testMissionPlanner::~testMissionPlanner() = default;

QTEST_MAIN(testMissionPlanner)

#include "tst_testmissionplanner.moc"
