#pragma once

#include <vector>
#include <string>
#include <iostream>

#include "runner.hxx"

namespace victoria{




struct RunResult{
};


struct BenchmarkBase{
public:

    virtual ~BenchmarkBase(){}

    virtual RunResult run() = 0;
    virtual std::string name() = 0;

};





struct BenchmarkInfo{
    BenchmarkBase * benchmark;
};



using BenchmarkInfoVec = std::vector<BenchmarkInfo>;
extern BenchmarkInfoVec *  benchmarkInfoVecPtr;





inline BenchmarkInfo registerAndMakeBenchmarkInfo(
    BenchmarkBase * benchmark
){

    if(benchmarkInfoVecPtr == nullptr){
        benchmarkInfoVecPtr = new BenchmarkInfoVec();
    }  


    BenchmarkInfo benchmarkInfo;
    benchmarkInfo.benchmark = benchmark;

    benchmarkInfoVecPtr->push_back(benchmarkInfo);

    return benchmarkInfo;
}






inline void runAll(){
    if(benchmarkInfoVecPtr == nullptr){
        std::cout<<"no benchmark\n";
    }
    else{
        for(auto benchmarkInfoPtr : *benchmarkInfoVecPtr ){
            auto benchmark = benchmarkInfoPtr.benchmark;
            std::cout<<benchmark->name()<<"\n";

            auto runneRes = runner(
                10, // outer
                10, // inner
                [benchmark](){
                    benchmark->run();
                }
            );


        }
    }

}




}



#define VICTORIA_MAIN_VARS(BENCHMAR_NAME) \
    namespace victoria{ \
    BenchmarkInfoVec *  benchmarkInfoVecPtr = nullptr; \
    } \

