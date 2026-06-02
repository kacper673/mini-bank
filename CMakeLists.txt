cmake_minimum_required(VERSION 3.16)
project(bank LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ----------------------------------------------------------------
# Wersja konsolowa
# ----------------------------------------------------------------
add_executable(bank_console
    main_console.cpp
    core/account.h
    core/bank.h
    core/validation.h
)
target_include_directories(bank_console PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/core
)

set(CMAKE_AUTOMOC ON)

find_package(Qt6 QUIET COMPONENTS Widgets)
if(Qt6_FOUND)
    message(STATUS "Znaleziono Qt6")
    set(QT_LIBS Qt6::Widgets)
else()
    find_package(Qt5 REQUIRED COMPONENTS Widgets)
    message(STATUS "Znaleziono Qt5")
    set(QT_LIBS Qt5::Widgets)
endif()

add_executable(bank
    main.cpp
    ui/mainwindow.cpp
    ui/mainwindow.h
    core/account.h
    core/bank.h
    core/validation.h
)

target_link_libraries(bank PRIVATE ${QT_LIBS})

target_include_directories(bank PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}        # dla main.cpp -> #include "ui/mainwindow.h"
    ${CMAKE_CURRENT_SOURCE_DIR}/core   # dla #include "account.h" / "bank.h" / "validation.h"
    ${CMAKE_CURRENT_SOURCE_DIR}/ui     # dla #include "mainwindow.h"
)
