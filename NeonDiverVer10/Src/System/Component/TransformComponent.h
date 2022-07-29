#pragma once

#include "Component.h"

class TransformComponent :public Component
{
public:
	TransformComponent();
	~TransformComponent() {}

	void Init()override;
	void Update() override;

	void ImGuiUpdateInspecter()override;
	void PrepareJsonParameter(nlohmann::json& json)override;

	const Math::Matrix& GetMatrix() { return m_transform.Matrix(); }

	const Math::Vector3 GetPos() { return m_transform.Positon(); }
	void SetPos(const Math::Vector3& pos) { m_transform.Positon(pos); }

private:
	void ReferenceJson()override;

	Transform m_transform;
	Math::Vector3 m_euler;
};