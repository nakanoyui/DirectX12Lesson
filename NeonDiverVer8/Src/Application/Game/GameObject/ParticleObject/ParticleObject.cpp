#include "ParticleObject.h"

nlohmann::json ParticleObject::SetJsonParameter()
{
	auto objects = nlohmann::json::object();
	auto array = nlohmann::json::object();

	// General
	objects["ParticleNum"] = m_particleNum;
	array["X"] = m_transform.Positon().x;
	array["Y"] = m_transform.Positon().y;
	array["Z"] = m_transform.Positon().z;
	objects["Position"] = array;
	array.clear();
	array["X"] = m_euler.x;
	array["Y"] = m_euler.y;
	array["Z"] = m_euler.z;
	objects["Rotate"] = array;
	array.clear();
	array["X"] = m_transform.Scale().x;
	array["Y"] = m_transform.Scale().y;
	array["Z"] = m_transform.Scale().z;
	objects["Scale"] = array;
	array.clear();
	objects["DefaultTexture"] = m_bDefaultTexture;
	objects["LaserTexture"] = m_bLaserTexture;

	// Emitte
	objects["EmitteParticlePerSecpnd"] = m_emitteParticlePerSecond;
	objects["ParticleCyclePerFrame"] = m_particleCyclePerFrame;
	array["X"] = m_emitterData.m_startPosition.x;
	array["Y"] = m_emitterData.m_startPosition.y;
	array["Z"] = m_emitterData.m_startPosition.z;
	objects["StartPosition"] = array;
	array.clear();
	array["X"] = m_emitterData.m_startScale.x;
	array["Y"] = m_emitterData.m_startScale.y;
	array["Z"] = m_emitterData.m_startScale.z;
	objects["StartScale"] = array;
	array.clear();
	objects["StartRotate"] = m_emitterData.m_startRotate;
	objects["MAX_LIFE"] = m_emitterData.MAX_LIFE;
	array["X"] = m_emitterData.m_velocity.x;
	array["Y"] = m_emitterData.m_velocity.y;
	array["Z"] = m_emitterData.m_velocity.z;
	objects["Velocity"] = array;
	array.clear();
	array["X"] = m_emitterData.m_startForce.x;
	array["Y"] = m_emitterData.m_startForce.y;
	array["Z"] = m_emitterData.m_startForce.z;
	objects["StartForce"] = array;
	array.clear();
	array["X"] = m_emitterData.m_endForce.x;
	array["Y"] = m_emitterData.m_endForce.y;
	array["Z"] = m_emitterData.m_endForce.z;
	objects["EndForce"] = array;
	array.clear();
	array["X"] = m_emitterData.m_randomMin.x;
	array["Y"] = m_emitterData.m_randomMin.y;
	array["Z"] = m_emitterData.m_randomMin.z;
	objects["RandomMin"] = array;
	array.clear();
	array["X"] = m_emitterData.m_randomMax.x;
	array["Y"] = m_emitterData.m_randomMax.y;
	array["Z"] = m_emitterData.m_randomMax.z;
	objects["RandomMax"] = array;
	array.clear();
	array["X"] = m_emitterData.m_startColor.x;
	array["Y"] = m_emitterData.m_startColor.y;
	array["Z"] = m_emitterData.m_startColor.z;
	objects["StartColor"] = array;
	array.clear();
	array["X"] = m_emitterData.m_endColor.x;
	array["Y"] = m_emitterData.m_endColor.y;
	array["Z"] = m_emitterData.m_endColor.z;
	objects["EndColor"] = array;
	array.clear();
	objects["SizeByLifeTime"] = m_emitterData.m_sizeByLifeTime;

	return objects;
}

void ParticleObject::Init()
{
	GeneralInit();

	EmitteInit();
}

void ParticleObject::SliderInt(const char* label, int& in, int min, int max)
{
	ImGui::SliderInt(label, &in, min, max);
	ImGui::Separator();
}

void ParticleObject::SliderFloat(const char* label, float& in, float min, float max)
{
	ImGui::SliderFloat(label, &in, min, max);
	ImGui::Separator();
}

void ParticleObject::SliderFloat3(const char* label, Math::Vector3& in, float min, float max)
{
	float temp[] = { in.x, in.y, in.z };

	ImGui::SliderFloat3(label, temp, min, max);
	ImGui::Separator();

	in = { temp[0], temp[1], temp[2] };
}

void ParticleObject::ColorEdit3(const char* label, Math::Vector3& in)
{
	float temp[] = { in.x, in.y, in.z };

	ImGui::ColorEdit3(label, temp);
	ImGui::Separator();

	in = { temp[0], temp[1], temp[2] };
}

