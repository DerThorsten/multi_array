
#include "victoria.hxx"

#include "multi_array/multi_array.hxx"


namespace vic = victoria;


VICTORIA_MAIN_VARS(MY_BENCHMARK)




struct FooBenchmark : vic::BenchmarkBase{
    virtual ~FooBenchmark(){}
    victoria::RunResult run(){
        this->a = ma::ones<double>(30,200,100);
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