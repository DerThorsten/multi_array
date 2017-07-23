#include "marray_benchmark.hxx"


// ARRAY IMPLS:

// this repo marray
#include "multi_array/multi_array.hxx"
namespace ma = multi_array;


// vigra (to compare with)
#include <vigra/multi_array.hxx>
#include <vigra/multi_math.hxx>

// blitz (to compare with)
#include <blitz/array.h>

// xtensor (to compare)
#include <xtensor/xtensor.hpp>
#include <xtensor/xarray.hpp>
#include <xtensor/xio.hpp>


ARRAY_BENCH(expr, ma){

    // setup
    const auto s = int64_t(state.range(0));
    auto a = ma::SmartMultiArray<T,DIM>(ma::Shape<DIM>(s));
    auto b = ma::SmartMultiArray<T,DIM>(ma::Shape<DIM>(s));
    auto c = ma::SmartMultiArray<T,DIM>(ma::Shape<DIM>(s));
    auto d = ma::SmartMultiArray<T,DIM>(ma::Shape<DIM>(s));
    auto e = ma::SmartMultiArray<T,DIM>(ma::Shape<DIM>(s));

    a = T(1);
    b = T(1);
    c = T(1);
    d = T(1);
   

    // run
    while (state.KeepRunning()){
        e = (a*2 + b*3 + 1)*2 + 4*(c + d+ 1);
        benchmark::DoNotOptimize(e);
    }

    // how much work was done
    state.SetBytesProcessed(int64_t(state.iterations())*std::pow(s,DIM));
};

REG_ARRAY_BENCH(expr, ma, 1, int)->RangeMultiplier(2)->Range(2<<19,2<<20);
//REG_ARRAY_BENCH(expr, ma, 2, int)->RangeMultiplier(2)->Range(2<<10,2<<10);

// REG_ARRAY_BENCH(expr, ma, 3, int)->RangeMultiplier(2)->Range(2,2<<3);
// REG_ARRAY_BENCH(expr, ma, 4, int)->RangeMultiplier(2)->Range(2,2<<3);





ARRAY_BENCH(expr, vigra){
    
    // setup
    const auto s = int64_t(state.range(0));
    auto a = vigra::MultiArray<DIM, T>((vigra::TinyVector<int, DIM>(s)));
    auto b = vigra::MultiArray<DIM, T>((vigra::TinyVector<int, DIM>(s)));
    auto c = vigra::MultiArray<DIM, T>((vigra::TinyVector<int, DIM>(s)));
    auto d = vigra::MultiArray<DIM, T>((vigra::TinyVector<int, DIM>(s)));
    auto e = vigra::MultiArray<DIM, T>((vigra::TinyVector<int, DIM>(s)));

    a = T(1);
    b = T(1);
    c = T(1);
    d = T(1);

    // run
    while (state.KeepRunning()){
        using namespace vigra::multi_math;
        e = (a*2 + b*3 + 1)*2 + 4*(c + d+ 1);
        benchmark::DoNotOptimize(e);
    }

    // how much work was done
    state.SetBytesProcessed(int64_t(state.iterations())*std::pow(s,DIM));
};

REG_ARRAY_BENCH(expr, vigra, 1, int)->RangeMultiplier(2)->Range(2<<19,2<<20);
//REG_ARRAY_BENCH(expr, vigra, 2, int)->RangeMultiplier(2)->Range(2<<10,2<<10);
// REG_ARRAY_BENCH(expr, vigra, 3, int)->RangeMultiplier(2)->Range(2,2<<3);
// REG_ARRAY_BENCH(expr, vigra, 4, int)->RangeMultiplier(2)->Range(2,2<<3);


ARRAY_BENCH(expr, blitz){
    
    // setup
    const auto s = int64_t(state.range(0));
    auto a = blitz::Array<T, DIM>(s);
    auto b = blitz::Array<T, DIM>(s);
    auto c = blitz::Array<T, DIM>(s);
    auto d = blitz::Array<T, DIM>(s);
    auto e = blitz::Array<T, DIM>(s);

    a = T(1);
    b = T(1);
    c = T(1);
    d = T(1);

    // run
    while (state.KeepRunning()){
        e = (a*2 + b*3 + 1)*2 + 4*(c + d+ 1);
        benchmark::DoNotOptimize(e);
    }

    // how much work was done
    state.SetBytesProcessed(int64_t(state.iterations())*std::pow(s,DIM));
};

REG_ARRAY_BENCH(expr, blitz, 1, int)->RangeMultiplier(2)->Range(2<<19,2<<20);
//REG_ARRAY_BENCH(expr, blitz, 2, int)->RangeMultiplier(2)->Range(2<<10,2<<10);
// REG_ARRAY_BENCH(expr, blitz, 3, int)->RangeMultiplier(2)->Range(2,2<<3);
// REG_ARRAY_BENCH(expr, blitz, 4, int)->RangeMultiplier(2)->Range(2,2<<3);






ARRAY_BENCH(expr, xt){

    // setup
    const auto s = int64_t(state.range(0));
    xt::xtensor<T, DIM> a = xt::ones<T>({s});
    xt::xtensor<T, DIM> b = xt::ones<T>({s});
    xt::xtensor<T, DIM> c = xt::ones<T>({s});
    xt::xtensor<T, DIM> d = xt::ones<T>({s});
    xt::xtensor<T, DIM> e = xt::ones<T>({s});

    // a = T(1);
    // b = T(1);
    // c = T(1);
    // d = T(1);

    // run
    while (state.KeepRunning()){
        e = (a*2 + b*3 + 1)*2 + 4*(c + d+ 1);
        benchmark::DoNotOptimize(e);
    }

    // how much work was done
    state.SetBytesProcessed(int64_t(state.iterations())*std::pow(s,DIM));
};

REG_ARRAY_BENCH(expr, xt, 1, int)->RangeMultiplier(2)->Range(2<<19,2<<20);
//REG_ARRAY_BENCH(expr, xt, 2, int)->RangeMultiplier(2)->Range(2<<10,2<<10);
// REG_ARRAY_BENCH(expr, blitz, 3, int)->RangeMultiplier(2)->Range(2,2<<3);
// REG_ARRAY_BENCH(expr, blitz, 4, int)->RangeMultiplier(2)->Range(2,2<<3);






ARRAY_BENCH(expr, opt){
    
    // setup
    const auto s = int64_t(state.range(0));
    const auto size = int64_t(std::pow(s,DIM));

    auto a = new T[size];
    auto b = new T[size];
    auto c = new T[size];
    auto d = new T[size];
    auto e = new T[size];

    // run
    while (state.KeepRunning()){
        for(auto i=0; i<size; ++i)
            e[i] = (a[i]*2 + b[i]*3 + 1)*2 + 4*(c[i] + d[i]+1);

    }

    // how much work was done
    state.SetBytesProcessed(int64_t(state.iterations())*std::pow(s,DIM));
    delete[] a;
    delete[] b;
    delete[] c;
    delete[] d;
    delete[] e;

};

REG_ARRAY_BENCH(expr, opt, 1, int)->RangeMultiplier(2)->Range(2<<19,2<<20);
//REG_ARRAY_BENCH(expr, opt, 2, int)->RangeMultiplier(2)->Range(2<<10,2<<10);



BENCHMARK_MAIN();