cmake_minimum_required(VERSION 2.8)


add_executable(dblayer_test
    main_dblayer_test.cpp
    dbschema.cpp
    dbshell.cpp
    )
target_link_libraries(dblayer_test ${EXTRA_LIBS})

add_executable(dbshell
    main_dblayer.cpp
    dbshell.cpp
    )
target_link_libraries(dbshell ${EXTRA_LIBS})

if(Qt5Widgets_FOUND)
 qt5_use_modules(dbshell qdblayer Core Network Widgets Xml)
endif()
