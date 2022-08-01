#pragma once

class CollisionShape;
class TransformComponent;

class Collider
{
public:

	// �Փ˃^�C�v
	enum Type
	{
		TypeGround = 1 << 0,
		TypeBump = 1 << 1,
		TypeAttack = 1 << 2,
	};

	struct RayInfo
	{
		RayInfo() {}
		RayInfo(const Math::Vector3& pos, const Math::Vector3& dir, float range)
			: m_pos(pos), m_dir(dir), m_range(range)
		{
			m_dir.Normalize();
		}

		Math::Vector3 m_pos;		// ���C�̔��ˈʒu
		Math::Vector3 m_dir;		// ���C�̕���
		float m_range = 0;			// ������E����
	};

	struct CollisionResult
	{
		bool m_hit = false;				// �Փ˂������ǂ���
		Math::Vector3 m_hitPos;			// �Փ˂������W
		Math::Vector3 m_hitDir;			// �ΏۂƂ̊Ԃ̕���
		float m_overlapDistance = 0.0f; // �d�Ȃ��
	};

	Collider(std::shared_ptr<TransformComponent>spTransformComponent) :m_wpTransformComponent(spTransformComponent) {}

	~Collider() {}

	void AddCollisionShape(const std::shared_ptr<CollisionShape>& spShape) { m_spCollisions.push_back(spShape); }

	bool Intersects(Type checkType, const DirectX::BoundingSphere& target, std::list<Collider::CollisionResult>* pResults);
	bool Intersects(Type checkType, const DirectX::BoundingBox& target, std::list<Collider::CollisionResult>* pResults);
	bool Intersects(Type checkType, const Collider::RayInfo& target, std::list<Collider::CollisionResult>* pResults);

private:
	std::list<std::shared_ptr<CollisionShape>> m_spCollisions;

	std::weak_ptr<TransformComponent> m_wpTransformComponent;
};

class CollisionShape
{
public:

	CollisionShape(UINT type) { m_type = type; }

	virtual ~CollisionShape() {}

	UINT GetType() const { return m_type; }

	virtual bool Intersects(const DirectX::BoundingSphere& target, const Math::Matrix& world, Collider::CollisionResult* pRes) = 0;
	virtual bool Intersects(const DirectX::BoundingBox& target, const Math::Matrix& world, Collider::CollisionResult* pRes) { return false; }
	virtual bool Intersects(const Collider::RayInfo& target, const Math::Matrix& world, Collider::CollisionResult* pRes) = 0;

private:
	// ���p�̓����蔻�肩
	UINT m_type = 0;
};

// �R���C�_�[�F���`��
class SphereCollision : public CollisionShape
{
public:
	SphereCollision(const DirectX::BoundingSphere& sphere, UINT type) :
		CollisionShape(type), m_shape(sphere) {}
	SphereCollision(const Math::Vector3& localPos, float radius, UINT type) :
		CollisionShape(type) {
		m_shape.Center = localPos; m_shape.Radius = radius;
	}

	virtual ~SphereCollision() {}

	bool Intersects(const DirectX::BoundingSphere& target, const Math::Matrix& world, Collider::CollisionResult* pRes) override;
	bool Intersects(const DirectX::BoundingBox& target, const Math::Matrix& world, Collider::CollisionResult* pRes)override;
	bool Intersects(const Collider::RayInfo& target, const Math::Matrix& world, Collider::CollisionResult* pRes) override;

private:
	DirectX::BoundingSphere m_shape;
};

class BoxCollision :public CollisionShape
{
public:
	BoxCollision(const DirectX::BoundingBox& box, UINT type) :
		CollisionShape(type), m_box(box) {}

	virtual ~BoxCollision() {}

	bool Intersects(const DirectX::BoundingSphere& target, const Math::Matrix& world, Collider::CollisionResult* pRes)override;
	bool Intersects(const Collider::RayInfo& target, const Math::Matrix& world, Collider::CollisionResult* pRes)override { return false; }

private:
	DirectX::BoundingBox m_box;
};

class ModelCollision : public CollisionShape
{
public:
	ModelCollision(const std::shared_ptr<ModelWork>& model, UINT type) :
		CollisionShape(type), m_shape(model) {}

	virtual ~ModelCollision() { m_shape = nullptr; }

	bool Intersects(const DirectX::BoundingSphere& target, const Math::Matrix& world, Collider::CollisionResult* pRes) override;
	bool Intersects(const Collider::RayInfo& target, const Math::Matrix& world, Collider::CollisionResult* pRes) override;

private:
	std::shared_ptr<ModelWork> m_shape = nullptr;
};