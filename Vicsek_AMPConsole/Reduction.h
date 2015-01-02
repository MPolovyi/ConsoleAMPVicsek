//////////////////////////////////////////////////////////////////////////////
//// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//// PARTICULAR PURPOSE.
////
//// Copyright (c) Microsoft Corporation. All rights reserved
//////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------
// File: Reduction.cpp
//
// Implements several different versions of reduction algorithm in C++ AMP.
// Each consequent version demonstrates optimization techniques to speed up
// the execution. All of the algorithms are tailored to 4-byte wide data and
// may require tuning when different element type is to be used.
// The content of the GPU-side data structures is lost during the algorithm
// processing.
// Note: these implementations are for demo purposes only and not recommended
// to use in production code.
//----------------------------------------------------------------------------

#define NOMINMAX
#include <amp.h>
#include <iostream>
#include <numeric>
#include <assert.h>

using namespace concurrency;
using concurrency::graphics::float_3;
namespace MathHelpers
{
	//----------------------------------------------------------------------------
	// Helper macro to tell whether the argument is a positive integer
	// power of two.
	//----------------------------------------------------------------------------
#ifndef IS_POWER_OF_2(arg)
#define IS_POWER_OF_2(arg) (arg > 1 && (arg & (arg - 1)) == 0)
#endif
	//----------------------------------------------------------------------------
	// Helper function checking the common preconditions for tiled algorithms.
	// If the conditions are not met, the implementations will fall back to CPU
	// reduction after some number of iterations using parallel_for_each.
	// As it may be unexpected, a warning message will be issued.
	// Addressing these limitations is left as an exercise for the reader.
	//----------------------------------------------------------------------------
	inline bool check_tiled_precondition(unsigned tile_size, unsigned element_count)
	{
		while ((element_count % tile_size) == 0)
		{
			element_count /= tile_size;
		}
		return element_count < tile_size;
	}

	//----------------------------------------------------------------------------
	// This is a simple sequential implementation.
	//----------------------------------------------------------------------------
	struct CReduction
	{
		//----------------------------------------------------------------------------
		// This is a version with a reduced number of stalled threads in the first
		// iteration.
		//----------------------------------------------------------------------------
		template <unsigned _tile_size>
		static float reduction_tiled_4(array<float, 1>& arr_1, size_t element_count)
		{
			static_assert(IS_POWER_OF_2(_tile_size), "Tile size must be a positive integer power of two!");

			assert(element_count != 0); // Cannot reduce an empty sequence.

			if (!check_tiled_precondition(_tile_size * 2, element_count))
			{
				std::cout << "Warning, reduction_tiled_4 is not designed for the current problem size." << std::endl;
			}

			// Using arrays as temporary memory.
			array<float, 1> arr_2((element_count / _tile_size) ? (element_count / _tile_size) : 1);

			// array_views may be swapped after each iteration.
			array_view<float, 1> av_src(arr_1);
			array_view<float, 1> av_dst(arr_2);
			av_dst.discard_data();

			// Reduce using parallel_for_each as long as the sequence length
			// is evenly divisable to twice the number of threads in the tile
			// (note each tile works on a problem size twice its own size).
			while (element_count >= _tile_size
				   && (element_count % (_tile_size * 2)) == 0)
			{
				parallel_for_each(extent<1>(element_count / 2).tile<_tile_size>(), [=](tiled_index<_tile_size> tidx) restrict(amp) {
					// Use tile_static as a scratchpad memory.
					tile_static float tile_data[_tile_size];

					unsigned local_idx = tidx.local[0];

					// Partition input data among tiles,
					// 2 * _tile_size because the number of threads spawned is halved.
					unsigned rel_idx = tidx.tile[0] * (_tile_size * 2) + local_idx;
					tile_data[local_idx] = av_src[rel_idx] + av_src[rel_idx + _tile_size];
					tidx.barrier.wait();

					// Reduce within a tile using multiple threads.
					for (unsigned s = _tile_size / 2; s > 0; s /= 2)
					{
						if (local_idx < s)
						{
							tile_data[local_idx] += tile_data[local_idx + s];
						}

						tidx.barrier.wait();
					}

					// Store the tile result in the global memory.
					if (local_idx == 0)
					{
						av_dst[tidx.tile] = tile_data[0];
					}
				});

				// Update the sequence length, swap source with destination.
				element_count /= _tile_size * 2;
				std::swap(av_src, av_dst);
				av_dst.discard_data();
			}

			// Perform any remaining uneven reduction on the CPU.
			std::vector<float> result(element_count);
			copy(av_src.section(0, element_count), result.begin());
			return std::accumulate(result.begin(), result.end(), 0.f);
		}

