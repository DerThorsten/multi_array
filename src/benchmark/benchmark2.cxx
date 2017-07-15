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
       this->a = ma::ones<double>(300,200,100);
       this->b = ma::ones<double>(300,200,100);
       this->c = ma::ones<double>(300,200,100);
       this->d = ma::ones<double>(300,200,100);
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
        this->a = vigra::MultiArray<3,double>(vigra::Shape3(300,200,100));
        this->b = vigra::MultiArray<3,double>(vigra::Shape3(300,200,100));
        this->c = vigra::MultiArray<3,double>(vigra::Shape3(300,200,100));
        this->d = vigra::MultiArray<3,double>(vigra::Shape3(300,200,100));
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



BENCHMARK_F(LargeArrayFixture, Optimal, 10, 20)
{
    const auto size = a.size();
    for(auto s=0; s<size; ++s){
        pd[s] = 1.0 
            + std::log(std::exp(pa[s]*2.0 + pb[s]*3.0) + 2.0*pc[s]) 
            + std::sqrt(pa[s] + 1.0) * 2.0 
            +(2.0*pa[s])*(2.0*pb[s])*(2.0*pc[s]);
    }
}


BENCHMARK_F(LargeArrayFixture, MultiArray, 10, 20)
{
    d = 1.0 
        + ma::log(ma::exp(a*2.0 + b*3.0) + 2.0*c) 
        + ma::sqrt(a + 1.0) * 2.0 
        + (2.0*a)*(2.0*b)*(2.0*c);
}



BENCHMARK_F(VigraArrayFixture, Vigra, 10, 20)
{
    using namespace vigra::multi_math;
    namespace vma = vigra::multi_math;
    d = 1.0 
        + vma::log(vma::exp(a*2.0 + b*3.0) + 2.0*c) 
        + vma::sqrt(a+1.0) * 2.0 
        + (2.0*a)*(2.0*b)*(2.0*c);
}




int main()
{ 
    hayai::ConsoleOutputter consoleOutputter;
 
    hayai::Benchmarker::AddOutputter(consoleOutputter);
    hayai::Benchmarker::RunAllTests();
    return 0;
}