void ParticleObject::GeneralInit()
{
	m_particleNum = m_wpJson.lock()->GetConstJson().at("ParticleNum");

	m_transform.Positon(Math::Vector3(
		m_wpJson.lock()->GetConstJson().at("Position").at("X"),
		m_wpJson.lock()->GetConstJson().at("Position").at("Y"),
		m_wpJson.lock()->GetConstJson().at("Position").at("Z")));

	m_euler = Math::Vector3(
		m_wpJson.lock()->GetConstJson().at("Rotate").at("X"),
		m_wpJson.lock()->GetConstJson().at("Rotate").at("Y"),
		m_wpJson.lock()->GetConstJson().at("Rotate").at("Z"));
	m_transform.RotationEuler(m_euler);

	m_transform.Scale(Math::Vector3(
		m_wpJson.lock()->GetConstJson().at("Scale").at("X"),
		m_wpJson.lock()->GetConstJson().at("Scale").at("Y"),
		m_wpJson.lock()->GetConstJson().at("Scale").at("Z")));

	m_bDefaultTexture = m_wpJson.lock()->GetConstJson().at("DefaultTexture");
	m_bLaserTexture = m_wpJson.lock()->GetConstJson().at("LaserTexture");

	if (m_bDefaultTexture)
	{
		m_upParticleEmitter->SetTexture(m_wpAssetManager.lock()->GetTexture(m_wpJsonManager.lock()->GetAssetJson().at("Texture").at("Particle").at("Default")).get());
	}

	if (m_bLaserTexture)
	{
		m_upParticleEmitter->SetTexture(m_wpAssetManager.lock()->GetTexture(m_wpJsonManager.lock()->GetAssetJson().at("Texture").at("Particle").at("Laser")).get());
	}
}

void ParticleObject::EmitteInit()
{
	m_emitteParticlePerSecond = m_wpJson.lock()->GetConstJson().at("EmitteParticlePerSecpnd");
	m_particleCyclePerFrame = m_wpJson.lock()->GetConstJson().at("ParticleCyclePerFrame");

	m_emitterData.m_startPosition = Math::Vector3(
		m_wpJson.lock()->GetConstJson().at("StartPosition").at("X"),
		m_wpJson.lock()->GetConstJson().at("StartPosition").at("Y"),
		m_wpJson.lock()->GetConstJson().at("StartPosition").at("Z"));

	m_emitterData.m_startScale = Math::Vector3(
		m_wpJson.lock()->GetConstJson().at("StartScale").at("X"),
		m_wpJson.lock()->GetConstJson().at("StartScale").at("Y"),
		m_wpJson.lock()->GetConstJson().at("StartScale").at("Z"));

	m_emitterData.m_startRotate = m_wpJson.lock()->GetConstJson().at("StartRotate");

	m_emitterData.MAX_LIFE = m_wpJson.lock()->GetConstJson().at("MAX_LIFE");

	m_emitterData.m_velocity = Math::Vector3(
		m_wpJson.lock()->GetConstJson().at("Velocity").at("X"),
		m_wpJson.lock()->GetConstJson().at("Velocity").at("Y"),
		m_wpJson.lock()->GetConstJson().at("Velocity").at("Z"));

	m_emitterData.m_startForce = Math::Vector3(
		m_wpJson.lock()->GetConstJson().at("StartForce").at("X"),
		m_wpJson.lock()->GetConstJson().at("StartForce").at("Y"),
		m_wpJson.lock()->GetConstJson().at("StartForce").at("Z"));

	m_emitterData.m_endForce = Math::Vector3(
		m_wpJson.lock()->GetConstJson().at("EndForce").at("X"),
		m_wpJson.lock()->GetConstJson().at("EndForce").at("Y"),
		m_wpJson.lock()->GetConstJson().at("EndForce").at("Z"));

	m_emitterData.m_randomMin = Math::Vector3(
		m_wpJson.lock()->GetConstJson().at("RandomMin").at("X"),
		m_wpJson.lock()->GetConstJson().at("RandomMin").at("Y"),
		m_wpJson.lock()->GetConstJson().at("RandomMin").at("Z"));

	m_emitterData.m_randomMax = Math::Vector3(
		m_wpJson.lock()->GetConstJson().at("RandomMax").at("X"),
		m_wpJson.lock()->GetConstJson().at("RandomMax").at("Y"),
		m_wpJson.lock()->GetConstJson().at("RandomMax").at("Z"));

	m_emitterData.m_startColor = Math::Vector3(
		m_wpJson.lock()->GetConstJson().at("StartColor").at("X"),
		m_wpJson.lock()->GetConstJson().at("StartColor").at("Y"),
		m_wpJson.lock()->GetConstJson().at("StartColor").at("Z"));

	m_emitterData.m_endColor = Math::Vector3(
		m_wpJson.lock()->GetConstJson().at("EndColor").at("X"),
		m_wpJson.lock()->GetConstJson().at("EndColor").at("Y"),
		m_wpJson.lock()->GetConstJson().at("EndColor").at("Z"));

	m_emitterData.m_sizeByLifeTime = m_wpJson.lock()->GetConstJson().at("SizeByLifeTime");
}
