#pragma once

#include <amp.h>
#include <amp_math.h>
#include <amp_graphics.h>
#include "MathHelper.h"

using namespace concurrency;
using namespace concurrency::graphics;

namespace Vicsek2DMath
{
	void BodyBodyInteraction(float_2& vel, const float_2 otherParticleVel,
							 const float_2 particlePosition, const float_2 otherParticlePosition,
							 float softeningSquared, float intR2) restrict(amp);

	void BorderCheckTransitional(float_2& pos, float_2& vel, const float_2 domainSize) restrict(amp);
	void BorderCheckMoovingTopY(float_2& pos, float_2& vel, const float_2 domainSize) restrict(amp);
	void BorderCheckStoppedTopY(float_2& pos, float_2& vel, const float_2 domainSize, float rndTop, float rndBottom) restrict(amp);
	void BorderCheckMoveTopMoveBottom(float_2& pos, float_2& vel, const float_2 domainSize, float rndTop, float rndBottom) restrict(amp);
}