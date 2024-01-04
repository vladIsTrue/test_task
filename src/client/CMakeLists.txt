find_package(Qt6 REQUIRED COMPONENTS Widgets Network Sql)

set(PROJECT_SOURCES
        main.cpp
        mainwindow/mainwindow.cpp
        mainwindow/mainwindow.h
        mainwindow/mainwindow.ui
)


qt_add_executable(client
	MANUAL_FINALIZATION
        ${PROJECT_SOURCES}

	socket/socketconnection.cpp
        socket/socketconnection.h
	tablemodel/tablemodelemployee.cpp
        tablemodel/tablemodelemployee.h
	tablemodel/tablemodeltask.cpp
	tablemodel/tablemodeltask.h 
	info/recordinfo.h
)

target_link_libraries(client PRIVATE Qt6::Widgets)
target_link_libraries(client PRIVATE Qt6::Network)
target_link_libraries(client PRIVATE Qt6::Sql)

qt_finalize_executable(client)
