find_package(Benchmark REQUIRED)



include_directories(${Benchmark_INCLUDE_DIR})
#include_directories(${HAYAI_INCLUDE_DIR})
include_directories(${VIGRA_INCLUDE_DIR})


add_executable(gbenchmark gbenchmark.cxx)


#target_link_libraries(gbenchmark pthread)
target_link_libraries(gbenchmark ${Benchmark_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT}  pthread )




#add_executable(benchmark2 benchmark2.cxx)
#add_executable(benchmark3 benchmark3.cxx)
#add_executable(benchmark4 benchmark4.cxx)