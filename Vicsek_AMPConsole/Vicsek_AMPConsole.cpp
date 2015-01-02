// Vicsek_AMPConsole.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "cvmarkersobj.h"
#include "Viscek2DKulinskIntegrator.h"
#include "VicsekStoppedTop.h"
void RunTestCollectionIntegrator(float domainSize, int collSize, int particleSize);
void RunCollectionIntegrator(float domainSize, int collSize, int particleSize);
void RunIntegrator(int size);
int StepsToEq(int size);
int TestReductions();

void RunManyIntegrators(float domainSize, int collSize, int particleSize);
void RunCollectionIntegratorOneNoise(float domainSize, int collSize, int particleSize, float noise);

int _tmain(int argc, _TCHAR* argv[])
{
	accelerator::set_default(accelerator::direct3d_warp);
	std::wcout << accelerator(accelerator::default_accelerator).description << std::endl;

	RunManyIntegrators(100, 100, 2048);
	char a;
	std::cin >> a;
	return 0;
}

void RunManyIntegrators(float domainSize, int collSize, int particleSize)
{
	for (size_t i = 0; i < 100; i++)
	{
		RunCollectionIntegratorOneNoise(domainSize, 1, particleSize, 156);
		std::cout << std::endl << "INTEGRATING "<<i<<" CONFIGURATION FINISHED, START NEXT" << std::endl;
	}
}

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
		integrs.push_back(std::make_shared<CViscek2DKulinskIntegrator>());
	}

	CIntegratorCollection IntegratorCollection(tasks, float_2(size, size), integrs);

	float noise = 360;

	time_t rawtime;
	struct tm timeinfo;
	char buffer[256];
	char buffer2[256];
	char buffer3[256];
	char buffer4[256];
	std::string bufferComment = "";
	time(&rawtime);
	localtime_s(&timeinfo, &rawtime);
	strftime(buffer, 256, "Velocities_%d.%m_%H.%M.%S.txt", &timeinfo);
	strftime(buffer2, 256, "SplitsVelocities_%d.%m_%H.%M.%S.txt", &timeinfo);
	strftime(buffer3, 256, "SplitsDensities_%d.%m_%H.%M.%S.txt", &timeinfo);
	strftime(buffer4, 256, "ParticleVelocities_%d.%m_%H.%M.%S.txt", &timeinfo);

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
			std::cout << "Velocity = " << abs(currAverSpd - prevAverSpd) - (1 / sqrParticleCount) << std::endl;

			if ((abs(currDisper - prevDisper) > (1 / sqrParticleCount)) && numStepsInIter[numStepsInIter.size() - 1] < 5000)
			{
				prevDisper = currDisper;
				prevAverSpd = currAverSpd;
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

void RunCollectionIntegratorOneNoise(float domainSize, int collSize, int particleSize, float noise = 180)
{
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
		integrs.push_back(std::make_shared<CVicsekStoppedTop>());
	}

	CIntegratorCollection IntegratorCollection(tasks, float_2(size, size), integrs);

	time_t rawtime;
	struct tm timeinfo;
	char buffer[256];
	char buffer2[256];
	char buffer3[256];
	char buffer4[256];
	std::string bufferComment = "";
	time(&rawtime);
	localtime_s(&timeinfo, &rawtime);
	strftime(buffer, 256, "Velocities_%d.%m_%H.%M.%S.txt", &timeinfo);
	strftime(buffer2, 256, "SplitsVelocities_%d.%m_%H.%M.%S.txt", &timeinfo);
	strftime(buffer3, 256, "SplitsDensities_%d.%m_%H.%M.%S.txt", &timeinfo);
	strftime(buffer4, 256, "ParticleData_%d.%m_%H.%M.%S.txt", &timeinfo);


	IntegratorCollection.WriteComment(bufferComment);

	CDataCollection dataCollection;

	std::vector<float> averSpd;
	std::vector<float> averSpdOnSlices;

	std::vector<float> averDispers;
	std::vector<int> numStepsInIter = { 0 };
	auto tmpNoise = noise - 10;
	while (noise > tmpNoise)
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
			std::cout << "Velocity = " << abs(currAverSpd - prevAverSpd) - (1 / sqrParticleCount) << std::endl;

			if ((abs(currDisper - prevDisper) > (1 / sqrParticleCount)) && numStepsInIter[numStepsInIter.size() - 1] < 5000)
			{
				prevDisper = currDisper;
				prevAverSpd = currAverSpd;
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
		IntegratorCollection.Integrate(noise);
		IntegratorCollection.WriteParticleDataOnDisc(buffer4);
		IntegratorCollection.Integrate(noise);

		noise -= 1;
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
