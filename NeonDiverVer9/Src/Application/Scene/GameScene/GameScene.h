#pragma once

#include "System/Scene/Scene.h"
#include "System/GameObject/GameObjectFactory.h"

class GameScene :public Scene
{
public:
	GameScene(std::string name) { m_name = name; }
	~GameScene() {}

	void LoadContents()override;
	void Init()override;
	void Update()override;
	void Draw()override;

	void ImGuiUpdate()override;
private:
	void LoadParameter();
	void ReferenceJson();
	void CheckDeleteObject();

	std::unique_ptr<GameObjectFactory> m_upGameObjectFactory = nullptr;

	std::unique_ptr<CSVManager>		m_upCSVManager = nullptr;

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
};