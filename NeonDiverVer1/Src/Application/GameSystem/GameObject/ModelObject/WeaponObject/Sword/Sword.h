#pragma once

#include "../WeaponObject.h"

class Sword :public WeaponObject
{
public:
	Sword() = default;
	~Sword() = default;

	void LoadAsset()override;

	void Init()override;
	void Update()override;
	void DrawStandard()override;

private:
};