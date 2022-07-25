#pragma once

#include "System/Math/MyMath.h"

class Json;
class JsonManager;
class Collider;

class GameObject
{
public:
	GameObject() = default;
	virtual ~GameObject() = default;

	virtual void Init() {}
	virtual void Update() {}
	virtual void DrawSprite() {}
	virtual void DrawStandard() {}
	virtual void DrawSkinMesh() {}
	virtual void DrawPlayer() {}
	virtual void DrawEffect() {}

	virtual void ImGuiUpdate() {}

	virtual void SetPos(const Math::Vector3& pos) { m_transform.Positon(pos); }
	virtual const Math::Vector3& GetPos() const { return m_transform.Positon(); }

	const Math::Matrix& GetMatrix() const { return m_transform.Matrix(); }

	const std::unique_ptr<Collider>& GetCollider()const { return m_upCollider; }

	virtual bool IsDelete() { return m_bDelete; }

	virtual const std::weak_ptr<Json>& GetJson() { return m_wpJson; }

	virtual void ReferenceJson() {}

protected:
	MyMath::Transform m_transform;
	std::unique_ptr<Collider> m_upCollider = nullptr;
	bool			m_bDelete = false;

	std::weak_ptr<JsonManager> m_wpJsonManager;
	std::weak_ptr<Json>m_wpJson;
};