		template <unsigned _tile_size>
		static float reduction_tiled_4(const std::vector<float>& source)
		{
			assert(source.size() <= UINT_MAX);
			unsigned element_count = static_cast<unsigned>(source.size());
			assert(element_count != 0); // Cannot reduce an empty sequence.

			// Using arrays as temporary memory.
			array<float, 1> arr_1(element_count, source.begin());
			return reduction_tiled_4<_tile_size>(arr_1, element_count);
		}

		template <unsigned _tile_size>
		static float_2 reduction_tiled_4(array<float_2, 1>& arr_1, size_t element_count)
		{
			static_assert(IS_POWER_OF_2(_tile_size), "Tile size must be a positive integer power of two!");

			assert(element_count != 0); // Cannot reduce an empty sequence.

			if (!check_tiled_precondition(_tile_size * 2, element_count))
			{
				std::cout << "Warning, reduction_tiled_4 is not designed for the current problem size." << std::endl;
			}

			// Using arrays as temporary memory.
			array<float_2, 1> arr_2((element_count / _tile_size) ? (element_count / _tile_size) : 1);

			// array_views may be swapped after each iteration.
			array_view<float_2, 1> av_src(arr_1);
			array_view<float_2, 1> av_dst(arr_2);
			av_dst.discard_data();

			// Reduce using parallel_for_each as long as the sequence length
			// is evenly divisable to twice the number of threads in the tile
			// (note each tile works on a problem size twice its own size).
			while (element_count >= _tile_size
				   && (element_count % (_tile_size * 2)) == 0)
			{
				parallel_for_each(extent<1>(element_count / 2).tile<_tile_size>(), [=](tiled_index<_tile_size> tidx) restrict(amp) {
					// Use tile_static as a scratchpad memory.
					tile_static float_2 tile_data[_tile_size];

					unsigned local_idx = tidx.local[0];

					// Partition input data among tiles,
					// 2 * _tile_size because the number of threads spawned is halved.
					unsigned rel_idx = tidx.tile[0] * (_tile_size * 2) + local_idx;
					tile_data[local_idx] = av_src[rel_idx] + av_src[rel_idx + _tile_size];
					tidx.barrier.wait();

					// Reduce within a tile using multiple threads.
					for (unsigned s = _tile_size / 2; s > 0; s /= 2)
					{
						if (local_idx < s)
						{
							tile_data[local_idx] += tile_data[local_idx + s];
						}

						tidx.barrier.wait();
					}

					// Store the tile result in the global memory.
					if (local_idx == 0)
					{
						av_dst[tidx.tile] = tile_data[0];
					}
				});

				// Update the sequence length, swap source with destination.
				element_count /= _tile_size * 2;
				std::swap(av_src, av_dst);
				av_dst.discard_data();
			}

			// Perform any remaining uneven reduction on the CPU.
			std::vector<float_2> result(element_count);
			copy(av_src.section(0, element_count), result.begin());
			return std::accumulate(result.begin(), result.end(), float_2(0.f));
		}

