#pragma once

#include <sstream>
#include <chrono>
#include <string>

// heavily inspired by http://www.andres.sc/graph.html


namespace victoria{


class Timer{
public:
    Timer();

    double elapsedSeconds() const;
    Timer &  reset();
    void start();
    Timer &  stop();
    
    std::string toString() const;
    
private:
    double seconds_;

    decltype(std::chrono::high_resolution_clock::now()) timeObject_;
};




inline Timer::Timer(){
    reset();
}

inline double Timer::elapsedSeconds() const{
    return seconds_;
}

inline Timer & Timer::reset(){
    seconds_ = .0;
    return *this;
}

inline void Timer::start(){
    timeObject_ = std::chrono::high_resolution_clock::now();
}

// todo rename me
inline Timer &  Timer::stop(){
    typedef std::chrono::duration<double> DDouble;
    seconds_ += std::chrono::duration_cast<DDouble>(std::chrono::high_resolution_clock::now() - timeObject_).count();
    return *this;
}

inline std::string Timer::toString() const{
    const auto h = static_cast<uint64_t>(seconds_) / 3600;
    const auto m = (static_cast<uint64_t>(seconds_) - h*3600) / 60;
    const auto s = seconds_ - 3600.0*h - 60.0*m;
    std::ostringstream ss(std::ostringstream::out);
    ss <<h << "h " << m << "m " << s << "s";
    return ss.str();
}



} // end namespace victoria

