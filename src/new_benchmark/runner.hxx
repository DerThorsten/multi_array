#pragma once

#include <vector>
#include <string>
#include <iostream>

#include "timer.hxx"

namespace victoria{

    struct RunnerResult{
        std::vector<double> runtimes;
    };

    template<class F>
    inline RunnerResult runner(
        const size_t outerIterations,
        const size_t innerIterations,
        F && f
    ){
        RunnerResult runnerResult;

        Timer timer;
        for(auto outerIter=0; outerIter<outerIterations; ++outerIter){

            
            timer.start();
            for(auto innerIter=0; innerIter<innerIterations; ++innerIter){
                f();
            } 
            timer.stop();
            const auto elapsedSeconds = timer.elapsedSeconds();
            runnerResult.runtimes.push_back(elapsedSeconds/innerIterations);
            timer.reset();
        }
        return runnerResult;
    }

}