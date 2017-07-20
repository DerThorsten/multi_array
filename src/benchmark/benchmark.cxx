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
       this->a = ma::ones<int>(1000,1000);
       this->b = ma::ones<int>(1000,1000);
       this->c = ma::ones<int>(1000,1000);
       this->d = ma::ones<int>(1000,1000);
       this->pa = a.data(); 
       this->pb = b.data();
       this->pc = c.data();
       this->pd = d.data();
    }
    virtual void TearDown(){
        //delete this->SlowDeliveryMan;
    }
    ma::SmartMultiArray<int, 2> a;
    ma::SmartMultiArray<int, 2> b;
    ma::SmartMultiArray<int, 2> c;
    ma::SmartMultiArray<int, 2> d;
    int* pa;
    int* pb;
    int* pc;
    int* pd;
};



BENCHMARK_F(LargeArrayFixture, ExplicitCode, 10, 100)
{
    const auto size = a.size();
    for(auto s=0; s<size; ++s){
        pd[s] = (pa[s] + pb[s])*2 + (2+pc[s])*(2*pa[s])*(3*pb[s]);
    }
}


BENCHMARK_F(LargeArrayFixture, WithExpressions, 10, 100)
{
    d = (a + b)*2 + (2+c)*(2*a)*(3*b);
}


class VigraArrayFixture
    :   public ::hayai::Fixture
{
public:
    virtual void SetUp()
    {
       this->a = vigra::MultiArray<2,int>(vigra::Shape2(1000,1000));
       this->b = vigra::MultiArray<2,int>(vigra::Shape2(1000,1000));
       this->c = vigra::MultiArray<2,int>(vigra::Shape2(1000,1000));
       this->d = vigra::MultiArray<2,int>(vigra::Shape2(1000,1000));
       this->pa = &a(0); 
       this->pb = &b(0);
       this->pc = &c(0);
       this->pd = &d(0);
    }
    virtual void TearDown(){
        //delete this->SlowDeliveryMan;
    }
    vigra::MultiArray<2, int> a;
    vigra::MultiArray<2, int> b;
    vigra::MultiArray<2, int> c;
    vigra::MultiArray<2, int> d;
    int* pa;
    int* pb;
    int* pc;
    int* pd;
};





BENCHMARK_F(VigraArrayFixture, WithExpressions, 10, 100)
{
    using namespace vigra::multi_math;
    d = (a + b)*2 + (2+c)*(2*a)*(3*b);
}




int main()
{ 
    hayai::ConsoleOutputter consoleOutputter;
 
    hayai::Benchmarker::AddOutputter(consoleOutputter);
    hayai::Benchmarker::RunAllTests();
    return 0;
}