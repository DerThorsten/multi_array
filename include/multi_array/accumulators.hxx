#pragma once

#include <cmath>

#include "multi_array/meta/promote_real.hxx"
#include "multi_array/numerics.hxx"

namespace multi_array{
    namespace accumulators{


        template<class T>
        class Sum{
        public:
            /// result type
            typedef T type;
            Sum(const T & initalValue = Numerics<T>::zero())
            :   sum_(initalValue){
            }
            void operator()(const T & val){
                sum_ += val;
            }
            type result()const{
                return sum_;
            }
        private:
            T sum_;
        };

        template<class T>
        class Min{
        public:
            typedef T type;
            Min(const T & initalValue = Numerics<T>::max())
            :   min_(initalValue){
            }
            void operator()(const T & val){
                min_ = std::min(val, min_);
            }
            type result()const{
                return min_;
            }
        private:
            T min_;
        };
        template<class T>
        class Max{
        public:
            typedef T type;
            Max(const T & initalValue = Numerics<T>::min())
            :   max_(initalValue){
            }
            void operator()(const T & val){
                max_ = std::max(val, max_);
            }
            type result()const{
                return max_;
            }
        private:
            T max_;
        };
        template<class T>
        class MinMax{
        public:
            typedef std::pair<T,T> type;
            MinMax(
                const T & initalMinValue = Numerics<T>::max(),
                const T & initalMaxValue = Numerics<T>::min()
            )
            :   min_(initalMinValue),
                max_(initalMaxValue){
            }
            void operator()(const T & val){
                min_ = std::min(val, min_);
                max_ = std::max(val, max_);
            }
            type result()const{
                return type(min_, max_);
            }
        private:
            T min_;
            T max_;
        };

        template<class T>
        class Mean{
        public:
            /// result type
            typedef typename meta::PromoteReal<T>::type type;

            Mean()
            :   sum_(Numerics<T>::zero()),
                count_(0){
            }

            void operator()(const T & val){
                sum_ += val;
                count_ += 1;
            }

            // accumulators return by value
            type result()const{
                return static_cast<type>(sum_)/
                    static_cast<type>(count_);
            }
        private:
            T sum_;
            uint64_t count_;
        };

    }
}