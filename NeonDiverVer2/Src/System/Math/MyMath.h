#pragma once

namespace Constant
{
	constexpr int Zero_I = 0;
	constexpr float Zero_F = 0.0f;
	constexpr int One_I = 1;
	constexpr float One_F = 1.0f;

	constexpr DirectX::SimpleMath::Vector2 ScreenSize = { 1280,720 };
}

namespace MyMath
{
	float lerp(const float& a,const float& b,const float& t);
}