// Vicsek_AMPConsole.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "cvmarkersobj.h"
#include "Viscek2DKulinskIntegrator.h"
#include "VicsekStoppedTop.h"

void RunTestCollectionIntegrator(float domainSize, int collSize, int particleSize)
{
	accelerator::set_default(accelerator::direct3d_warp);
	concurrency::diagnostic::marker_series NextIterationMarker(_T("NextIteration"));
	concurrency::diagnostic::marker_series NextNoizeIterationMarker(_T("NextNoizeIterationMarker"));
	concurrency::diagnostic::marker_series NextVelocAppendIterationMarker(_T("NextVelocAppendIterationMarker"));
	float size = domainSize;
	int partCount = 512 * (particleSize / 512);
	float sqrParticleCount = sqrt(partCount);

	std::vector<TaskData*> tasks;
	for (int i = 0; i < collSize; i++)
	{
		tasks.push_back(new TaskData(partCount, accelerator(accelerator::default_accelerator).default_view, accelerator(accelerator::default_accelerator)));
	}

	std::vector<std::shared_ptr<CIntegrator2D>> integrs;
	for (int i = 0; i < tasks.size(); i++)
	{
		integrs.push_back(std::make_shared<CVicsek2DIntegrator>());
	}

	CIntegratorCollection IntegratorCollection(tasks, float_2(size, size), integrs);

	float noise = 360;

	time_t rawtime;
	struct tm timeinfo;
	char buffer[256];
	char buffer2[256];
	std::string bufferComment = "";
	time(&rawtime);
	localtime_s(&timeinfo, &rawtime);
	strftime(buffer, 256, "Velocities_%d.%m_%H.%M.%S.txt", &timeinfo);
	strftime(buffer2, 256, "SplitsVelocities_%d.%m_%H.%M.%S.txt", &timeinfo);

	IntegratorCollection.WriteComment(bufferComment);

	CDataCollection dataCollection;

	std::vector<float> averSpd;
	std::vector<float> averSpdOnSlices;

	for (int i = 0; i < 360; i++)
	{
		NextNoizeIterationMarker.write_flag(1, L"BEFORE noise %2.2f iteration", noise);

		bool iterate = true;
		float prevAverSpd = 0;
		float currAverSpd = 0;
		int iteration = 0;
		while (iterate)
		{
			int numSteps = 50;
			for (int j = 0; j < numSteps; j++)
			{
				iteration++;
				NextIterationMarker.write_flag(1, L"BEFORE iteration N %d", j);
				IntegratorCollection.Integrate(noise);
				NextIterationMarker.write_flag(1, L"AFTER iteratin N %d", j);
				std::cout << j;
			}

			concurrency::diagnostic::marker_series averagingSeries;
			concurrency::diagnostic::span* averagingSpan = new concurrency::diagnostic::span(averagingSeries, 1, L"Averaging span");
			for (int j = 0; j < 20; j++)
			{
				iteration++;
				IntegratorCollection.Integrate(noise);
				averagingSeries.write_flag(1, L"BEFORE veloc count and append N %d", j);
				averSpd.push_back(IntegratorCollection.GetAnsambleAveragedABSVeloc());
				averagingSeries.write_flag(1, L"AFTER veloc count and append N %d", j);
			}
			delete averagingSpan;
			currAverSpd = std::accumulate(averSpd.begin(), averSpd.end(), 0.0f) / averSpd.size();
			if (!(abs(currAverSpd - prevAverSpd) < (1 / sqrParticleCount)))
			{
				prevAverSpd = currAverSpd;
				numSteps *= 2;
				averSpd.clear();
			}
			else
			{
				iterate = false;
			}
		}
		NextNoizeIterationMarker.write_flag(1, L"AFTER noise %2.2f iteration", noise);
		dataCollection.AddAverSpeed(currAverSpd, noise);
		dataCollection.AddAverSpeedOnSlices(IntegratorCollection.GetAnsambleAveragedVeclocOnSlicesX(100), noise);
		NextNoizeIterationMarker.write_flag(1, L"AFTER noise AND SLICES VELOCITY COUNT + APPEND %2.2f iteration", noise);
		averSpd.clear();
		noise -= 1;
		iterate = true;

		std::cout << noise << std::endl;
	}
	dataCollection.WriteOnDisk(buffer, buffer2, bufferComment);
}

