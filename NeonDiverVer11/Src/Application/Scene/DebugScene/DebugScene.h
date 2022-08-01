#pragma once

#include "System/Scene/Scene.h"
#include "System/Manager/ImGuiManager/ImGuiHelper.h"

class DebugScene :public Scene
{
public:
	DebugScene() {}
	~DebugScene() {}

	void LoadContents()override;
	void Init()override;
	void Update()override;
	void Draw()override;

	void ImGuiUpdate()override;

	void UpdateLast()override;
private:

	std::weak_ptr<GPUParticleManager> m_wpParticleManager;

	// FXAA
	RenderTexture m_fxaa;

	RenderTexture m_screen;
	Math::Matrix m_mScreen;

	char m_names[ImGuiHelper::InputTextSize] = "";
	char m_tags[ImGuiHelper::InputTextSize] = "";

	bool m_bExists;
};