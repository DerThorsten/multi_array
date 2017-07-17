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
    # nonius (benchmarks)
    #############################
    include(ExternalProject)
    find_package(Git REQUIRED)

    ExternalProject_Add(
        nonius
        PREFIX ${CMAKE_BINARY_DIR}/external_projects/nonius
        GIT_REPOSITORY https://github.com/rmartinho/nonius
        GIT_TAG v1.2.0-beta.1
        TIMEOUT 10
        #UPDATE_COMMAND ${GIT_EXECUTABLE} pull
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        INSTALL_COMMAND ""
        LOG_DOWNLOAD ON
    )

    # Expose required variable (DOCTEST_INCLUDE_DIR) to parent scope
    ExternalProject_Get_Property(nonius source_dir)
    set(NONIUS_INCLUDE_DIR ${source_dir}/include/ CACHE INTERNAL "Path to include folder for nonius")
    message(STATUS "NONIUS_INCLUDE_DIR" ${NONIUS_INCLUDE_DIR} )



    #############################
    # hayai (benchmarks)
    #############################
    include(ExternalProject)
    find_package(Git REQUIRED)

    ExternalProject_Add(
        hayai
        PREFIX ${CMAKE_BINARY_DIR}/external_projects/hayai
        GIT_REPOSITORY https://github.com/nickbruun/hayai
        GIT_TAG v1.0.1
        TIMEOUT 10
        #UPDATE_COMMAND ${GIT_EXECUTABLE} pull
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        INSTALL_COMMAND ""
        LOG_DOWNLOAD ON
    )
    # Expose required variable (DOCTEST_INCLUDE_DIR) to parent scope
    ExternalProject_Get_Property(hayai source_dir)
    set(HAYAI_INCLUDE_DIR ${source_dir}/src/ CACHE INTERNAL "Path to include folder for hayai")
    message(STATUS "HAYAI_INCLUDE_DIR" ${HAYAI_INCLUDE_DIR} )


    #############################
    # vigra (to compare)
    #############################
    include(ExternalProject)
    find_package(Git REQUIRED)

    ExternalProject_Add(
        vigra
        PREFIX ${CMAKE_BINARY_DIR}/external_projects/vigra
        GIT_REPOSITORY https://github.com/ukoethe/vigra
        GIT_TAG Version-1-11-1
        TIMEOUT 10
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



endif(BUILD_BENCHMARK)