#include "Laser.h"

void Laser::Init()
{
	ReferenceJson();

	ParticleObject::Init();

	m_wpTextureManager = Application::Instance().GetGameSystem()->GetTextureManager();

	m_upParticleEmitter = std::make_unique<DefaultGPUParticleEmitter>();
	m_upParticleEmitter->SetComputeShader(Application::Instance().GetGameSystem()->GetComputePipeline());
	m_upParticleEmitter->CreateParticleEmitter(GraphicsDevice::Instance().GetGPUParticleManager().get(), m_particleNum);
}

void Laser::Update()
{
	m_upParticleEmitter->Update(m_emitteParticlePerSecond, m_particleCyclePerFrame);

	m_transform.MatrixComposition();
}

void Laser::DrawParticle()
{
	m_upParticleEmitter->Draw(m_transform.Matrix());
}

void Laser::ImGuiParticleUpdate()
{
	if (ImGui::TreeNode("Laser"))
	{
		if (ImGui::TreeNode("General"))
		{
			SliderFloat("ParticleNum", m_particleNum, 0, 2560);

			{
				float temp[] = { m_transform.Positon().x, m_transform.Positon().y, m_transform.Positon().z };

				ImGui::SliderFloat3("Position", temp, -50, 50);
				ImGui::Separator();

				m_transform.Positon({ temp[0],temp[1],temp[2] });
			}

			{
				float temp[] = { m_euler.x, m_euler.y, m_euler.z };

				ImGui::SliderFloat3("Rotate", temp, -M_PI * ToDegrees, M_PI * ToDegrees);
				ImGui::Separator();

				if (abs(temp[0]) > 90)
				{
					int a = 0;
				}

				m_euler = { temp[0],temp[1],temp[2] };
				m_transform.RotationEuler(m_euler);
			}

			{
				float temp[] = { m_transform.Scale().x, m_transform.Scale().y, m_transform.Scale().z };

				ImGui::SliderFloat3("Scale", temp, -50, 50);
				ImGui::Separator();

				m_transform.Scale({ temp[0],temp[1],temp[2] });
			}

			ImGui::Checkbox("DefaultTexture", &m_bDefaultTexture);
			if (m_bDefaultTexture)
			{
				m_bLaserTexture = false;
				m_upParticleEmitter->SetTexture(m_wpTextureManager.lock()->GetTexture("Particle.png").get());
			}

			ImGui::Checkbox("LaserTexture", &m_bLaserTexture);
			if (m_bLaserTexture)
			{
				m_bDefaultTexture = false;
				m_upParticleEmitter->SetTexture(m_wpTextureManager.lock()->GetTexture("Laser.png").get());
			}

			if (ImGui::Button("Reset"))
			{
				GeneralInit();
			}

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Emitter"))
		{
			SliderInt("EmitteParticlePerSecond", m_emitteParticlePerSecond, 0, 30);

			SliderInt("ParticleCyclePerFrame", m_particleCyclePerFrame, 0, 30);

			SliderFloat3("StartPosition", m_emitterData.m_startPosition, -50, 50);

			SliderFloat3("StartScale", m_emitterData.m_startScale, 0, 5);

			SliderFloat("StartRotate", m_emitterData.m_startRotate, Constant::Zero_F, M_PI * Constant::Two_F);

			SliderFloat("LifeTime", m_emitterData.MAX_LIFE, 0, 600);

			SliderFloat3("Velocity", m_emitterData.m_velocity, -1, 1);

			SliderFloat3("StartForce", m_emitterData.m_startForce, 0, 10);

			SliderFloat3("EndForce", m_emitterData.m_endForce, 0, 10);

			SliderFloat3("RandomMin", m_emitterData.m_randomMin, -1, 1);

			SliderFloat3("RandomMax", m_emitterData.m_randomMax, -1, 1);

			ColorEdit3("StartColor", m_emitterData.m_startColor);

			ColorEdit3("EndColor", m_emitterData.m_endColor);

			ImGui::Checkbox("SizeByLifeTime", &m_emitterData.m_sizeByLifeTime);

			if (ImGui::Button("Reset"))
			{
				EmitteInit();
			}

			ImGui::TreePop();
		}

		if (ImGui::Button("BakeJson"))
		{
			m_wpJson.lock()->BakeJson(SetJsonParameter());
		}

		ImGui::TreePop();
	}

	m_upParticleEmitter->Set(m_emitterData);
}

void Laser::ReferenceJson()
{
	m_wpJsonManager = Application::Instance().GetGameSystem()->GetJsonManager();
	m_wpJson = m_wpJsonManager.lock()->GetJson("Laser");
}
