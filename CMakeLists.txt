project(kde-telepathy-test-tool)

SET(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules" ${CMAKE_MODULE_PATH})

find_package(KDE4 REQUIRED)

find_package(TelepathyQt4 REQUIRED)

include(KDE4Defaults)
include(MacroLibrary)


# Add the compiler flags
add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories(
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_BINARY_DIR}
  ${KDE4_INCLUDES}
  ${TELEPATHY_QT4_INCLUDE_DIR}
)

# Insert your sources here
set(CHANNEL-JOINER_SRCS
    mainwindow.cpp
    account-item.cpp
    accounts-list-model.cpp
    contactlistmodel.cpp
    contactmodelfilter.cpp
    main.cpp
)

kde4_add_ui_files(CHANNEL-JOINER_SRCS mainwindow.ui)

# Create the plasmoid
kde4_add_executable(kde-telepathy-test-tool ${CHANNEL-JOINER_SRCS})

# Link against the needed libraries
target_link_libraries(kde-telepathy-test-tool
  ${KDE4_KDEUI_LIBS}
  ${TELEPATHY_QT4_LIBRARIES}
)

install(TARGETS kde-telepathy-test-tool ${INSTALL_TARGETS_DEFAULT_ARGS})
# install(FILES kde-telepathy-test-tool.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})

macro_display_feature_log()


