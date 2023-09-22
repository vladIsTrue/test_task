find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Network)
find_package(Qt6 REQUIRED COMPONENTS Sql)

add_executable(Server
  main.cpp
  utils.h
  server.h server.cpp
)
target_link_libraries(Server Qt6::Core)
target_link_libraries(Server Qt6::Network)
target_link_libraries(Server Qt6::Sql)


include(GNUInstallDirs)
install(TARGETS Server
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
