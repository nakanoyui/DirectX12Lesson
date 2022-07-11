#pragma once

#include "../EnvironmentObject.h"

class NeonBuilding :public EnvironmentObject
{
public:
	NeonBuilding() = default;
	~NeonBuilding() = default;

	void LoadAsset()override;

	void Init()override;
	void Update()override;
	void DrawStandard()override;

private:
};