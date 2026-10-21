cmake_minimum_required(VERSION 3.16)

project(BullFrogOS
    VERSION 0.1.0
    LANGUAGES C CXX ASM
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS
    Widgets
    Network
    WebEngineWidgets
)


# =============================================================
# BullFrog System
# =============================================================

add_executable(
    bullfrog-system

    src/system/system_info.c

    src/arch/x86_64.S
)

target_compile_options(
    bullfrog-system
    PRIVATE
        -Wall
        -Wextra
)


# =============================================================
# BullFrog Desktop
# =============================================================

add_executable(
    bullfrog-desktop

    # ---------------------------------------------------------
    # Main
    # ---------------------------------------------------------

    src/desktop/main.cpp


    # ---------------------------------------------------------
    # Shell
    # ---------------------------------------------------------

    src/desktop/shell/desktop.h
    src/desktop/shell/desktop.cpp

    src/desktop/shell/panel.h
    src/desktop/shell/panel.cpp

    src/desktop/shell/dock.h
    src/desktop/shell/dock.cpp


    # ---------------------------------------------------------
    # Launcher
    # ---------------------------------------------------------

    src/desktop/launcher/launcher.h
    src/desktop/launcher/launcher.cpp


    # ---------------------------------------------------------
    # Quick Settings
    # ---------------------------------------------------------

    src/desktop/quicksettings/quicksettings.h
    src/desktop/quicksettings/quicksettings.cpp


    # ---------------------------------------------------------
    # Notifications
    # ---------------------------------------------------------

    src/desktop/notifications/notifications.h
    src/desktop/notifications/notifications.cpp


    # ---------------------------------------------------------
    # Applications
    # ---------------------------------------------------------

    src/desktop/apps/files.h
    src/desktop/apps/files.cpp

    src/desktop/apps/terminal.h
    src/desktop/apps/terminal.cpp

    src/desktop/apps/settings.h
    src/desktop/apps/settings.cpp

    src/desktop/apps/browser.h
    src/desktop/apps/browser.cpp

    src/desktop/apps/activity_monitor.h
    src/desktop/apps/activity_monitor.cpp


    # ---------------------------------------------------------
    # Settings
    # ---------------------------------------------------------

    src/desktop/settings/settings_manager.h
    src/desktop/settings/settings_manager.cpp


    # ---------------------------------------------------------
    # Hardware
    # ---------------------------------------------------------

    src/hardware/cpu.h
    src/hardware/cpu.cpp

    src/hardware/memory.h
    src/hardware/memory.cpp

    src/hardware/network.h
    src/hardware/network.cpp

    src/hardware/bluetooth.h
    src/hardware/bluetooth.cpp

    src/hardware/system_summary.h
    src/hardware/system_summary.cpp
)


# =============================================================
# Qt Libraries
# =============================================================

target_link_libraries(
    bullfrog-desktop
    PRIVATE

        Qt6::Widgets

        Qt6::Network

        Qt6::WebEngineWidgets
)


# =============================================================
# Compiler warnings
# =============================================================

target_compile_options(
    bullfrog-desktop
    PRIVATE

        -Wall

        -Wextra
)