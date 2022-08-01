#pragma once

#include "../Manager/ImGuiManager/ImGuiHelper.h"

class Component
{
public:
	Component();
	virtual ~Component() = default;

	virtual void Init() {}
	virtual void Start() {}
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

	void SetDelete(bool bDelete) { m_bDelete = bDelete; }
	bool GetDelete() { return m_bDelete; }

	void SetGameObject(std::shared_ptr<GameObject>wpGameObject);
	const std::shared_ptr<GameObject> GetGameObject();

protected:
	std::string m_name;
	std::string m_objectName;
	bool m_bEnable;
	bool m_bDelete;
	std::weak_ptr<GameObject> m_wpGameObject;
};