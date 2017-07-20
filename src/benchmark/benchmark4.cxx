#include <hayai.hpp>


#include "multi_array/multi_array.hxx"


// vigra (to compare with)
#include <vigra/multi_array.hxx>
#include <vigra/multi_math.hxx>




namespace ma = multi_array;

// multi_array 



class LargeArrayFixture
    :   public ::hayai::Fixture
{
public:
    virtual void SetUp()
    {
       this->a = ma::ones<double>(30,200,200);
       this->b = ma::ones<double>(30,200,200);
       this->c = ma::ones<double>(30,200,200);
       this->d = ma::ones<double>(30,200,200);
       this->pa = a.data(); 
       this->pb = b.data();
       this->pc = c.data();
       this->pd = d.data();
    }
    virtual void TearDown(){
        //delete this->SlowDeliveryMan;
    }
    ma::SmartMultiArray<double, 3> a;
    ma::SmartMultiArray<double, 3> b;
    ma::SmartMultiArray<double, 3> c;
    ma::SmartMultiArray<double, 3> d;
    double* pa;
    double* pb;
    double* pc;
    double* pd;
};
class VigraArrayFixture
    :   public ::hayai::Fixture
{
public:
    virtual void SetUp()
    {
        this->a = vigra::MultiArray<3,double>(vigra::Shape3(30,200,200));
        this->b = vigra::MultiArray<3,double>(vigra::Shape3(30,200,200));
        this->c = vigra::MultiArray<3,double>(vigra::Shape3(30,200,200));
        this->d = vigra::MultiArray<3,double>(vigra::Shape3(30,200,200));
        a = 1.0;
        b = 1.0;
        c = 1.0;
        d = 1.0;
        
        this->pa = &a(0); 
        this->pb = &b(0);
        this->pc = &c(0);
        this->pd = &d(0);
    }
    virtual void TearDown(){
        //delete this->SlowDeliveryMan;
    }
    vigra::MultiArray<3, double> a;
    vigra::MultiArray<3, double> b;
    vigra::MultiArray<3, double> c;
    vigra::MultiArray<3, double> d;
    double* pa;
    double* pb;
    double* pc;
    double* pd;
};



BENCHMARK_F(LargeArrayFixture, Optimal, 15, 40)
{
    const auto size = a.size();
    for(auto s=0; s<size; ++s){
        pd[s] = ((pa[s]+2)+(pb[s]+2)) + ((pa[s]+2)+(pc[s]+2)) + 
            ((pa[s]-2)+(pb[s]-2)) + ((pa[s]-2)+(pc[s]-2));
    }
}


BENCHMARK_F(LargeArrayFixture, MultiArray, 15, 40)
{
    d = ((a+2)+(b+2)) + ((a+2)+(c+2)) +
        ((a-2)+(b-2)) + ((a-2)+(c-2));
}



BENCHMARK_F(VigraArrayFixture, Vigra, 15, 40)
{
    using namespace vigra::multi_math;
    namespace vma = vigra::multi_math;
    d = ((a+2)+(b+2)) + ((a+2)+(c+2)) +
        ((a-2)+(b-2)) + ((a-2)+(c-2));
}




int main()
{ 
    hayai::ConsoleOutputter consoleOutputter;
 
    hayai::Benchmarker::AddOutputter(consoleOutputter);
    hayai::Benchmarker::RunAllTests();
    return 0;
}