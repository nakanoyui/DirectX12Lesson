#pragma once

class CollisionShape;

class Collider
{
public:

	// 衝突タイプ
	enum Type
	{
		TypeGround = 1 << 0,
		TypeBump = 1 << 1,
		TypeAttack = 1 << 2,
		TypeAttackLine = 1 << 3,
	};

	struct RayInfo
	{
		RayInfo() {}
		RayInfo(const Math::Vector3& pos, const Math::Vector3& dir, float range)
			: m_pos(pos), m_dir(dir), m_range(range)
		{
			m_dir.Normalize();
		}

		Math::Vector3 m_pos;		// レイの発射位置
		Math::Vector3 m_dir;		// レイの方向
		float m_range = 0;			// 判定限界距離
	};

	struct CollisionResult
	{
		bool m_hit = false;				// 衝突したかどうか
		Math::Vector3 m_hitPos;			// 衝突した座標
		Math::Vector3 m_hitDir;			// 対象との間の方向
		float m_overlapDistance = 0.0f; // 重なり量
	};

	Collider(GameObject& rOwner) :m_owner(rOwner) {}

	~Collider() {}

	void AddCollisionShape(std::shared_ptr<CollisionShape> spShape)
	{
		if (!spShape) { return; }

		m_collisions.push_back(spShape);
	}

	bool Intersects(Type checkType, const DirectX::BoundingSphere& target, std::list<Collider::CollisionResult>* pResults);
	bool Intersects(Type checkType, const DirectX::BoundingBox& target, std::list<Collider::CollisionResult>* pResults);
	bool Intersects(Type checkType, const Collider::RayInfo& target, std::list<Collider::CollisionResult>* pResults);

private:
	std::list<std::shared_ptr<CollisionShape>> m_collisions;

	GameObject& m_owner;
};

class CollisionShape
{
public:

	CollisionShape(UINT type) { m_type = type; }

	virtual ~CollisionShape() {}

	UINT GetType() const { return m_type; }

	virtual bool Intersects(const DirectX::BoundingSphere& target, const Math::Matrix& world, Collider::CollisionResult* pRes) = 0;
	virtual bool Intersects(const DirectX::BoundingBox& target, const Math::Matrix& world, Collider::CollisionResult* pRes) {return false;}
	virtual bool Intersects(const Collider::RayInfo& target, const Math::Matrix& world, Collider::CollisionResult* pRes) = 0;

private:
	// 何用の当たり判定か
	UINT m_type = 0;
};

// コライダー：球形状
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

// コライダー：AABB形状
class AABBCollision :public CollisionShape
{
public:
	AABBCollision(const DirectX::BoundingBox& aabb, UINT type) :
		CollisionShape(type), m_aabb(aabb) {}

	virtual ~AABBCollision() {}

	bool Intersects(const DirectX::BoundingSphere& target, const Math::Matrix& world, Collider::CollisionResult* pRes)override;
	bool Intersects(const Collider::RayInfo& target, const Math::Matrix& world, Collider::CollisionResult* pRes)override { return false; }

private:
	DirectX::BoundingBox m_aabb;
};

// コライダー：モデル形状
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