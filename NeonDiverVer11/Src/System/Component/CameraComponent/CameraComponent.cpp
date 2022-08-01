#include "CameraComponent.h"

#include "Application/Application.h"

CameraComponent::CameraComponent()
{
	m_name = "CameraComponent";
}

void CameraComponent::Init()
{
	m_wpCameraManager = Application::Instance().GetCameraManager();

	ReferenceJson();

	m_spCamera = std::make_shared<TPSCamera>();
	SetCameraInfo();
}

void CameraComponent::Start()
{
	m_wpTransformComponent = m_wpGameObject.lock()->GetComponent<TransformComponent>();
}

void CameraComponent::Update()
{
	m_wpCameraManager.lock()->SetCamera(m_spCamera);
	m_wpCameraManager.lock()->SetMatrix(m_wpTransformComponent.lock()->GetMatrix());
}

void CameraComponent::ImGuiUpdateInspecter()
{
	if (ImGui::TreeNode(m_name.c_str()))
	{
		ImGui::InputFloat("Fov",&m_camInfo.Fov);
		ImGui::Separator();

		ImGui::InputFloat("MaxRange", &m_camInfo.MaxRange);
		ImGui::Separator();

		ImGui::InputFloat("MinRange", &m_camInfo.MinRange);
		ImGui::Separator();

		ImGui::InputFloat("AspectRatio", &m_camInfo.AspectRatio);
		ImGui::Separator();

		ImGuiHelper::InputFloat3("LocalPos",m_camInfo.LocalPos);

		ImGuiHelper::InputFloat3("LocalGazePos",m_camInfo.LocalGazePos);

		ImGuiHelper::InputFloat2("ClampAngleX",m_camInfo.ClampAngleX);

		ImGuiHelper::InputFloat2("ClampAngleY",m_camInfo.ClampAngleY);

		SetCameraInfo();

		if (ImGui::Button("Reset"))
		{
			ReferenceJson();
		}
		ImGui::Separator();

		ImGui::TreePop();
	}
}

void CameraComponent::PrepareJsonParameter(nlohmann::json& json)
{
	auto array = nlohmann::json::array();

	json["Fov"] = m_camInfo.Fov;
	json["MaxRange"] = m_camInfo.MaxRange;
	json["MinRange"] = m_camInfo.MinRange;
	json["AspectRatio"] = m_camInfo.AspectRatio;
	array.clear();
	array.push_back(m_camInfo.LocalPos.x);
	array.push_back(m_camInfo.LocalPos.y);
	array.push_back(m_camInfo.LocalPos.z);
	json["LocalPosition"] = array;
	array.clear();
	array.push_back(m_camInfo.LocalGazePos.x);
	array.push_back(m_camInfo.LocalGazePos.y);
	array.push_back(m_camInfo.LocalGazePos.z);
	json["LocalGazePosition"] = array;
	array.clear();
	array.push_back(m_camInfo.ClampAngleX.x);
	array.push_back(m_camInfo.ClampAngleX.y);
	json["ClampAngleX"] = array;
	array.clear();
	array.push_back(m_camInfo.ClampAngleY.x);
	array.push_back(m_camInfo.ClampAngleY.y);
	json["ClampAngleY"] = array;
}

void CameraComponent::ReferenceJson()
{
	ImGuiHelper::CheckData(m_wpGameObject.lock()->GetJsonManager().lock()->GetJson(m_objectName)->WorkConstJson(), m_camInfo.Fov, "Fov", CameraInfo().Fov);

	ImGuiHelper::CheckData(m_wpGameObject.lock()->GetJsonManager().lock()->GetJson(m_objectName)->WorkConstJson(), m_camInfo.MaxRange, "MaxRange", CameraInfo().MaxRange);

	ImGuiHelper::CheckData(m_wpGameObject.lock()->GetJsonManager().lock()->GetJson(m_objectName)->WorkConstJson(), m_camInfo.MinRange, "MinRange", CameraInfo().MinRange);

	ImGuiHelper::CheckData(m_wpGameObject.lock()->GetJsonManager().lock()->GetJson(m_objectName)->WorkConstJson(), m_camInfo.AspectRatio, "AspectRatio", CameraInfo().AspectRatio);

	ImGuiHelper::CheckDataVector3(m_wpGameObject.lock()->GetJsonManager().lock()->GetJson(m_objectName)->WorkConstJson(), m_camInfo.LocalPos, "LocalPosition", CameraInfo().LocalPos);

	ImGuiHelper::CheckDataVector3(m_wpGameObject.lock()->GetJsonManager().lock()->GetJson(m_objectName)->WorkConstJson(), m_camInfo.LocalGazePos, "LocalGazePosition", CameraInfo().LocalGazePos);

	ImGuiHelper::CheckDataVector2(m_wpGameObject.lock()->GetJsonManager().lock()->GetJson(m_objectName)->WorkConstJson(), m_camInfo.ClampAngleX, "ClampAngleX", CameraInfo().ClampAngleX);

	ImGuiHelper::CheckDataVector2(m_wpGameObject.lock()->GetJsonManager().lock()->GetJson(m_objectName)->WorkConstJson(), m_camInfo.ClampAngleY, "ClampAngleY", CameraInfo().ClampAngleY);
}

void CameraComponent::SetCameraInfo()
{
	m_spCamera->SetProjectionMatrix(m_camInfo.Fov, m_camInfo.MaxRange, m_camInfo.MinRange, m_camInfo.AspectRatio);
	m_spCamera->SetLocalPos(Math::Vector3(m_camInfo.LocalPos));
	m_spCamera->SetLocalGazePos(Math::Vector3(m_camInfo.LocalGazePos));
	m_spCamera->SetClampAngleX(m_camInfo.ClampAngleX.x, m_camInfo.ClampAngleX.y);
	m_spCamera->SetClampAngleY(m_camInfo.ClampAngleY.x, m_camInfo.ClampAngleY.y);
}