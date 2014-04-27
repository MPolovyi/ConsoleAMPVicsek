#pragma once

#include <numeric>

#include <amp.h>
#include <amp_math.h>
#include <amp_graphics.h>

using namespace concurrency;
using namespace concurrency::graphics;

namespace MathHelpers
{
	const float SqrLength(const float_4& r) restrict(amp, cpu);
	const float SqrLength(const float_3& r) restrict(amp, cpu);
	const float SqrLength(const float_2& r) restrict(amp, cpu);
	const float Length(const float_4& r) restrict(amp, cpu);
	const float Length(const float_3& r) restrict(amp, cpu);
	const float Length(const float_2& r) restrict(amp, cpu);
	float_3		CrossProduct(float_3 A, float_3 B) restrict(amp);
	float		DotProduct(float_3 A, float_3 B) restrict(amp);
	void		RotateVector2D(float_3& vect, float ang) restrict(amp);
	void		RotateVector2D(float_2& vect, float ang) restrict(amp);
	void		RotateVector(float_3& vect, float_3& ax, float ang) restrict(amp);
	void		NormalizeVector(float_3& vect) restrict(amp);
	void		NormalizeVector(float_2& vect) restrict(amp);
	float_3		CountAverageVector(std::vector<float_3>& vectors);
	float_3		CountAverageVector(array<float_3>& src, uint count);
	float_3		AccumulateValue(array<float_3>& src, uint count);
}