//
//  fundamentals.hpp
//  Bela Mirror
//
//  Created by Adam Pultz Melbye on 16.03.21.
//

#ifndef fundamentals_h
#define fundamentals_h

#include <random>

using namespace std;

template <class T> // Sign operator. 1 if x > 0, -1 if x < 0, 0 if x == 0
inline int
sgn(T v) {
    return (v > T(0)) - (v < T(0));
}

template<typename T>
T uniform_Rand_dec(T range_from, T range_to) {
    std::random_device                  rand_dev;
    std::mt19937                        generator(rand_dev());
    std::uniform_real_distribution<>    distr(range_from, range_to);
    return distr(generator);
}

template<typename T>
T uniform_Rand_int(T range_from, T range_to) {
    std::random_device                  rand_dev;
    std::mt19937                        generator(rand_dev());
    std::uniform_real_distribution<>    distr(range_from, range_to);
    return distr(generator);
}

// Wrap an integer to the range of 0 and max, using the bitwise & operation
constexpr int wrapMax(int val, int max)
{
    return val &= max;
}

static inline float mapLin(float x, float in_min, float in_max, float out_min, float out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

template <typename t>
t clamp2(t x, t min, t max)
{
if (x < min) x = min;
if (x > max) x = max;
return x;
}

constexpr float lin_interpol(float y1, float y2, float mu)
{
   return y1 + mu * (y2 - y1);
}


#endif /* fundamentals_h */
