#include "PhysicModel.h"

//PhysicModel::PhysicModel()
//{
//	
//}
//
//PhysicModel::PhysicModel(const PhysicModel &obj)
//{
//	m_CI_RigidBody = new btRigidBody::btRigidBodyConstructionInfo(*obj.m_CI_RigidBody);
//	m_RigidBody = new btRigidBody(*m_CI_RigidBody);
//}

btRigidBody* PhysicModel::getRigidBody()
{
	return m_RigidBody;
}

void PhysicModel::ApplySphereShape(float fRadius, float fMass, glm::quat quatInit, glm::vec3 posInit)
{
	btQuaternion quat(quatInit.x, quatInit.y, quatInit.z, quatInit.w);
	btVector3 vec(posInit.x, posInit.y, posInit.z);
	m_CollisionShape = new btSphereShape(fRadius);
	m_DefaultMotionState = new btDefaultMotionState(btTransform(quat,vec));
	btVector3 fallInertia(0, 0, 0);
    m_CollisionShape->calculateLocalInertia(fMass, fallInertia);
	m_CI_RigidBody = new btRigidBody::btRigidBodyConstructionInfo(fMass, m_DefaultMotionState, m_CollisionShape, fallInertia);
	m_RigidBody = new btRigidBody(*m_CI_RigidBody);

}

void PhysicModel::ApplyCapsuleShape(float fRadius, float fHeight, float fMass, glm::quat quatInit, glm::vec3 posInit)
{
	btQuaternion quat(quatInit.w, quatInit.x, quatInit.y, quatInit.z);
	btVector3 vec(posInit.x, posInit.y, posInit.z);
	m_CollisionShape = new btCapsuleShape(fRadius, fHeight);	
	m_DefaultMotionState = new btDefaultMotionState(btTransform(quat,vec));
	btVector3 fallInertia(0, 0, 0);
    m_CollisionShape->calculateLocalInertia(fMass, fallInertia);
	m_CI_RigidBody = new btRigidBody::btRigidBodyConstructionInfo(fMass, m_DefaultMotionState, m_CollisionShape, fallInertia);
	m_RigidBody = new btRigidBody(*m_CI_RigidBody);

	//set kinematic object
	m_RigidBody->setCollisionFlags( m_RigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
	m_RigidBody->setActivationState(DISABLE_DEACTIVATION);
}

void PhysicModel::ApplyIndexedMeshShape(int iNumTriangles, int iNumVertices, GLuint* iIndices, glm::vec3* fVertices)
{
	t_mesh.m_numTriangles = iNumTriangles;
	t_mesh.m_numVertices = iNumVertices;
	t_mesh.m_triangleIndexBase = (unsigned char*)iIndices;
	t_mesh.m_triangleIndexStride = 3*sizeof(int);
	t_mesh.m_vertexBase = (unsigned char*)fVertices;
	t_mesh.m_vertexStride = 3*sizeof(float);
	triVert.addIndexedMesh(t_mesh);

	m_CollisionShape = new btBvhTriangleMeshShape(&triVert, true);
	m_DefaultMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
	m_CI_RigidBody = new btRigidBody::btRigidBodyConstructionInfo(0,m_DefaultMotionState,m_CollisionShape, btVector3(0, 0, 0));		
	m_RigidBody = new btRigidBody(*m_CI_RigidBody);
}

float PhysicModel::gety()
{
	return m_RigidBody->getWorldTransform().getOrigin().getY();
}
