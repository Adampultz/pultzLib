/*
Fundamental operations for running the algorithms in pultzLib
 */

#ifndef fundamentals_h
#define fundamentals_h

#include <random>

using namespace std;

/* Random floating point number generator */
template<typename T>
T uniform_Rand_dec(T range_from, T range_to) {
    std::random_device                  rand_dev;
    std::mt19937                        generator(rand_dev());
    std::uniform_real_distribution<>    distr(range_from, range_to);
    return distr(generator);
}

/* Random integer number generator */
template<typename T>
T uniform_Rand_int(T range_from, T range_to) {
    std::random_device                  rand_dev;
    std::mt19937                        generator(rand_dev());
    std::uniform_real_distribution<>    distr(range_from, range_to);
    return distr(generator);
}

/* Constrict a value to min-max */
template <typename t>
t clamp2(t x, t min, t max)
{
if (x < min) x = min;
if (x > max) x = max;
return x;
}

/* Compute the fractional value of a zero-crossing */
template <typename t>
t frac_ZC(int x_m1, float y, float y_m1) {
t slope = y - y_m1;
t b = (slope * x_m1) - y_m1;
t xIntCept = b / slope;
return xIntCept;
}

/* Sign operator. 1 if x > 0, -1 if x < 0, 0 if x == 0 */
template <class T>
inline int
sgn(T v) {
    return (v > T(0)) - (v < T(0));
}

/* Linear interpolation */
constexpr float lin_interpol(float y1, float y2, float mu)
{
   return y1 + mu * (y2 - y1);
}

/* Wrap an integer to the range of 0 and max, using the bitwise & operation */
constexpr int wrapMax(int val, int max)
{
    return val &= max;
}

/* Linearly map a value to the range of min-max. Does not use constrict output */
static inline float mapLin(float x, float in_min, float in_max, float out_min, float out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/* Scale a value logarithmically */
static inline float log_scale(float y1, float shape)
{
	float x1 = 1.0 / (exp(shape) - 1.0);
	 return (exp(y1 * shape) - 1.0) * x1;
}


#endif /* fundamentals_h */
