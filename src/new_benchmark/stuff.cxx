
#include <chrono>
#include <iostream>
#include <memory>
#include <vector>

class BenchmarkInstanceBase{

};

void registerStuf(){

}


class BenchmarkRegistrySingleton
{
    public:
        static BenchmarkRegistrySingleton& getInstance()
        {
            static BenchmarkRegistrySingleton    instance; // Guaranteed to be destroyed.
                                  // Instantiated on first use.
            return instance;
        }
    private:
        BenchmarkRegistrySingleton() {}                    // Constructor? (the {} brackets) are needed here.


        // C++ 11
        // =======
        // We can use the better technique of deleting the methods
        // we don't want.
    public:
        BenchmarkRegistrySingleton(BenchmarkRegistrySingleton const&)               = delete;
        void operator=(BenchmarkRegistrySingleton const&)  = delete;

        // Note: Scott Meyers mentions in his Effective Modern
        //       C++ book, that deleted functions should generally
        //       be public as it results in better error messages
        //       due to the compilers behavior to check accessibility
        //       before deleted status
        void foo(){};

    std::vector< std::unique_ptr<BenchmarkInstanceBase> > benchmarks_;
};

//registerStuf();



struct MyBenchmarkInstance : BenchmarkInstanceBase{
    MyBenchmarkInstance(){
    }
};

//auto & r  = BenchmarkRegistrySingleton::getInstance()//.benchmarks_.emplace_back(new MyBenchmarkInstance());



BenchmarkRegistrySingleton & s = BenchmarkRegistrySingleton::getInstance();


int main()
{ 


    
    //hayai::ConsoleOutputter consoleOutputter;
    //hayai::Benchmarker::AddOutputter(consoleOutputter);
    //hayai::Benchmarker::RunAllTests();


    return 0;
}