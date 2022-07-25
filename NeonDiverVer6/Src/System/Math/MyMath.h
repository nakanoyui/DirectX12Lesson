#pragma once

namespace Constant
{
	constexpr int Zero_I = 0;
	constexpr float Zero_F = 0.0f;
	constexpr int One_I = 1;
	constexpr float One_F = 1.0f;
	constexpr int Two_I = 2;
	constexpr float Two_F = 2.0f;
	
	constexpr DirectX::SimpleMath::Vector2 ScreenSize = { 1280,720 };

	constexpr float SpriteFontAttenuation = 0.01f;
}

namespace MyMath
{
	float lerp(const float& a,const float& b,const float& t);

	const Math::Vector3& Slerp(const Math::Vector3& start, const Math::Vector3& end, float t);

	const Math::Vector3& RouteCurve(const Math::Vector3& start, const Math::Vector3& end, const Math::Vector3& control, float t);

	class Transform
	{
	public:
		Transform() :m_position(Math::Vector3::Zero), m_scale(Math::Vector3::One), m_quaternion(Math::Quaternion::Identity) {}

		const Math::Vector3& Positon()const { return m_position; }
		void Positon(float pos) { m_position = { pos,pos,pos }; }
		void Positon(const Math::Vector3& pos) { m_position = pos; }

		const Math::Quaternion& Quaternion()const { return m_quaternion; }

		void Quaternion(const Math::Quaternion& quaternion) { m_quaternion = quaternion; }

		void RotationX(float angle)
		{
			m_quaternion = DirectX::XMQuaternionRotationAxis(Math::Vector3(1, 0, 0), DirectX::XMConvertToRadians(angle));
		}

		void RotationY(float angle)
		{
			m_quaternion = DirectX::XMQuaternionRotationAxis(Math::Vector3(0, 1, 0), DirectX::XMConvertToRadians(angle));
		}

		void RotationZ(float angle)
		{
			m_quaternion = DirectX::XMQuaternionRotationAxis(Math::Vector3(0, 0, 1), DirectX::XMConvertToRadians(angle));
		}

		void RotationEuler(float yaw, float pitch, float roll)
		{
			m_quaternion = DirectX::XMQuaternionRotationRollPitchYaw(pitch, yaw, roll);
		}

		void RotationEuler(const Math::Vector3& euler)
		{
			m_quaternion = DirectX::XMQuaternionRotationRollPitchYawFromVector(euler);
		}

		const Math::Vector3& Scale()const { return m_scale; }
		void Scale(float scale) { m_scale = { scale,scale,scale }; }
		void Scale(const Math::Vector3& scale) { m_scale = scale; }

		const Math::Matrix& Matrix()const { return m_matrix; }
		void Matrix(const Math::Matrix& matrix) { m_matrix = matrix; }

		void MatrixComposition()
		{
			Math::Matrix mTrans = Math::Matrix::CreateTranslation(m_position);
			Math::Matrix mRotate = Math::Matrix::CreateFromQuaternion(m_quaternion);
			Math::Matrix mScale = Math::Matrix::CreateScale(m_scale);

			m_matrix = mScale * mRotate * mTrans;
		}

	private:
		Math::Vector3 m_position;
		Math::Quaternion m_quaternion;
		Math::Vector3 m_scale;

		Math::Matrix m_matrix;
	};
}