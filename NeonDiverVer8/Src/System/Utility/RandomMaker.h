#pragma once

#include <random>

class RandomMaker
{
private:
	static std::mt19937_64 rnd;
public:
	static int GetInt(int minValue, int maxValue);
	static float GetFloat(float minValue, float maxValue);
};