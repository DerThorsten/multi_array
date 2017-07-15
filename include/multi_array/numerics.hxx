#include <limits>


namespace multi_array{

    /// TODO make better
    template<class T>
    struct Numerics{

        inline static T one(){
            return T(1);
        }
        inline static T zero(){
            return T(0);
        }
        inline static T max(){
            return std::numeric_limits<T>::max();
        }
        inline static T min(){
            return std::numeric_limits<T>::min();
        }
    };
}