void RunCollectionIntegrator(float domainSize, int collSize, int particleSize)
{
	float size = domainSize;
	int partCount = 256 * (particleSize / 256);
	float sqrParticleCount = sqrt(partCount);

	std::vector<TaskData*> tasks;
	for (int i = 0; i < collSize; i++)
	{
		tasks.push_back(new TaskData(partCount, accelerator(accelerator::default_accelerator).default_view, accelerator(accelerator::default_accelerator)));
	}

	std::vector<std::shared_ptr<CIntegrator2D>> integrs;
	for (int i = 0; i < tasks.size(); i++)
	{
		integrs.push_back(std::make_shared<CViscek2DKulinskIntegrator>());
	}

	CIntegratorCollection IntegratorCollection(tasks, float_2(size, size), integrs);

	float noise = 360;

	
	time_t rawtime;
	struct tm timeinfo;
	char buffer[256];
	char buffer2[256];
	char buffer3[256];
	std::string bufferComment = "";
	time(&rawtime);
	localtime_s(&timeinfo, &rawtime);
	strftime(buffer, 256, "Velocities_%d.%m_%H.%M.%S.txt", &timeinfo);
	strftime(buffer2, 256, "SplitsVelocities_%d.%m_%H.%M.%S.txt", &timeinfo);
	strftime(buffer3, 256, "SplitsDensities_%d.%m_%H.%M.%S.txt", &timeinfo);

	IntegratorCollection.WriteComment(bufferComment);

	CDataCollection dataCollection;

	std::vector<float> averSpd;
	std::vector<float> averSpdOnSlices;

	std::vector<float> averDispers;
	std::vector<int> numStepsInIter = { 0 };
	
	while (noise > 0)
	{
		bool iterate = true;
		float prevAverSpd = 0;
		float currAverSpd = 0;

		float prevDisper = 0;
		float currDisper = 0;

		int iteration = 0;
		
		int numSteps = 100;
		while (iterate)
		{
			for (int j = 0; j < numSteps; j++)
			{
				iteration++;
				IntegratorCollection.Integrate(noise);
			}
			for (int j = 0; j < 20; j++)
			{
				iteration++;
				IntegratorCollection.Integrate(noise);
				averSpd.push_back(IntegratorCollection.GetAnsambleAveragedABSVeloc());
				IntegratorCollection.Integrate(noise);
				auto velocOnSlices = IntegratorCollection.GetAnsambleAveragedVeclocOnSlicesX(15);
				IntegratorCollection.Integrate(noise);
				averDispers.push_back(MathHelpers::Dispercion(velocOnSlices));
			}

			currAverSpd = std::accumulate(averSpd.begin(), averSpd.end(), 0.0f) / averSpd.size();
			averSpd.clear();

			currDisper = std::accumulate(averDispers.begin(), averDispers.end(), 0.0f) / averDispers.size();
			averDispers.clear();
			std::cout << "Dispercion = " << abs(currDisper - prevDisper) - (1 / sqrParticleCount) << " Steps = " << numSteps << std::endl;
			//std::cout << "Velocity = " << abs(currAverSpd - prevAverSpd) - (1 / sqrParticleCount) << std::endl;

			if ((abs(currDisper - prevDisper) > (1 / sqrParticleCount)) && numSteps < 25000)
			{
				prevDisper = currDisper;
				prevAverSpd = currAverSpd;
				numSteps *= 2;
				numStepsInIter[numStepsInIter.size() - 1] += numSteps;
			}
			else
			{
				iterate = false;
				numStepsInIter.push_back(0);
			}
		}
		dataCollection.AddAverSpeed(currAverSpd, noise);
		IntegratorCollection.Integrate(noise);
		dataCollection.AddAverSpeedOnSlices(IntegratorCollection.GetAnsambleAveragedVeclocOnSlicesX(15), noise);
		IntegratorCollection.Integrate(noise);
		dataCollection.AddAverRhoOnSlices(IntegratorCollection.GetAnsambleAveragedDencityOnSlicesX(15), noise);

		noise -= 1;
		prevDisper = 0;
		iterate = true;

		std::cout << noise << std::endl;
	}
	dataCollection.WriteOnDisk(buffer, buffer2, buffer3, IntegratorCollection.WriteComment(bufferComment));
	std::cout << "Computation finished." << std::endl;
	std::wcout << L"AverageSteps: " << std::accumulate(numStepsInIter.begin(), numStepsInIter.end(), 0.0f) / numStepsInIter.size();
}


