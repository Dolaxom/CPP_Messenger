cmake_minimum_required(VERSION 3.5)

project(messengerServer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(CMAKE_PREFIX_PATH "~/Qt/6.4.1/gcc_64")
set(CMAKE_PREFIX_PATH "/Users/thedolaxom/Qt/6.4.1/macos/lib/cmake")

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

set(PROJECT_SOURCES
        main.cpp
        )

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(messengerServer
            MANUAL_FINALIZATION
            ${PROJECT_SOURCES}
            )

else()
    if(ANDROID)
        add_library(messengerServer SHARED
                ${PROJECT_SOURCES}
                )
    else()
        add_executable(messengerServer
                ${PROJECT_SOURCES}
                )
    endif()
endif()

target_link_libraries(messengerServer PRIVATE Qt${QT_VERSION_MAJOR}::Core)

set_target_properties(messengerServer PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
        )

install(TARGETS messengerServer
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(messengerServer)
endif()

add_executable(messengerServerTarget ${PROJECT_SOURCES})
target_link_libraries(messengerServerTarget PRIVATE Qt${QT_VERSION_MAJOR}::Core)