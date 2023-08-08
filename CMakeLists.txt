cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)
project(gem5_visualizer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Quick Concurrent)

qt_standard_project_setup(REQUIRES 6.5)


set(SOURCES
    src/parser.cpp
    src/pipeline.cpp
    src/logmodel.cpp
)
set (HEADERS
    src/parser.h
    src/pipeline.h
    src/logmodel.h
)

qt_add_executable(appgem5_visualizer
    main.cpp
    ${SOURCES}
    ${HEADERS}
)


qt_add_qml_module(appgem5_visualizer
    URI gem5_visualizer
    VERSION 1.0
    QML_FILES Main.qml
    SOURCES
        src/pipeline.h
        src/pipeline.cpp
)




include_directories(src/)

set_target_properties(appgem5_visualizer PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appgem5_visualizer
    PRIVATE Qt6::Quick Qt6::Core Qt6::Concurrent
)

install(TARGETS appgem5_visualizer
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