		template <unsigned _tile_size>
		static float_2 reduction_tiled_4(const std::vector<float_2>& source)
		{
			assert(source.size() <= UINT_MAX);
			unsigned element_count = static_cast<unsigned>(source.size());
			assert(element_count != 0); // Cannot reduce an empty sequence.

			// Using arrays as temporary memory.
			array<float_2, 1> arr_1(element_count, source.begin());
			return reduction_tiled_4<_tile_size>(arr_1, element_count);
		}

		//----------------------------------------------------------------------------
		// This is a version with a reduced number of stalled threads in the first
		// iteration.
		//----------------------------------------------------------------------------
		template <unsigned _tile_size>
		static float_3 reduction_tiled_4(array<float_3, 1>& arr_1, size_t element_count)
		{
			static_assert(IS_POWER_OF_2(_tile_size), "Tile size must be a positive integer power of two!");

			assert(element_count != 0); // Cannot reduce an empty sequence.

			if (!check_tiled_precondition(_tile_size * 2, element_count))
			{
				std::cout << "Warning, reduction_tiled_4 is not designed for the current problem size." << std::endl;
			}

			// Using arrays as temporary memory.
			array<float_3, 1> arr_2((element_count / _tile_size) ? (element_count / _tile_size) : 1);

			// array_views may be swapped after each iteration.
			array_view<float_3, 1> av_src(arr_1);
			array_view<float_3, 1> av_dst(arr_2);
			av_dst.discard_data();

			// Reduce using parallel_for_each as long as the sequence length
			// is evenly divisable to twice the number of threads in the tile
			// (note each tile works on a problem size twice its own size).
			while (element_count >= _tile_size
				   && (element_count % (_tile_size * 2)) == 0)
			{
				parallel_for_each(extent<1>(element_count / 2).tile<_tile_size>(), [=](tiled_index<_tile_size> tidx) restrict(amp) {
					// Use tile_static as a scratchpad memory.
					tile_static float_3 tile_data[_tile_size];

					unsigned local_idx = tidx.local[0];

					// Partition input data among tiles,
					// 2 * _tile_size because the number of threads spawned is halved.
					unsigned rel_idx = tidx.tile[0] * (_tile_size * 2) + local_idx;
					tile_data[local_idx] = av_src[rel_idx] + av_src[rel_idx + _tile_size];
					tidx.barrier.wait();

					// Reduce within a tile using multiple threads.
					for (unsigned s = _tile_size / 2; s > 0; s /= 2)
					{
						if (local_idx < s)
						{
							tile_data[local_idx] += tile_data[local_idx + s];
						}

						tidx.barrier.wait();
					}

					// Store the tile result in the global memory.
					if (local_idx == 0)
					{
						av_dst[tidx.tile] = tile_data[0];
					}
				});

				// Update the sequence length, swap source with destination.
				element_count /= _tile_size * 2;
				std::swap(av_src, av_dst);
				av_dst.discard_data();
			}

			// Perform any remaining uneven reduction on the CPU.
			std::vector<float_3> result(element_count);
			copy(av_src.section(0, element_count), result.begin());
			return std::accumulate(result.begin(), result.end(), float_3(0.f));
		}

		template <unsigned _tile_size>
		static float_3 reduction_tiled_4(const std::vector<float_3>& source)
		{
			assert(source.size() <= UINT_MAX);
			unsigned element_count = static_cast<unsigned>(source.size());
			assert(element_count != 0); // Cannot reduce an empty sequence.

			// Using arrays as temporary memory.
			array<float_3, 1> arr_1(element_count, source.begin());
			return reduction_tiled_4<_tile_size>(arr_1, element_count);
		}

		//----------------------------------------------------------------------------
		// Helper function comparing floating point numbers within a given relative
		// difference.
		//----------------------------------------------------------------------------
		static bool fp_equal(float a, float b, float max_rel_diff)
		{
			float diff = std::fabs(a - b);
			a = std::fabs(a);
			b = std::fabs(b);
			return diff <= max(a, b) * max_rel_diff;
		}
	};
}