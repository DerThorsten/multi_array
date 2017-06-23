
#pragma once


#include <tuple>


namespace multi_array{
namespace meta{

    template<class ... ARGS> 
    struct VariadicArgumentsSize
    : public std::tuple_size<
        std::tuple<ARGS ... >
    >{

    };



}
}