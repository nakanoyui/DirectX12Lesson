#include "MyMath.h"

float MyMath::lerp(const float& a, const float& b, const float& t)
{
	return a + t * (b - a);
}

Math::Vector3 MyMath::Slerp(const Math::Vector3& start, const Math::Vector3& end, float t)
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

Math::Vector3 MyMath::RouteCurve(const Math::Vector3& start, const Math::Vector3& end, const Math::Vector3& control, float t)
{
	Math::Vector3 Q0 = Math::Vector3::Lerp(start,control,t);
	Math::Vector3 Q1 = Math::Vector3::Lerp(control,end,t);
	Math::Vector3 Q2 = Math::Vector3::Lerp(Q0,Q1,t);

	return Q2;
}

Math::Vector3 MyMath::ToEuler(const Math::Quaternion& q)
{
	auto sy = -(2 * q.x * q.z - 2 * q.y * q.w);
	auto unlocked = std::abs(sy) < 0.99999f;

	return Math::Vector3(
		unlocked ? std::atan2(2 * q.y * q.z + 2 * q.x * q.w, 2 * q.w * q.w + 2 * q.z * q.z - 1) : 0,
		std::asin(sy),
		unlocked ? std::atan2(2 * q.x * q.y + 2 * q.z * q.w, 2 * q.w * q.w + 2 * q.x * q.x - 1)
		: std::atan2(-(2 * q.x * q.y - 2 * q.z * q.w), 2 * q.w * q.w + 2 * q.y * q.y - 1)
	);
}

Math::Quaternion MyMath::ToQuaternion(const Math::Vector3& e)
{
	auto cx = std::cos(0.5f * e.x);
	auto sx = std::sin(0.5f * e.x);
	auto cy = std::cos(0.5f * e.y);
	auto sy = std::sin(0.5f * e.y);
	auto cz = std::cos(0.5f * e.z);
	auto sz = std::sin(0.5f * e.z);

		return Math::Quaternion(
			sx * cy * cz - cx * sy * sz,
			sx * cy * sz + cx * sy * cz,
			-sx * sy * cz + cx * cy * sz,
			sx * sy * sz + cx * cy * cz
		);
}
