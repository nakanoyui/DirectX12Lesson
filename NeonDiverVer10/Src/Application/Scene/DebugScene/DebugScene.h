#pragma once

#include "System/Scene/Scene.h"

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
private:
	void LoadParameter();
	void ReferenceJson();
	void CheckDeleteObject();

	std::weak_ptr<GPUParticleManager> m_wpParticleManager;

	bool m_bEnableDebugWireFrame;

	// FXAA
	RenderTexture m_fxaa;
	bool m_bEnableFXAA;

	enum class StageType
	{
		None,
		Road,
		NeonBuildNotRotate,
		NeonBuildRotate,
	};

	RenderTexture m_screen;
	Math::Matrix m_mScreen;
};