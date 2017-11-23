#include "main_header.h"
#include "Graphics\Mesh.h"
#include "PhysicModel.h"
#include "PhysicsManager.h"

#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_

#define fAngleInc	0.01

enum ObjectState 
{
	FALLING,
	ON_GROUND,
	MOVING,
	JUMPING,
	IDLE,
	START_WALKING,
	STOP_WALKING
};

class GameObject
{
protected:
	Mesh			m_mesh;
	PhysicModel*	m_model;
	PhysicsManager* m_myPhysicsManager;
	btCollisionWorld::ClosestRayResultCallback*	m_RayCallback;
	btCollisionWorld::ClosestRayResultCallback*	m_RayCallbackDirection1;
	btCollisionWorld::ClosestRayResultCallback*	m_RayCallbackDirection2;
	btCollisionWorld::ClosestRayResultCallback*	m_RayCallbackDirection3;
	std::string		m_strName;

	int				m_iState;

	sf::Clock		m_FallingTimer;
	sf::Clock		m_MovingTimer;
	sf::Clock		m_IdleTimer;

	btQuaternion		m_qAll;
	float				m_fAngleAll;

public:
	/*GameObject();
	GameObject(const GameObject &obj);*/
	void CopyMesh(const GameObject &obj);
	void GenerateObject(std::string strMeshFile);
	void GenerateObject(Mesh *mesh);
	void DrawMesh(int Shader);
	void Update(); 
	Mesh* getMesh();
	PhysicModel getModel();
	void ApplyCapsuleShape(float fRadius, float fHeight, float fMass, glm::quat quatInit, glm::vec3 posInit);
	void ApplySphereShape(float fRadius, float fMass, glm::quat quatInit, glm::vec3 posInit);
	void ApplyIndexedMeshShape();
	void ApplyPhysicsManager(PhysicsManager* manager);
	void ApplyGround(GameObject* ground);
	void Raycast(glm::vec3 vStart, glm::vec3 vEnd, btCollisionWorld::ClosestRayResultCallback* RayCallback);
	void RaycastToGround();
	void RayCastDirection();
	void MoveForward();
	void StateMachine();

	void setName(std::string strName);
	std::string getName();
	glm::vec3 getPosition();
	unsigned int getNumAnimations();
	void setState(int iState);

	bool isOnGround();
};

#endif