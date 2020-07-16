#pragma once

#include "Math.hpp"
#include <xmmintrin.h>
#include <immintrin.h>

namespace Egliss::XMath
{
	static __forceinline constexpr unsigned int XMask(int i1, int i2, int i3, int i4)
	{
		return i1 | i2 << 2 | i3 << 4 | i4 << 6;
	}
#pragma region Values
	extern const __declspec(selectany) __m128 Zero4
	{
		0.0f,0.0f,0.0f,0.0f
	};
	extern const __declspec(selectany) __m256 Zero8
	{
		0.0f,0.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f,0.0f
	};
	extern const __declspec(selectany) __m128 One4
	{
		0.0f,0.0f,0.0f,0.0f
	};
	extern const __declspec(selectany) __m256 One8
	{
		0.0f,0.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f,0.0f
	};
	extern const __declspec(selectany) __m128 Radians4
	{
		Math::PI<float> / 180.0f,Math::PI<float> / 180.0f ,Math::PI<float> / 180.0f ,Math::PI<float> / 180.0f
	};
	extern const __declspec(selectany) __m256 Radians8
	{
		Math::PI<float> / 180.0f,Math::PI<float> / 180.0f ,Math::PI<float> / 180.0f ,Math::PI<float> / 180.0f ,
		Math::PI<float> / 180.0f,Math::PI<float> / 180.0f ,Math::PI<float> / 180.0f ,Math::PI<float> / 180.0f
	};
	extern const __declspec(selectany) __m128 Degree4
	{
		180.0f / Math::PI<float> ,180.0f / Math::PI<float>,180.0f / Math::PI<float>,180.0f / Math::PI<float>
	};
	extern const __declspec(selectany) __m256 Degree8
	{
		180.0f / Math::PI<float> ,180.0f / Math::PI<float>,180.0f / Math::PI<float>,180.0f / Math::PI<float>,
		180.0f / Math::PI<float> ,180.0f / Math::PI<float>,180.0f / Math::PI<float>,180.0f / Math::PI<float>
	};
#pragma endregion

#pragma region FunctionWrapper

	static __forceinline __m128 __vectorcall Add(__m128 A, __m128 B)
	{
		return _mm_add_ps(A, B);
	}
	static __forceinline __m128 __vectorcall Sub(__m128 A, __m128 B)
	{
		return _mm_sub_ps(A, B);
	}
	static __forceinline __m128 __vectorcall Mul(__m128 A, __m128 B)
	{
		return _mm_mul_ps(A, B);
	}
	static __forceinline __m128 __vectorcall Mul(__m128 A, float B)
	{
		__m128 scaler = { B,B,B,B };
		return _mm_mul_ps(A, scaler);
	}
	static __forceinline __m128 __vectorcall Div(__m128 A, __m128 B)
	{
		return _mm_div_ps(A, B);
	}
	static __forceinline __m128 __vectorcall AddSub(__m128 A, __m128 B)
	{
		return _mm_addsub_ps(A, B);
	}

	static __forceinline __m128 __vectorcall And(__m128 A, __m128 B)
	{
		return _mm_and_ps(A, B);
	}
	static __forceinline __m128 __vectorcall Or(__m128 A, __m128 B)
	{
		return _mm_or_ps(A, B);
	}
	static __forceinline __m128 __vectorcall Xor(__m128 A, __m128 B)
	{
		return _mm_xor_ps(A, B);
	}
	static __forceinline __m128 __vectorcall AndNot(__m128 A, __m128 B)
	{
		return _mm_andnot_ps(A, B);
	}

	template<unsigned int Mask>
	static __forceinline __m128 __vectorcall Shuffle(__m128 A, __m128 B)
	{
		return _mm_shuffle_ps(A, B, Mask);
	}
	template<unsigned int Mask>
	static __forceinline __m128 __vectorcall Blend(__m128 A, __m128 B)
	{
		return _mm_blend_ps(A, B, Mask);
	}

	static __forceinline __m128 __vectorcall ToRadians(__m128 degree)
	{
		return _mm_mul_ps(degree, Radians4);
	}
	static __forceinline __m128 __vectorcall ToDegree(__m128 radians)
	{
		return _mm_mul_ps(radians, Degree4);
	}

	static __forceinline __m256 __vectorcall ToRadians(__m256 degree)
	{
		return _mm256_mul_ps(degree, Radians8);
	}
	static __forceinline __m256 __vectorcall ToDegree(__m256 radians)
	{
		return _mm256_mul_ps(radians, Degree8);
	}

	static __forceinline __m128 __vectorcall Load(float value)
	{
		return _mm_load_ss(&value);
	}
	static __forceinline __m128 __vectorcall Load(const float* value)
	{
		return _mm_load_ss(value);
	}
	static __forceinline __m128 __vectorcall Load2(const float* ptr)
	{
		//Example In [1,2]
		__m128 x = Load(ptr);				//[1,0,0,0]
		__m128 y = Load(ptr + 1);			//[2,0,0,0]
		return _mm_unpacklo_ps(x, y);		//[1,2,0,0] -> (x0,y0,x1,y1)
	}
	static __forceinline __m128 __vectorcall Load3(const float* ptr)
	{
		//Example In [1,2,3]
		__m128 x = Load(ptr);				//[1,0,0,0]
		__m128 y = Load(ptr + 1);			//[2,0,0,0]
		__m128 z = Load(ptr + 2);			//[3,0,0,0]
		__m128 xy = _mm_unpacklo_ps(x, y);	//[1,2,0,0] -> (x0,y0,x1,y1)
		return _mm_movelh_ps(xy, z);		//[1,2,3,0] -> (x0,x1,y0,y1)
	}
	static __forceinline __m128 __vectorcall Load4(const float* ptr)
	{
		return _mm_loadu_ps(ptr);
	}
	static __forceinline void __vectorcall Store(float* f1, __m128 A)
	{
		_mm_store_ss(f1, A);
	}
	static __forceinline void __vectorcall Store2(float* f2, __m128 A)
	{
		__m128 y = Shuffle<XMask(1, 1, 1, 1)>(A, A);
		Store(f2, A);
		Store(f2 + 1, y);
	}
	static __forceinline void __vectorcall Store3(float* f3, __m128 A)
	{
		__m128 y = Shuffle<XMask(1, 1, 1, 1)>(A, A);
		__m128 z = Shuffle<XMask(2, 2, 2, 2)>(A, A);
		Store(f3, A);
		Store(f3 + 1, y);
		Store(f3 + 2, z);
	}
	static __forceinline void __vectorcall Store4(float* f4, __m128 A)
	{
		_mm_store_ps(f4, A);
	}
#pragma endregion

}