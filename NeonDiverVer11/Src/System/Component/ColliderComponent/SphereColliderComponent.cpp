#include "SphereColliderComponent.h"

SphereColliderComponent::SphereColliderComponent()
{
	m_name = "SphereColliderComponent";
}

void SphereColliderComponent::Init()
{
	ColliderComponent::Init();

	ReferenceJson();
}

void SphereColliderComponent::Start()
{
	ColliderComponent::Start();

	CreateCollision();
}

void SphereColliderComponent::Update()
{
	CreateCollision();

	if (m_upDebugWireFrame)
	{
		m_wpDrawManager.lock()->GetDrawList("EffectShader")->AddDrawList(m_wpGameObject.lock());
	}
}

void SphereColliderComponent::ImGuiUpdateInspecter()
{
	if (ImGui::TreeNode(m_name.c_str()))
	{
		m_bs.Center = ImGuiHelper::InputFloat3("Center", Math::Vector3(m_bs.Center));

		ImGui::InputFloat("Radius", &m_bs.Radius);

		ImGuiHelper::ColorEdit3("Color", m_color);

		if (ImGui::Button("Reset"))
		{
			ReferenceJson();
		}
		ImGui::Separator();

		ImGui::TreePop();
	}
}

void SphereColliderComponent::PrepareJsonParameter(nlohmann::json& json)
{
	auto array = nlohmann::json::array();

	array.clear();
	array.push_back(m_bs.Center.x);
	array.push_back(m_bs.Center.y);
	array.push_back(m_bs.Center.z);
	json["Center"] = array;
	json["Radius"] = m_bs.Radius;
	array.clear();
	array.push_back(m_color.x);
	array.push_back(m_color.y);
	array.push_back(m_color.z);
	json["Color"] = array;
	array.clear();
}

void SphereColliderComponent::ReferenceJson()
{
	m_bs.Center = ImGuiHelper::CheckDataVector3(m_wpGameObject.lock()->GetJsonManager().lock()->GetJson(m_objectName)->WorkConstJson(), Math::Vector3(m_bs.Center), "Center", Math::Vector3(Constant::Zero_F));

	m_bs.Radius = ImGuiHelper::CheckData(m_wpGameObject.lock()->GetJsonManager().lock()->GetJson(m_objectName)->WorkConstJson(), m_bs.Radius, "Radius", Constant::One_F);

	ImGuiHelper::CheckDataVector3(m_wpGameObject.lock()->GetJsonManager().lock()->GetJson(m_objectName)->WorkConstJson(), m_color, "Color", Math::Vector3(Constant::One_F));
}

void SphereColliderComponent::CreateCollision()
{
	m_upCollider = nullptr;
	m_upDebugWireFrame = nullptr;

	m_upCollider = std::make_unique<Collider>(m_wpTransformComponent.lock());
	m_upDebugWireFrame = std::make_unique<DebugWireFrame>();

	auto pos = m_wpTransformComponent.lock()->GetPos();
	m_bs.Center = { m_bs.Center.x + pos.x,m_bs.Center.y + pos.y,m_bs.Center.z + pos.z };
	m_upCollider->AddCollisionShape(std::make_shared<SphereCollision>(m_bs, Collider::Type::TypeBump));
	m_upDebugWireFrame->CreateLineSphere(m_bs, 50, Math::Color(m_color.x, m_color.y, m_color.z, Constant::One_F));
	m_bs.Center = { m_bs.Center.x - pos.x,m_bs.Center.y - pos.y,m_bs.Center.z - pos.z };
}