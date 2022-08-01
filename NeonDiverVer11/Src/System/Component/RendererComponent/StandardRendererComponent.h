#pragma once

#include "RendererComponent.h"

class StandardRendererComponent :public RendererComponent
{
public:
	StandardRendererComponent();

	void Draw()override;

	void ImGuiUpdateInspecter()override;

	void PrepareJsonParameter(nlohmann::json& json)override;
private:
	void ReferenceJson()override;
};