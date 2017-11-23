#include "main_header.h"
#include "Graphics\Mesh.h"


#ifndef PHYSICMODEL_H_
#define PHYSICMODEL_H_

class PhysicModel
{
private:
	 btCollisionShape*							m_CollisionShape;
	 btDefaultMotionState*						m_DefaultMotionState;
	 btRigidBody::btRigidBodyConstructionInfo*  m_CI_RigidBody;
	 btRigidBody*								m_RigidBody;
	 btTransform								m_trans;
	 btTriangleIndexVertexArray triVert;
	 btIndexedMesh t_mesh;
public:
	//PhysicModel();
	//PhysicModel(const PhysicModel &obj);
	btRigidBody* getRigidBody();
	void ApplySphereShape(float fRadius, float fMass, glm::quat quatInit, glm::vec3 posInit);
	void ApplyCapsuleShape(float fRadius, float fHeight, float fMass, glm::quat quatInit, glm::vec3 posInit);
	void ApplyIndexedMeshShape(int iNumTriangles, int iNumVertices, GLuint* iIndices, glm::vec3* fVertices);
	float gety();
};

#endif