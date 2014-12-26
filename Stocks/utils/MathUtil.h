#ifndef _MATH_UTIL_H_
#define _MATH_UTIL_H_

#include <cctype>
#include <string>

namespace MathUtil
{
	inline double difference(double val1, double val2)
	{
		if(val1 == val2)
		{
			return 0.0;
		}
		return 100.0 * (abs(val1 - val2) / ((val1 + val2) / 2));
	}


	template<typename T>
	inline T clamp(const T& val, const T& minVal, const T& maxVal)
	{
		if(val < minVal)
		{
			return minVal;
		}
		if(val > maxVal)
		{
			return maxVal;
		}
		return val;
	}

	template<typename T>
	inline T lerp(T a, T b, float percent)
	{
		if(percent >= 1.0f)
		{
			return b;
		}
		if(percent <= 0.0f)
		{
			return a;
		}
		return a + percent * (b - a);
	}

	inline float round(float a)
	{
		int fa;
		if(a >= 0)
		{
			fa = (int)floorf(a);
		}
		else
		{
			fa = (int)ceilf(a);
		}

		if(fabs(a - fa) > 0.4f)
		{
			return (float)(fa + 1);
		}
		else
		{
			return (float)(fa);
		}
	}

	inline float fract(float x)
	{
		return x - floorf(x);
	}

};
#endif
