cmake_minimum_required(VERSION 3.16)

project(SQLWrapper LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Sql)

add_library(SQLWrapper SHARED
  SQLWrapper_global.h
  sqlwrapper.cpp
  sqlwrapper.h
)

target_link_libraries(SQLWrapper PRIVATE
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Sql
)

target_compile_definitions(SQLWrapper PRIVATE SQLWRAPPER_LIBRARY)
