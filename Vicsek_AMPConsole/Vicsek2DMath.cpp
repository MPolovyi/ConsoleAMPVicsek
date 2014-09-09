#include "Vicsek2DMath.h"

namespace Vicsek2DMath
{
	void BodyBodyInteraction(float_2& vel, const float_2 otherParticleVel,
							 const float_2 particlePosition, const float_2 otherParticlePosition,
							 float softeningSquared, float intR2) restrict(amp)
	{
		float_2 r = otherParticlePosition - particlePosition;
		float distSqr = r.x*r.x + r.y*r.y + softeningSquared;

		if (distSqr > intR2)
		{
			return;
		}

		vel += otherParticleVel;
	}

	void BorderCheckTransitional(float_2& pos, float_2& vel, const float_2 domainSize) restrict(amp)
	{
		if (concurrency::direct3d::step(domainSize.x, pos.x))
		{
			pos.x -= domainSize.x;
		}
		if (concurrency::direct3d::step(pos.x, 0))
		{
			pos.x += domainSize.x;
		}
		if (concurrency::direct3d::step(domainSize.y, pos.y))
		{
			pos.y -= domainSize.y;
		}
		if (concurrency::direct3d::step(pos.y, 0))
		{
			pos.y += domainSize.y;
		}
	}

	void BorderCheckMoovingTopY(float_2& pos, float_2& vel, const float_2 domainSize) restrict(amp)
	{
		//domainSize.x < pos.x
		if (concurrency::direct3d::step(domainSize.x, pos.x))
		{
			pos.x -= domainSize.x;
		}
		//pos.x < 0
		if (concurrency::direct3d::step(pos.x, 0))
		{
			pos.x += domainSize.x;
		}

		//check. possible interract simulatenously with particle-particle interraction, and not on touch but being in interraction radius
		//domainSize.y < pos.y
		if (concurrency::direct3d::step(domainSize.y-1, pos.y))
		{
			float dist = pos.y - domainSize.y;
			pos.y -= 2*dist;
			vel += float_2(1, 0);
			MathHelpers::NormalizeVector(vel);
		}
		//pos.y < 0
		if (concurrency::direct3d::step(pos.y, 0))
		{
			pos.y = -pos.y;
			vel.y = -vel.y;
		}
	}

	void BorderCheckStoppedBottomMovingTopY(float_2& pos, float_2& vel, const float_2 domainSize, float rndTop, float rndBottom) restrict(amp)
	{
		//domainSize.x < pos.x
		if (concurrency::direct3d::step(domainSize.x, pos.x))
		{
			pos.x -= domainSize.x;
		}
		//pos.x < 0
		if (concurrency::direct3d::step(pos.x, 0))
		{
			pos.x += domainSize.x;
		}

		//check. possible interract simulatenously with particle-particle interraction, and not on touch but being in interraction radius
		//domainSize.y < pos.y
		if (concurrency::direct3d::step(domainSize.y - 1, pos.y))
		{
			float dist = pos.y - domainSize.y;
			pos.y -= 2 * dist;
			vel += float_2(1, 0);
			MathHelpers::NormalizeVector(vel);
		}
		//pos.y < 0
		if (concurrency::direct3d::step(pos.y, 1))
		{
			pos.y = -pos.y;
			vel.y = -vel.y;
			vel.x *= rndBottom;
			MathHelpers::NormalizeVector(vel);
		}
	}

	void BorderCheckMoveTopMoveBottom(float_2& pos, float_2& vel, const float_2 domainSize, float rndTop, float rndBottom) restrict(amp)
	{
		//domainSize.x < pos.x
		if (concurrency::direct3d::step(domainSize.x, pos.x))
		{
			pos.x -= domainSize.x;
		}
		//pos.x < 0
		if (concurrency::direct3d::step(pos.x, 0))
		{
			pos.x += domainSize.x;
		}

		//check. possible interract simulatenously with particle-particle interraction, and not on touch but being in interraction radius
		//domainSize.y < pos.y
		if (concurrency::direct3d::step(domainSize.y - 1, pos.y))
		{
			float dist = pos.y - domainSize.y;
			pos.y -= 2 * dist;
			vel += float_2(1, 0);
			MathHelpers::NormalizeVector(vel);
		}
		//pos.y < 1
		if (concurrency::direct3d::step(pos.y, 1))
		{
			pos.y = -pos.y;
			vel += float_2(-1, 0);
			MathHelpers::NormalizeVector(vel);
		}
	}
}