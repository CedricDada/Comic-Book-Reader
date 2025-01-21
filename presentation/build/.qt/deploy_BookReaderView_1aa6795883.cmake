include(/media/cedric/DSCD/Projets/ENSTA/Comic book reader/presentation/build/.qt/QtDeploySupport.cmake)
include("${CMAKE_CURRENT_LIST_DIR}/BookReaderView-plugins.cmake" OPTIONAL)
set(__QT_DEPLOY_ALL_MODULES_FOUND_VIA_FIND_PACKAGE "Core;DBus;Gui;Widgets")

qt6_deploy_runtime_dependencies(
    EXECUTABLE /media/cedric/DSCD/Projets/ENSTA/Comic book reader/presentation/build/BookReaderView
    GENERATE_QT_CONF
)
