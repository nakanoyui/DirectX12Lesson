#pragma once

namespace ConstantBuffer
{
	struct cbWorld
	{
		Math::Matrix m_mWorld;
	};

	struct cbMaterial
	{
		Math::Vector4	BaseColor = { 1.0f, 1.0f, 1.0f, 1.0f };
		Math::Vector3	Emissive = { 1.0f, 1.0f, 1.0f };
		float			Metallic = 0.0f;

		float			Roughness = 1.0f;
		float			_blank[3] = { 0.0f, 0.0f ,0.0f };
	};

	struct cbBones
	{
		Math::Matrix	m_mBone;
	};

	struct cbCamera
	{
		Math::Matrix m_mView;
		Math::Matrix m_mProj;
		Math::Vector4 m_camPos;
		Math::Matrix m_mBillboard;
	};

	struct cbLight
	{
		Math::Vector4 m_DL_Dir = {-1.0f,-1.0f,1.0f,1.0f};
		Math::Vector4 m_DL_Color = {2.0f,2.0f,2.0f,1.0f};
		Math::Vector4 m_Ambient_Color = {0.3f,0.3f,0.3f,1.0f};
	};
}