void RunIntegrator(int size)
{
	TaskData td(size*size, accelerator(accelerator::default_accelerator).default_view, accelerator(accelerator::default_accelerator));

	CVicsek2DIntegrator Integrator(td, float_2(size, size));

	float s_Noise = 180;

	time_t timer;
	time_t timer2;

	/* get current time; same as: timer = time(NULL)  */

	std::fstream file;
	file.open("Velocities.txt", std::ios::app);
	std::vector<float> averSpd(20);

	for (int i = 0; i < 180; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			Integrator.Integrate(s_Noise);
			std::cout << i << " " << j << std::endl;
			std::cout << "Veloc: " << MathHelpers::SqrLength(Integrator.GetAverageVeloc()) << " noize: " << s_Noise << std::endl;
		}
		for (int j = 0; j < 20; j++)
		{
			Integrator.Integrate(s_Noise);
			averSpd.push_back(MathHelpers::SqrLength(Integrator.GetAverageVeloc()));
			std::cout << i << " " << j << std::endl;
			std::cout << "Veloc: " << MathHelpers::SqrLength(Integrator.GetAverageVeloc()) << " noize: " << s_Noise << std::endl;
		}

		auto averS = std::accumulate(averSpd.begin(), averSpd.end(), 0.0f);
		averS /= averSpd.size();

		file << "Veloc: " << averS << " noize: " << s_Noise << std::endl;
		averSpd.clear();
		s_Noise -= 0.5;
	}

	for (int i = 0; i < 180; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			Integrator.Integrate(s_Noise);
			std::cout << i << " " << j << std::endl;
			std::cout << "Veloc: " << MathHelpers::SqrLength(Integrator.GetAverageVeloc()) << " noize: " << s_Noise << std::endl;
		}
		for (int j = 0; j < 20; j++)
		{
			Integrator.Integrate(s_Noise);
			averSpd.push_back(MathHelpers::SqrLength(Integrator.GetAverageVeloc()));
			std::cout << i << " " << j << std::endl;
			std::cout << "Veloc: " << MathHelpers::SqrLength(Integrator.GetAverageVeloc()) << " noize: " << s_Noise << std::endl;
		}
		file << std::endl;
		file << std::endl;
		auto averS = std::accumulate(averSpd.begin(), averSpd.end(), 0.0f);
		averS /= averSpd.size();

		file << "Veloc: " << averS << " noize: " << s_Noise << std::endl;

		s_Noise -= 0.5;
		averSpd.clear();
	}
	file.close();
}

