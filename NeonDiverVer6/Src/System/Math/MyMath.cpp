#include "MyMath.h"

float MyMath::lerp(const float& a, const float& b, const float& t)
{
	return a + t * (b - a);
}

const Math::Vector3& MyMath::Slerp(const Math::Vector3& start, const Math::Vector3& end, float t)
{
	Math::Vector3 nStart = start;
	Math::Vector3 nEnd = end;
	nStart.Normalize();
	nEnd.Normalize();

	Math::Vector3 dot = DirectX::XMVector3Dot(nStart, nEnd);
	float theta = acos(dot.x);

	float sinTheta = sin(theta);
	float sinThetaFrom = sin((1 - t) * theta);
	float sinThetaTo = sin(t * theta);

	float magnitudeLerp = lerp(nStart.Length(), end.Length(), t);

	Math::Vector3 slerpVector = (sinThetaFrom * nStart + sinThetaTo * nEnd) / sinTheta;

	return magnitudeLerp * slerpVector;
}

const Math::Vector3& MyMath::RouteCurve(const Math::Vector3& start, const Math::Vector3& end, const Math::Vector3& control, float t)
{
	Math::Vector3 Q0 = Math::Vector3::Lerp(start,control,t);
	Math::Vector3 Q1 = Math::Vector3::Lerp(control,end,t);
	Math::Vector3 Q2 = Math::Vector3::Lerp(Q0,Q1,t);

	return Q2;
}
