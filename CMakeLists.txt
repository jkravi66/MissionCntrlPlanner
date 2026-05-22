cmake_minimum_required(VERSION 3.16)
project(missionPlanner VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ── Qt packages ──────────────────────────────────────────────────────────────
find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets Svg)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Svg)

# ── Source files ─────────────────────────────────────────────────────────────
set(PROJECT_SOURCES
    src/main.cpp
    src/MainWindow.h
    src/MainWindow.cpp
    src/Waypoint.h
    src/MissionManager.h
    src/MissionManager.cpp
    src/MapEngine.h
    src/MapEngine.cpp
)

# ── Executable ───────────────────────────────────────────────────────────────
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(missionPlanner
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(missionPlanner SHARED ${PROJECT_SOURCES})
    else()
        add_executable(missionPlanner ${PROJECT_SOURCES})
    endif()
endif()

# ── Resources —  ────────────────────────────────────────
qt_add_resources(missionPlanner "app_resources"
    PREFIX "/icons"
    BASE   "resources/icons"
    FILES
        resources/icons/Avilus_Logo.svg
)

# ── Link libraries ───────────────────────────────────────────────────────────
target_link_libraries(missionPlanner PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Svg
)

# ── Target properties ────────────────────────────────────────────────────────
if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.missionPlanner)
endif()

set_target_properties(missionPlanner PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# ── Install ───────────────────────────────────────────────────────────────────
include(GNUInstallDirs)
install(TARGETS missionPlanner
    BUNDLE  DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(missionPlanner)
endif()

# Enable CTest and add the headless test sub-directory.
enable_testing()
add_subdirectory(missionPlanTest)



