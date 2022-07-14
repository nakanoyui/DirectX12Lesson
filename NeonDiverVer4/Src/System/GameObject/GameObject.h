#pragma once

#include "System/Math/MyMath.h"

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
	virtual void DrawEffect() {}

	virtual void ImGuiUpdate() {}

	virtual void SetPos(const Math::Vector3& pos) { m_transform.Positon(pos); }
	virtual Math::Vector3 GetPos() const { return m_transform.Positon(); }

	const Math::Matrix& GetMatrix() const { return m_transform.Matrix(); }

	const std::unique_ptr<Collider>& GetCollider()const { return m_upCollider; }

	virtual bool IsDelete() { return m_bDelete; }

protected:
	MyMath::Transform m_transform;
	std::unique_ptr<Collider> m_upCollider = nullptr;
	bool			m_bDelete = false;
};