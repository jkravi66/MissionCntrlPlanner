# Prerequisites
- Qt 6.2+ with Widgets and Svg modules
- CMake ≥ 3.16
- C++17 compiler (GCC 10+ / MSVC 2019+)

# Build
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build

# Run
./build/missionPlanner

# Run Tests (headless)
cd build
QT_QPA_PLATFORM=offscreen ctest --output-on-failure

# Requirements & Traceability
See docs/Mission Planner.docx — contains SYS/SRD/SDD requirements
and the full bidirectional traceability matrix.