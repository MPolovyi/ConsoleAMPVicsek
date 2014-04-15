// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace concurrency;
using namespace concurrency::graphics;

void RandomDevice();
void AMP();
void RandRotation(float_3& vel, float rndX, float rndY, int noize) restrict(amp);
void RotateVector(float_3& vect, float_3& ax, float ang) restrict(amp);


int _tmain(int argc, _TCHAR* argv[])
{

	AMP();
	return 0;
}

void RandRotation(float_3& vel, float rndX, float rndY, int noize) restrict(amp)
{
	float rndZ = 0;
	if (vel.z > 0.00001)
	{
		auto prod = vel.xy * float_2(rndX, rndY);
		rndZ = (prod.x + prod.y) / vel.z;
	}
	float_3 rot(rndX, rndY, rndZ);

	RotateVector(vel, rot, noize);
}

void RotateVector(float_3& vect, float_3& ax, float ang) restrict(amp)
{

	auto c = concurrency::fast_math::cos(ang);
	auto s = fast_math::sin(ang);
	auto c1 = 1 - c;

	auto c1xy = c1*ax.x*ax.y;
	auto c1xz = c1*ax.x*ax.z;
	auto c1yz = c1*ax.y*ax.z;

	auto sx = s*ax.x;
	auto sy = s*ax.y;
	auto sz = s*ax.z;

	//Rotation matrix
	float R11(c + c1 * ax.x*ax.x), R12(c1xy + sz), R13(c1xz - sy);
	float R21(c1xy - sz), R22(c + c1*ax.y*ax.y), R23(c1yz + sx);
	float R31(c1xz + sy), R32(c1yz - sx), R33(c + c1*ax.z*ax.z);

	float_3 ans(R11*vect.x + R12*vect.y + R13*vect.z, R21*vect.x + R22*vect.y + R23*vect.z, R31*vect.x + R32*vect.y + R33*vect.z);
	vect = ans;
}

void RandomDevice()
{
	std::random_device rd;
	std::map<int, int> hist;
	std::uniform_int_distribution<int> dist(0, 9);
	for (int n = 0; n < 20000; ++n) {
		++hist[dist(rd)]; // note: demo only: the performance of many 
		// implementations of random_device degrades sharply
		// once the entropy pool is exhausted. For practical use
		// random_device is generally only used to seed 
		// a PRNG such as mt19937
	}
	for (auto p : hist) {
		std::cout << p.first << " : " << std::string(p.second / 100, '*') << '\n';
	}
}

float reduction_simple_1(const std::vector<float>& source)
{
	assert(source.size() <= UINT_MAX);
	unsigned element_count = static_cast<unsigned>(source.size());
	assert(element_count != 0); // Cannot reduce an empty sequence.
	if (element_count == 1)
	{
		return source[0];
	}

	// Using array, as we mostly need just temporary memory to store
	// the algorithm state between iterations and in the end we have to copy
	// back only the first element.
	array<float, 1> a(element_count, source.begin());

	// Takes care of odd input elements – we could completely avoid tail sum
	// if we would require source to have even number of elements.
	float tail_sum = (element_count % 2) ? source[element_count - 1] : 0;
	array_view<float, 1> av_tail_sum(1, &tail_sum);

	// Each thread reduces two elements.
	for (unsigned s = element_count / 2; s > 0; s /= 2)
	{
		parallel_for_each(extent<1>(s), [=, &a](index<1> idx) restrict(amp)
		{
			a[idx] = a[idx] + a[idx + s];

			// Reduce the tail in cases where the number of elements is odd.
			if ((idx[0] == s - 1) && (s & 0x1) && (s != 1))
			{
				av_tail_sum[0] += a[s - 1];
			}
		});
	}

	// Copy the results back to CPU.
	std::vector<float> result(1);
	copy(a.section(0, 1), result.begin());
	av_tail_sum.synchronize();

	return result[0] + tail_sum;
}

void AMP()
{
	std::vector<float> vect1({ 1.1f, 1.2f, 1.3f });
	std::vector<float> vect2({ 2.1f, 2.2f, 2.3f });
	std::vector<float> vect3({ 0, 0, 0 });
	array_view<float, 1> arr1(vect1);
	array_view<float, 1> arr2(vect2);
	array_view<float, 1> arr3(vect3);

	accelerator defaultDevice(accelerator::default_accelerator);
	accelerator_view defaultView = defaultDevice.default_view;

	std::wcout << L" Using device : " << defaultDevice.get_description() << std::endl;

	concurrency::parallel_for_each(accelerator(accelerator::default_accelerator).get_default_view(), arr2.extent, [=](concurrency::index<1> ind) restrict(amp)
	{
		arr1[ind] = arr2[ind] + arr2[ind];
		float_3 a(1, 0, 0);
		float_3 b(0, 1, 1);
		RotateVector(a, b, 90);
	});
	arr3.synchronize();
	std::wcout << reduction_simple_1(vect2) << std::endl;
}

