// Vicsek_AMPConsole.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

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
		integrs.push_back(std::make_shared<CVicsek2DIntegrator>());
	}

	CIntegratorCollection IntegratorCollection(tasks, float_2(size, size), integrs);

	float noise = 360;

	time_t rawtime;
	struct tm timeinfo;
	char buffer[256];
	char buffer2[256];
	char bufferComment[256];
	time(&rawtime);
	localtime_s(&timeinfo, &rawtime);
	strftime(buffer, 256, "Velocities_%d.%m_%H.%M.%S.txt", &timeinfo);
	strftime(buffer2, 256, "SplitsVelocities_%d.%m_%H.%M.%S.txt", &timeinfo);

	sprintf_s(bufferComment, "Particle count = %d Domain size = %5.2f", partCount, size);

	
	CDataCollection dataCollection;

	std::vector<float> averSpd;
	std::vector<float> averSpdOnSlices;

	for (int i = 0; i < 360; i++)
	{
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
				IntegratorCollection.Integrate(noise);
				std::cout << "noise = " << noise << " iteration = " << iteration << std::endl;
			}
			for (int j = 0; j < 20; j++)
			{
				iteration++;
				IntegratorCollection.Integrate(noise);
				averSpd.push_back(IntegratorCollection.GetAnsambleAveragedABSVeloc());

				std::cout << "noise = " << noise << " iteration = " << iteration << std::endl;
			}
			currAverSpd = std::accumulate(averSpd.begin(), averSpd.end(), 0.0f) / averSpd.size();
			if (!(abs(currAverSpd - prevAverSpd) < 1 / sqrParticleCount))
			{
				prevAverSpd = currAverSpd;
				numSteps *= 2;
			}
			else
			{
				iterate = false;
			}
		}
		dataCollection.AddAverSpeed(currAverSpd, noise);
		dataCollection.AddAverSpeedOnSlices(IntegratorCollection.GetAnsambleAveragedVeclocOnSplitsX(100), noise);
			
		averSpd.clear();
		noise -= 1;
		iterate = true;
	}
	dataCollection.WriteOnDisk(buffer, buffer2, bufferComment);
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
	for (int  i = 0; i < 100; i++)
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

int _tmain(int argc, _TCHAR* argv[])
{
	accelerator::set_default(accelerator::direct3d_warp);
	std::wcout << accelerator(accelerator::default_accelerator).description << std::endl;
	
	RunCollectionIntegrator(150, 5, 9984);

	return 0;
}



