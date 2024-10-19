cmake_minimum_required(VERSION 3.5)

project(GridEditor VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    include_directories($(GLEW_INCLUDE_DIRS))
    link_libraries(${GLEW_LIBRARIES})
endif()
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

find_package(X11 REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        qcustomglwidget.h
        qcustomglwidget.cpp
        shaders.cpp
        shaders.h
        vertices.cpp
        vertices.h
        drawable.cpp
        drawable.h
        point.cpp
        point.h
        handle.cpp
        handle.h
        curvepiece.cpp
        curvepiece.h
        curvebuilder.cpp
        curvebuilder.h
        mathtools.cpp
        mathtools.h
        draggable.cpp
        draggable.h
        camera.cpp
        camera.h
        defaultVertexShader.vert
        defaultVertexShader.frag
)

set(SOURCE_FILE_VERT ${CMAKE_SOURCE_DIR}/defaultVertexShader.vert)
set(SOURCE_FILE_FRAG ${CMAKE_SOURCE_DIR}/defaultVertexShader.frag)
set(DESTINATION_DIR ${CMAKE_BINARY_DIR})

add_custom_command(
    OUTPUT ${DESTINATION_DIR}/defaultVertexShader.vert ${DESTINATION_DIR}/defaultVertexShader.frag
    COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_FILE_VERT} ${DESTINATION_DIR}/defaultVertexShader.vert
    COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_FILE_FRAG} ${DESTINATION_DIR}/defaultVertexShader.frag
    DEPENDS ${SOURCE_FILE_VERT} ${SOURCE_FILE_FRAG}
    COMMENT "Copying defaultVertexShader.vert to build directory"
)

add_custom_target(copy_myfile ALL DEPENDS ${DESTINATION_DIR}/defaultVertexShader.vert ${DESTINATION_DIR}/defaultVertexShader.frag)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(GridEditor
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET GridEditor APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(GridEditor SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(GridEditor
            ${PROJECT_SOURCES}
        )
    endif()
endif()

find_package(glm REQUIRED)
target_link_libraries(GridEditor PRIVATE glm)

target_link_libraries(GridEditor PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(GridEditor PRIVATE ${X11_LIBRARIES})

set_target_properties(GridEditor PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include_directories(${X11_INCLUDE_DIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(GridEditor)
endif()
