#pragma once

#include "RendererComponent.h"

class SkinMeshRendererComponent :public RendererComponent
{
public:
	SkinMeshRendererComponent();

	void Draw()override;

	void ImGuiUpdateInspecter()override;

	void PrepareJsonParameter(nlohmann::json& json)override;
private:
	void ReferenceJson()override;
};