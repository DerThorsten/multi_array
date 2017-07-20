#include <benchmark/benchmark.h>


#include "multi_array/multi_array.hxx"


// vigra (to compare with)
#include <vigra/multi_array.hxx>
#include <vigra/multi_math.hxx>




namespace ma = multi_array;


template<std::size_t DIM>
static void expression1_ma(benchmark::State& state) {

    // setup
    const auto s = int64_t(state.range(0));
    auto a = ma::SmartMultiArray<int,2>(ma::Shape<2>(s));
    auto b = ma::SmartMultiArray<int,2>(ma::Shape<2>(s));
    auto c = ma::SmartMultiArray<int,2>(ma::Shape<2>(s));
    auto d = ma::SmartMultiArray<int,2>(ma::Shape<2>(s));


    // run
    while (state.KeepRunning()){
        d = (a + b)*2 + (2+c)*(2*a)*(3*b);
    }

    // how much work was done
    state.SetBytesProcessed(int64_t(state.iterations())*s*s);

    // tear down
    // ...nothing to do
}
BENCHMARK_TEMPLATE(expression1_ma, 2)->Range(8, 8<<10);








template<std::size_t DIM>
static void expression1_vigra(benchmark::State& state) {

    // setup
    const auto s = int64_t(state.range(0));
    auto a = vigra::MultiArray<2,int>(vigra::Shape2(s));
    auto b = vigra::MultiArray<2,int>(vigra::Shape2(s));
    auto c = vigra::MultiArray<2,int>(vigra::Shape2(s));
    auto d = vigra::MultiArray<2,int>(vigra::Shape2(s));


    // run
    while (state.KeepRunning()){
        using namespace vigra::multi_math;
        d = (a + b)*2 + (2+c)*(2*a)*(3*b);
    }

    // how much work was done
    state.SetBytesProcessed(int64_t(state.iterations())*s*s);

    // tear down
    // ...nothing to do
}
BENCHMARK(expression1_vigra)->Range(8, 8<<10);








BENCHMARK_MAIN();