#include "Collider.hpp"

#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include "Maths/Maths.hpp"
#include "Objects/GameObject.hpp"

namespace acid
{
	btVector3 Collider::Convert(const Vector3 &vector)
	{
		return btVector3(vector.m_x, vector.m_y, vector.m_z);
	}

	Vector3 Collider::Convert(const btVector3 &vector)
	{
		return Vector3(vector.getX(), vector.getY(), vector.getZ());
	}

	btQuaternion Collider::Convert(const Quaternion &quaternion)
	{
		return btQuaternion(quaternion.m_x, quaternion.m_y, quaternion.m_z, quaternion.m_w);
	}

	Quaternion Collider::Convert(const btQuaternion &quaternion)
	{
		return Quaternion(quaternion.getX(), quaternion.getY(), quaternion.getZ(), quaternion.getW());
	}

	btTransform Collider::Convert(const Transform &transform)
	{
		btTransform worldTransform = btTransform();
		worldTransform.setIdentity();
		worldTransform.setOrigin(Collider::Convert(transform.GetPosition()));
		worldTransform.setRotation(btQuaternion(transform.GetRotation().m_y, transform.GetRotation().m_x, transform.GetRotation().m_z));
		return worldTransform;
	}

	Transform Collider::Convert(const btTransform &transform, const Vector3 &scaling)
	{
		btVector3 position = transform.getOrigin();

		btQuaternion rotation = transform.getRotation();
		float pitch, yaw, roll;
		rotation.getEulerZYX(yaw, pitch, roll);

		return Transform(Collider::Convert(position), Vector3(pitch * RAD_TO_DEG, yaw * RAD_TO_DEG, roll * RAD_TO_DEG), scaling);
	}
}
