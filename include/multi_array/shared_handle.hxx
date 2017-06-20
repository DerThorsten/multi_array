#pragma once


#include <iostream>
#include <memory>





namespace multi_array{


///\cond
namespace detail_multi_array{
    
    class SharedHandle{
    private:
        template<class T>
        struct ArrayDeleter{
            ArrayDeleter(T * ptr)
            : ptr_(ptr){

            }
            template<class H>
            void operator()(H * handle) {
                delete[] ptr_;
                delete handle;
            }
            T * ptr_;
        };



        struct HandleType{};
        typedef std::shared_ptr<HandleType> SharedHandlePtr;
    public:

        struct AllocNewTag{};
        struct CopyHandleTag{};

        // empty array
        SharedHandle()
        : handle_(){
        }


        // alloc new mem
        template<class U>
        SharedHandle(U * ptr, const AllocNewTag & )
        : handle_(new HandleType(), ArrayDeleter<U>(ptr)){
        }


        // copy handle => view
        SharedHandle(const SharedHandle & other, const CopyHandleTag &)
        : handle_(other.handle_){
        }

        long useCount() const{
            return handle_.use_count();
        }


    private:    
        SharedHandlePtr handle_;
    };
} // end namespace multi_array::detail_multi_array
///\encdond

} 
