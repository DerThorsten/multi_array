# dependencies for running the benchmarks 
# (we use google benchmarks)


find_package(Benchmark REQUIRED)
find_package(BLITZ REQUIRED)

include_directories(${Benchmark_INCLUDE_DIR})
include_directories(${JSON_INCLUDE_DIR})


# baselines / to compare
include_directories(${BLITZ_INCLUDE_DIR})
include_directories(${VIGRA_INCLUDE_DIR})
include_directories(${BLITZ_INCLUDE_DIR})
include_directories(${XTENSOR_INCLUDE_DIR})

# Set libs
SET(BM_LIBS     

    ${Benchmark_LIBRARIES} 
    ${CMAKE_THREAD_LIBS_INIT}  
    pthread 
    ${BLITZ_LIBRARIES}
)




add_executable(gbenchmark gbenchmark.cxx)


#target_link_libraries(gbenchmark pthread)
target_link_libraries(gbenchmark 

    ${Benchmark_LIBRARIES} 
    ${CMAKE_THREAD_LIBS_INIT}  
    pthread 
)




#add_executable(benchmark2 benchmark2.cxx)
#add_executable(benchmark3 benchmark3.cxx)
#add_executable(benchmark4 benchmark4.cxx)