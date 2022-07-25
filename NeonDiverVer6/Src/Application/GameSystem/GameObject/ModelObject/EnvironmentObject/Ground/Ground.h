#pragma once

#include "../EnvironmentObject.h"

class Ground :public EnvironmentObject
{
public:
	Ground() = default;
	~Ground() = default;

	void LoadAsset()override;

	void Init()override;
	void Update()override;
	void DrawStandard()override;

private:
	void ReferenceJson()override;
};