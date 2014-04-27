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
}