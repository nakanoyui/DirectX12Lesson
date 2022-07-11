#pragma once

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

	virtual void ImGuiUpdate() {}

	virtual void SetPos(const Math::Vector3& pos) { m_mWorld.Translation(pos); }
	virtual Math::Vector3 GetPos() const { return m_mWorld.Translation(); }

	const Math::Matrix& GetMatrix() const { return m_mWorld; }

	virtual bool IsDelete() { return m_bDelete; }

protected:
	Math::Matrix	m_mWorld;
	bool			m_bDelete = false;
};