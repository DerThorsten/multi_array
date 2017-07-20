#include "victoria.hxx"

namespace vic = victoria;


VICTORIA_MAIN_VARS(MY_BENCHMARK)




struct FooBenchmark : vic::BenchmarkBase{
    virtual ~FooBenchmark(){}
    victoria::RunResult run(){
        std::cout<<"do foo\n";
        return vic::RunResult();
    }
    std::string name(){
        return "FooBenchmark";
    }
};


const vic::BenchmarkInfo a = vic::registerAndMakeBenchmarkInfo(
    new FooBenchmark()
);


int main()
{ 
    vic::runAll();

}