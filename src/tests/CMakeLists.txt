cmake_minimum_required(VERSION 2.8)


add_executable(cm_test
    cm_test.cpp
    )

add_executable(dbmgr_test
    dbmgr_test.cpp
    dbschema.cpp
#    dbshell.cpp
    )

add_executable(connection_test
    connection_test.cpp
    dbschema.cpp
#    dbshell.cpp
    )

if(Qt5Widgets_FOUND)
 target_link_libraries(connection_test qdblayer ${EXTRA_LIBS})
 qt5_use_modules(connection_test Core Network Sql Widgets Xml)

 target_link_libraries(dbmgr_test qdblayer ${EXTRA_LIBS})
 qt5_use_modules(dbmgr_test Core Network Sql Widgets Xml)

 target_link_libraries(cm_test qdblayer ${EXTRA_LIBS})
 qt5_use_modules(cm_test Core Network Sql Widgets Xml)
else()
 target_link_libraries(connection_test ${EXTRA_LIBS})
 target_link_libraries(dbmgr_test ${EXTRA_LIBS})
 target_link_libraries(cm_test ${EXTRA_LIBS})
endif()