int StepsToEq(int size)
{
	int a = 0;
	float aver = 0;
	float maxAver = 0;

	std::vector<float> incrAverStor(22);
	float incAver = 0;

	TaskData td(size*size, accelerator(accelerator::default_accelerator).default_view, accelerator(accelerator::default_accelerator));
	CVicsek2DIntegrator Integrator(td, float_2(size, size));

	float noise = 20;
	std::cout << "Start";
	for (int i = 0; i < 50; i++)
	{
		Integrator.Integrate(noise);
		aver = MathHelpers::Length(Integrator.GetAverageVeloc());
		if (aver > maxAver)
		{
			maxAver = aver;
		}

		incrAverStor.push_back(aver);
		if (incrAverStor.size() > 19)
		{
			incAver = std::accumulate(incrAverStor.begin(), incrAverStor.end(), 0.0f) / 20;
			incrAverStor.clear();
		}
		char c[512];

		sprintf_s(c, "%4d %10.5f %10.5f %10.5f", a, maxAver, aver, incAver);
		std::cout << c << std::endl;
		a++;
	}

	noise = 10;
	for (int i = 0; i < 100; i++)
	{
		Integrator.Integrate(noise);
		aver = MathHelpers::Length(Integrator.GetAverageVeloc());
		if (aver > maxAver)
		{
			maxAver = aver;
		}

		incrAverStor.push_back(aver);
		if (incrAverStor.size() > 19)
		{
			incAver = std::accumulate(incrAverStor.begin(), incrAverStor.end(), 0.0f) / 20;
			incrAverStor.clear();
		}
		char c[512];

		sprintf_s(c, "%4d %10.5f %10.5f %10.5f", a, maxAver, aver, incAver);
		std::cout << c << std::endl;
		a++;
	}

	noise = 0.5;
	maxAver = 0;

	while (true)
	{
		Integrator.Integrate(noise);
		aver = MathHelpers::Length(Integrator.GetAverageVeloc());
		if (aver > maxAver)
		{
			maxAver = aver;
		}

		incrAverStor.push_back(aver);
		if (incrAverStor.size() > 19)
		{
			incAver = std::accumulate(incrAverStor.begin(), incrAverStor.end(), 0.0f) / 20;
			incrAverStor.clear();
		}
		char c[512];

		sprintf_s(c, "%4d %10.5f %10.5f %10.5f", a, maxAver, aver, incAver);
		std::cout << c << std::endl;
		a++;
	}
	return a;
}

int TestReductions()
{
	accelerator default_device;
	std::wcout << "Using device : " << default_device.get_description() << std::endl;
	if (default_device == accelerator(accelerator::direct3d_ref))
		std::cout << "WARNING!! Running on very slow emulator! Only use this accelerator for debugging." << std::endl;

	// Make sure that elements can be split into tiles so the number of
	// tiles in any dimension is less than 65536. Here we we have
	// element_count == 16777216 so the number of tiles:
	// tile_count = element_count / tile_size == 32768 < 65536
	unsigned element_count = 16 * 1024 * 1024;

		std::vector<float_3> source(element_count);
	for (unsigned i = 0; i < element_count; ++i)
	{
		// Element range is limited to avoid overflow or underflow
		source[i] = float_3((i & 0xf) * 0.01f);
	}

	// The data is generated in a pattern and its sum can be computed by the following formula
	const float expected_result = ((element_count / 16) * ((15 * 16) / 2)) * 0.01f;

	std::cout << "Running kernels..." << std::endl;

	const unsigned tile_size = 512;

	typedef float_3(*ReductionFunction)(array<float_3, 1>&, size_t);

	typedef std::pair<ReductionFunction, std::string> user_pair;

	std::vector<user_pair> functions;
	functions.push_back(user_pair(MathHelpers::CReduction::reduction_simple_1, "reduction_simple_1"));
	functions.push_back(user_pair(MathHelpers::CReduction::reduction_simple_2, "reduction_simple_2"));
	functions.push_back(user_pair(MathHelpers::CReduction::reduction_tiled_1<tile_size>, "reduction_tiled_1"));
	functions.push_back(user_pair(MathHelpers::CReduction::reduction_tiled_2<tile_size>, "reduction_tiled_2"));
	functions.push_back(user_pair(MathHelpers::CReduction::reduction_tiled_3<tile_size>, "reduction_tiled_3"));
	functions.push_back(user_pair(MathHelpers::CReduction::reduction_tiled_4<tile_size>, "reduction_tiled_4"));
	
	array<float_3, 1> arr_1(element_count, source.begin());

	for (const auto& func : functions)
	{
		concurrency::diagnostic::marker_series series;
		concurrency::diagnostic::span *flagSpan = new concurrency::diagnostic::span(series, 1, _T("Function span"));
		series.write_flag(_T("Before function."));
		float_3 result = func.first(arr_1, source.size());
		series.write_flag(L"After function");
		delete flagSpan;
		
		//std::cout << "SUCCESS: " << func.second << "." << "  " << result.x << std::endl;
	}
	return 1;
}

int _tmain(int argc, _TCHAR* argv[])
{
	std::wcout << accelerator(accelerator::default_accelerator).description << std::endl;

	RunCollectionIntegrator(32, 10, 4096);
	return 0;
}