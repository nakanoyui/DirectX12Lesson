#pragma once

class Component
{
public:
	Component();
	virtual ~Component() = default;

	virtual void Init() {}
	virtual void Update() {}
	virtual void Draw() {}
	virtual void DrawLate() {}

	virtual void ImGuiUpdateInspecter() = 0;
	virtual void PrepareJsonParameter(nlohmann::json&json) = 0;
	virtual void ReferenceJson() = 0;

	const std::string& GetName() { return m_name; }
	void SetObjectName(const std::string_view& objectName) { m_objectName = objectName; }

	void SetEnable(bool bEnable) { m_bEnable = bEnable; }
	bool GetEnable() { return m_bEnable; }

	void SetGameObject(GameObject*pGameObject);
	const GameObject* GetGameObject();

protected:
	std::string m_name;
	std::string m_objectName;
	bool m_bEnable;
	GameObject* m_pGameObject;
};