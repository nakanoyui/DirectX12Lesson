#pragma once

#include "RendererComponent.h"

class SpriteRendererComponent :public RendererComponent
{
public:
	SpriteRendererComponent();

	void Draw()override;

	void ImGuiUpdateInspecter()override;

	void PrepareJsonParameter(nlohmann::json& json)override;
private:
	void ReferenceJson()override;
};