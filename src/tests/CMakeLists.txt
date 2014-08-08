cmake_minimum_required(VERSION 2.8)


add_executable(dblayer_test
    main_dblayer_test.cpp
    dbschema.cpp
#    dbshell.cpp
    )

if(Qt5Widgets_FOUND)
 target_link_libraries(dblayer_test qdblayer ${EXTRA_LIBS})
 qt5_use_modules(dblayer_test Core Network Widgets Xml)
else()
 target_link_libraries(dblayer_test ${EXTRA_LIBS})
endif()

