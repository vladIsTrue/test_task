find_package(Qt6 REQUIRED COMPONENTS Core Network Sql)

add_executable(Server
	main.cpp
	dbcontroller/idbcontroller.h
	dbcontroller/pgdbcontroller.cpp
	dbcontroller/pgdbcontroller.h
	server/server.cpp
	server/server.h
)
target_link_libraries(Server Qt6::Core)
target_link_libraries(Server Qt6::Network)
target_link_libraries(Server Qt6::Sql)


include(GNUInstallDirs)
install(TARGETS Server
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
