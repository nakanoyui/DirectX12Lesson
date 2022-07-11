#pragma once

#include "../../CharacterObject.h"

class Robot :public CharacterObject
{
public:
	Robot() = default;
	~Robot() = default;

	void LoadAsset()override;

	void Init()override;
	void Update()override;
	void DrawStandard()override;
	void DrawEffect()override;

	//void UpdateRotate()override;
	//void UpdateMatrix()override;

	//void ImGuiUpdate()override;
private:
};