#############################
# DOCTEST
#############################
include(ExternalProject)
find_package(Git REQUIRED)


if(BUILD_TESTS)
    ExternalProject_Add(
        doctest
        PREFIX ${CMAKE_BINARY_DIR}/external_projects/doctest
        GIT_REPOSITORY https://github.com/onqtam/doctest.git
        TIMEOUT 10
        #UPDATE_COMMAND ${GIT_EXECUTABLE} pull
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        INSTALL_COMMAND ""
        LOG_DOWNLOAD ON
    )

    # Expose required variable (DOCTEST_INCLUDE_DIR) to parent scope
    ExternalProject_Get_Property(doctest source_dir)
    set(DOCTEST_INCLUDE_DIR ${source_dir}/doctest CACHE INTERNAL "Path to include folder for doctest")
    message(STATUS "DOCTEST_INCLUDE_DIR" ${DOCTEST_INCLUDE_DIR} )
endif(BUILD_TESTS)


if(BUILD_BENCHMARK)




    #############################
    # hayai (benchmarks)
    #############################
    ExternalProject_Add(
        json
        PREFIX ${CMAKE_BINARY_DIR}/external_projects/json
        GIT_REPOSITORY https://github.com/nlohmann/json
        GIT_TAG v2.1.1
        TIMEOUT 5
        #UPDATE_COMMAND ${GIT_EXECUTABLE} pull
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        INSTALL_COMMAND ""
        LOG_DOWNLOAD ON
    )
    # Expose required variable (DOCTEST_INCLUDE_DIR) to parent scope
    ExternalProject_Get_Property(json source_dir)
    set(JSON_INCLUDE_DIR ${source_dir}/src/ CACHE INTERNAL "Path to include folder for json")
    message(STATUS "JSON_INCLUDE_DIR" ${JSON_INCLUDE_DIR} )


    #############################
    # vigra (to compare)
    #############################
    ExternalProject_Add(
        vigra
        PREFIX ${CMAKE_BINARY_DIR}/external_projects/vigra
        GIT_REPOSITORY https://github.com/ukoethe/vigra
        GIT_TAG Version-1-11-1
        TIMEOUT 5
        #UPDATE_COMMAND ${GIT_EXECUTABLE} pull
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        INSTALL_COMMAND ""
        LOG_DOWNLOAD ON
    )
    # Expose required variable (DOCTEST_INCLUDE_DIR) to parent scope
    ExternalProject_Get_Property(vigra source_dir)
    set(VIGRA_INCLUDE_DIR ${source_dir}/include/ CACHE INTERNAL "Path to include folder for vigra")
    message(STATUS "VIGRA_INCLUDE_DIR" ${VIGRA_INCLUDE_DIR} )

    #############################
    # vigra (to compare)
    #############################
    ExternalProject_Add(
        xtensor
        PREFIX ${CMAKE_BINARY_DIR}/external_projects/xtensor
        GIT_REPOSITORY https://github.com/QuantStack/xtensor
        GIT_TAG 0.10.7
        TIMEOUT 5
        #UPDATE_COMMAND ${GIT_EXECUTABLE} pull
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        INSTALL_COMMAND ""
        LOG_DOWNLOAD ON
    )
    # Expose required variable (DOCTEST_INCLUDE_DIR) to parent scope
    ExternalProject_Get_Property(xtensor source_dir)
    set(XTENSOR_INCLUDE_DIR ${source_dir}/include/ CACHE INTERNAL "Path to include folder for xtensor")
    message(STATUS "XTENSOR_INCLUDE_DIR" ${XTENSOR_INCLUDE_DIR} )





    # #################################
    # # blitz++ https://github.com/syntheticpp/blitz
    # #################################
    # ExternalProject_Add(
    #     blitz
    #     PREFIX ${CMAKE_BINARY_DIR}/external_projects/blitz
    #     URL https://downloads.sourceforge.net/project/blitz/blitz/Blitz%2B%2B%200.10/blitz-0.10.tar.gz?r=https%3A%2F%2Fsourceforge.net%2Fprojects%2Fblitz%2Ffiles%2Fblitz%2FBlitz%252B%252B%25200.10%2F&ts=1500718778&use_mirror=kent
    #     TIMEOUT 5
    #     #UPDATE_COMMAND ${GIT_EXECUTABLE} pull
    #     CONFIGURE_COMMAND "./autoreconf -f -i -v"
    #     BUILD_COMMAND "make lib"
    #     INSTALL_COMMAND "make install"
    #     LOG_DOWNLOAD ON
    # )
    # # Expose required variable (DOCTEST_INCLUDE_DIR) to parent scope
    # ExternalProject_Get_Property(blitz source_dir)
    # set(BLITZ_INCLUDE_DIR ${source_dir} CACHE INTERNAL "Path to include folder for blitz")
    # message(STATUS "BLITZ_INCLUDE_DIR" ${BLITZ_INCLUDE_DIR} )

endif(BUILD_BENCHMARK)