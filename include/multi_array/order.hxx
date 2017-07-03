#pragma once



namespace multi_array{

    // altitude may be altitude::high or altitude::low
    enum class Order : uint8_t
    { 
        C_ORDER=0,
        F_ORDER=1,
        A_ORDER=2,
        K_ORDER=3,
    }; 

}