cmake_minimum_required(VERSION 2.8)


add_executable(dbshell
    main_dblayer.cpp
    dbshell.cpp
    )

if(Qt5Widgets_FOUND)
 target_link_libraries(dbshell qdblayer ${EXTRA_LIBS})
 qt5_use_modules(dbshell Core Network Widgets Xml)
else()
 target_link_libraries(dbshell ${EXTRA_LIBS})
endif()
