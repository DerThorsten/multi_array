#pragma once

#include <vector>

namespace victoria{


class BenchmarkBase{
public:
    virtual ~BenchmarkBase(){}
    virtual run() = 0

    

};




struct TestInfo{
    ~TestInfo(){}
};

using TestInfoPtrVec = std::vector<TestInfo*>;
extern TestInfoPtrVec *  gTestInfoPtrVec;



}


#define VICTORIA_MAIN(a) \
victoria::TestInfoPtrVec *  gTestInfoPtrVec = nullptr;





class BENCHMARK_NAME{
public:
    virtual ~BENCHMARK_NAME(){}
    virtual run() = 0
};