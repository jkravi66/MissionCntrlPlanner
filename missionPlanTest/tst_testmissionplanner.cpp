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
    /*  Test ID         : TEST-001
        Verifies        : SDD-010, SRD-014
        Test Type       : Normal
        Test Description: This test verifies isValid() returns false when 0 waypoints are present
    */
    void test_001_isValid_zeroWaypoints()
    {
        MissionManager mm;
        QVERIFY(!mm.isValid());
    }
    // -------------------------------------------------------------------------

    // -------------------------------------------------------------------------
    /*  Test ID         : TEST-002
        Verifies        : SDD-010, SRD-014
        Test Type       : Normal
        Test Description: This test verifies isValid() returns false when exactly 1 waypoint is present
    */
    void test_002_isValid_oneWaypoint()
    {
        MissionManager mm;
        mm.addWaypoint(10.0, 20.0);
        QVERIFY(!mm.isValid());
    }
    // -------------------------------------------------------------------------

    // -------------------------------------------------------------------------
    /*  Test ID         : TEST-003
        Verifies        : SDD-010
        Test Type       : Normal
        Test Description: This test verifies isValid() returns true for a valid 2-waypoint mission
    */
    void test_003_isValid_twoDistinctWaypoints()
    {
        MissionManager mm;
        mm.addWaypoint(0.0, 0.0);
        mm.addWaypoint(1.0, 1.0);
        QVERIFY(mm.isValid());
    }
    // -------------------------------------------------------------------------

    // -------------------------------------------------------------------------
    /*  Test ID         : TEST-004
        Verifies        : SDD-011, SRD-015
        Test Type       : Normal
        Test Description: This test verifies isValid() returns false when consecutive waypoints are identical (distance = 0)
    */
    void test_004_isValid_exactDuplicatePoints()
    {
        MissionManager mm;
        mm.addWaypoint(5.0, 5.0);
        mm.addWaypoint(5.0, 5.0);   // identical — distance = 0
        QVERIFY(!mm.isValid());
    }
    // -------------------------------------------------------------------------

    // -------------------------------------------------------------------------
    /*  Test ID         : TEST-005
        Verifies        : SDD-011, SRD-015
        Test Type       : Normal
        Test Description: This test verifies isValid() false when consecutive waypoints are within 0.001 units
    */
    void test_005_isValid_nearDuplicatePoints()
    {
        MissionManager mm;
        mm.addWaypoint(0.0, 0.0);
        mm.addWaypoint(0.0005, 0.0005);  // dist ≈ 0.000707 < 0.001
        QVERIFY(!mm.isValid());
    }
    // -------------------------------------------------------------------------

    // -------------------------------------------------------------------------
    /*  Test ID         : TEST-006
        Verifies        : SDD-011, SRD-015
        Test Type       : Normal
        Test Description: This test verifies isValid() returns true when waypoints ‘x’ distance exactly at DUPLICATE_THRESHOLD
    */
    void test_006_isValid_boundaryThresholdx_rejected()
    {
        MissionManager mm;
        mm.addWaypoint(0.0, 0.0);
        // Distance = DUPLICATE_THRESHOLD exactly → still invalid (< check)
        mm.addWaypoint(MissionManager::DUPLICATE_THRESHOLD, 0.0);
        QVERIFY(mm.isValid());
    }
    // -------------------------------------------------------------------------

    // -------------------------------------------------------------------------
    /*  Test ID         : TEST-007
        Verifies        : SDD-011, SRD-015
        Test Type       : Robust
        Test Description: This test verifies isValid() true when waypoints ‘x’ distance just above (0,0001 tolerance) at DUPLICATE_THRESHOLD.
    */
    void test_007_isValid_justAboveThresholdx_accepted()
    {
        MissionManager mm;
        mm.addWaypoint(0.0, 0.0);
        mm.addWaypoint(MissionManager::DUPLICATE_THRESHOLD + 0.0001, 0.0);
        QVERIFY(mm.isValid());
    }

    // -------------------------------------------------------------------------
    /*  Test ID         : TEST-008
        Verifies        : SDD-011, SRD-015
        Test Type       : Robust
        Test Description: This test verifies isValid() false when waypoints ‘x’ distance just below (0,0001 tolerance) at DUPLICATE_THRESHOLD
    */
    void test_008_isValid_justBelowThresholdx_rejected()
    {
        MissionManager mm;
        mm.addWaypoint(0.0, 0.0);
        mm.addWaypoint(MissionManager::DUPLICATE_THRESHOLD - 0.0001, 0.0);
        QVERIFY(!mm.isValid());
    }

    // -------------------------------------------------------------------------
    /*  Test ID         : TEST-009
        Verifies        : SDD-011, SRD-015
        Test Type       : Normal
        Test Description: This test verifies isValid() returns true when waypoints ‘y’ distance exactly at DUPLICATE_THRESHOLD
    */
    void test_009_isValid_boundaryThresholdy_rejected()
    {
        MissionManager mm;
        mm.addWaypoint(0.0, 0.0);
        // Distance = DUPLICATE_THRESHOLD exactly → still invalid (< check)
        mm.addWaypoint(0.0, MissionManager::DUPLICATE_THRESHOLD);
        QVERIFY(mm.isValid());
    }


    // -------------------------------------------------------------------------
    /*  Test ID         : TEST-010
        Verifies        : SDD-011, SRD-015
        Test Type       : Robust
        Test Description: This test verifies isValid() true when waypoints ‘y’ distance just above (0,0001 tolerance) at DUPLICATE_THRESHOLD.
    */

    void test_010_isValid_justAboveThresholdy_accepted()
    {
        MissionManager mm;
        mm.addWaypoint(0.0, 0.0);
        mm.addWaypoint(0.0, MissionManager::DUPLICATE_THRESHOLD + 0.0001);
        QVERIFY(mm.isValid());
    }

    // -------------------------------------------------------------------------
    /*  Test ID         : TEST-011
        Verifies        : SDD-011, SRD-015
        Test Type       : Robust
        Test Description: This test verifies isValid() false when waypoints ‘y’ distance just below (0,0001 tolerance) at DUPLICATE_THRESHOLD
    */
    void test_011_isValid_justBelowThresholdx_rejected()
    {
        MissionManager mm;
        mm.addWaypoint(0.0, 0.0);
        mm.addWaypoint(0.0, MissionManager::DUPLICATE_THRESHOLD - 0.0001);
        QVERIFY(!mm.isValid());
    }

};

testMissionPlanner::testMissionPlanner() {}

testMissionPlanner::~testMissionPlanner() = default;

//void testMissionPlanner::test_case1() {}

QTEST_MAIN(testMissionPlanner)

#include "tst_testmissionplanner.moc"
