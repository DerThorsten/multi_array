if(BUILD_TESTS)


    # add it globally
    include_directories(${DOCTEST_INCLUDE_DIR})



    # Prepare doctest for other targets to use
    #add_library(doctest_interface INTERFACE)
    #target_include_directories(doctest_interface INTERFACE ${test_multi_array}/include)


    add_executable(test_multi_array 
        test.cxx
        test_smart_multi_array.cxx
        test_operators.cxx
        test_misc.cxx
    )
    add_test(NAME test_multi_array COMMAND test_multi_array)
    target_link_libraries(test_multi_array)

endif(BUILD_TESTS)