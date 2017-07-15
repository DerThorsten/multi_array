#pragma once


#include <iostream>
#include <memory>
#include "multi_array/pre_exisiting_memory_poliy.hxx"




namespace multi_array{


///\cond
namespace detail_multi_array{
    
    class SharedHandle{
    private:


        template<class PTR_TYPE>
        struct ArrayDeleter{
            ArrayDeleter(PTR_TYPE ptr)
            : ptr_(ptr){

            }
            template<class H>
            void operator()(H * handle) {
                delete[] ptr_;
                delete handle;
            }
            PTR_TYPE ptr_;
        };


        template<class PTR_TYPE>
        struct FunctionCallingDeleter{
            FunctionCallingDeleter(
                PTR_TYPE ptr,
                const std::function<void(PTR_TYPE)> & f
            )
            :   ptr_(ptr),
                f_(f){

            }
            template<class H>
            void operator()(H * handle) {
                f(ptr_);
                delete handle;
            }
            PTR_TYPE ptr_;
            std::function<void(PTR_TYPE)>  f_;
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
        template<class PTR_TYPE>
        SharedHandle(PTR_TYPE ptr, const AllocNewTag & )
        : handle_(new HandleType(), ArrayDeleter<PTR_TYPE>(ptr)){
        }


        // copy handle => view
        SharedHandle(const SharedHandle & other, const CopyHandleTag &)
        : handle_(other.handle_){
        }


        // PreExistingMemoryPolicy
        template<class PTR_TYPE>
        SharedHandle(PTR_TYPE ptr, const PreExistingMemoryPolicy & preExistingMemoryPolicy)
        : handle_(){
            if(preExistingMemoryPolicy == DELETE_DATA_WHEN_DONE){
                handle_ = SharedHandlePtr(new HandleType(),ArrayDeleter<PTR_TYPE>(ptr));
            }
            else{
                handle_ = SharedHandlePtr(new HandleType());
            }
        }
        // call functor when refcount goes to zero
        template<class PTR_TYPE>
        SharedHandle(PTR_TYPE ptr, const std::function<void(PTR_TYPE)> & f)
        : handle_(new HandleType(),FunctionCallingDeleter<PTR_TYPE>(ptr,f)){
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
