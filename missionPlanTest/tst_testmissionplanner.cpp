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
   // void test_case1();
    void test_isValid_zeroWaypoints()
    {
        MissionManager mm;
        QVERIFY(!mm.isValid());
    }
};

testMissionPlanner::testMissionPlanner() {}

testMissionPlanner::~testMissionPlanner() = default;

//void testMissionPlanner::test_case1() {}

QTEST_MAIN(testMissionPlanner)

#include "tst_testmissionplanner.moc"
