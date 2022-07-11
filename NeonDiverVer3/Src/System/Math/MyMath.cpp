#include "MyMath.h"

float MyMath::lerp(const float& a, const float& b, const float& t)
{
	return a + t * (b - a);
}