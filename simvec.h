#ifndef SIMVEC_H_INCLUDED
#define SIMVEC_H_INCLUDED

#include "inugami/math.h"

template <typename T>
class SimVec
{
public:
    SimVec() : x(0), y(0) {}
    SimVec(const T& in) : x(in), y(in) {}
    SimVec(const T& inX, const T& inY) : x(inX), y(inY) {}
    SimVec(const SimVec& in) : x(in.x), y(in.y) {}

    SimVec operator+(const SimVec& in)
    {
        return {x+in.x, y+in.y};
    }

    SimVec operator-(const SimVec& in)
    {
        return {x-in.x, y-in.y};
    }

    SimVec operator+(const T& in)
    {
        return {x+in, y+in};
    }

    SimVec operator*(const T& in)
    {
        return {x*in, y*in};
    }

    SimVec operator/(const T& in)
    {
        return {x/in, y/in};
    }

    SimVec& operator+=(const SimVec& in)
    {
        x+=in.x; y+=in.y;
        return *this;
    }

    SimVec& operator-=(const SimVec& in)
    {
        x-=in.x; y-=in.y;
        return *this;
    }

    SimVec& operator/=(const SimVec& in)
    {
        x/=in.x; y/=in.y;
        return *this;
    }

    SimVec& operator*=(const SimVec& in)
    {
        x*=in.x; y*=in.y;
        return *this;
    }

    SimVec& operator*=(const T& in)
    {
        x*=in; y*=in;
        return *this;
    }

    SimVec& operator/=(const T& in)
    {
        x/=in; y/=in;
        return *this;
    }

    T length()
    {
        return std::sqrt(x*x+y*y);
    }

    T x, y;
};

#endif // SIMVEC_H_INCLUDED
