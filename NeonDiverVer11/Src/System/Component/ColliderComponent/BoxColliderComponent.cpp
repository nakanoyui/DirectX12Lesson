#include "BoxColliderComponent.h"

BoxColliderComponent::BoxColliderComponent()
{
	m_name = "BoxColliderComponent";
}

void BoxColliderComponent::Init()
{
	ColliderComponent::Init();

	ReferenceJson();
}

void BoxColliderComponent::Start()
{
	ColliderComponent::Start();

	CreateCollision();
}

void BoxColliderComponent::Update()
{
	CreateCollision();

	if (m_upDebugWireFrame)
	{
		m_wpDrawManager.lock()->GetDrawList("EffectShader")->AddDrawList(m_wpGameObject.lock());
	}
}

void BoxColliderComponent::ImGuiUpdateInspecter()
{
	if (ImGui::TreeNode(m_name.c_str()))
	{
		m_bb.Center = ImGuiHelper::InputFloat3("Center", Math::Vector3(m_bb.Center));

		m_bb.Extents = ImGuiHelper::InputFloat3("Size", Math::Vector3(m_bb.Extents));

		ImGuiHelper::ColorEdit3("Color", m_color);

		if (ImGui::Button("Reset"))
		{
			ReferenceJson();
		}
		ImGui::Separator();

		ImGui::TreePop();
	}
}

void BoxColliderComponent::PrepareJsonParameter(nlohmann::json& json)
{
	auto array = nlohmann::json::array();

	array.clear();
	array.push_back(m_bb.Center.x);
	array.push_back(m_bb.Center.y);
	array.push_back(m_bb.Center.z);
	json["Center"] = array;
	array.clear();
	array.push_back(m_bb.Extents.x);
	array.push_back(m_bb.Extents.y);
	array.push_back(m_bb.Extents.z);
	json["Size"] = array;
	array.clear();
	array.push_back(m_color.x);
	array.push_back(m_color.y);
	array.push_back(m_color.z);
	json["Color"] = array;
	array.clear();
}

void BoxColliderComponent::ReferenceJson()
{
	m_bb.Center = ImGuiHelper::CheckDataVector3(m_wpGameObject.lock()->GetJsonManager().lock()->GetJson(m_objectName)->WorkConstJson(), Math::Vector3(m_bb.Center), "Center", Math::Vector3(Constant::Zero_F));

	m_bb.Extents = ImGuiHelper::CheckDataVector3(m_wpGameObject.lock()->GetJsonManager().lock()->GetJson(m_objectName)->WorkConstJson(), Math::Vector3(m_bb.Extents), "Size", Math::Vector3(Constant::One_F));

	ImGuiHelper::CheckDataVector3(m_wpGameObject.lock()->GetJsonManager().lock()->GetJson(m_objectName)->WorkConstJson(), m_color, "Color", Math::Vector3(Constant::One_F));
}

void BoxColliderComponent::CreateCollision()
{
	m_upCollider = nullptr;
	m_upDebugWireFrame = nullptr;

	m_upCollider = std::make_unique<Collider>(m_wpTransformComponent.lock());
	m_upDebugWireFrame = std::make_unique<DebugWireFrame>();

	auto pos = m_wpTransformComponent.lock()->GetPos();
	m_bb.Center = { m_bb.Center.x + pos.x,m_bb.Center.y + pos.y,m_bb.Center.z + pos.z };
	m_bb.Extents = { m_bb.Extents.x / Constant::Two_F,m_bb.Extents.y / Constant::Two_F,m_bb.Extents.z / Constant::Two_F };
	m_upCollider->AddCollisionShape(std::make_shared<BoxCollision>(m_bb, Collider::Type::TypeBump));
	m_bb.Extents = { m_bb.Extents.x * Constant::Two_F,m_bb.Extents.y * Constant::Two_F,m_bb.Extents.z * Constant::Two_F };
	m_upDebugWireFrame->CreateLineCube(m_bb, Math::Color(m_color.x, m_color.y, m_color.z, Constant::One_F));
	m_bb.Center = { m_bb.Center.x - pos.x,m_bb.Center.y - pos.y,m_bb.Center.z - pos.z };
}