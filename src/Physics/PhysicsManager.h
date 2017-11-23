#include "main_header.h"
#include "PhysicModel.h"

#ifndef PHYSICSMANAGER_H_
#define PHYSICSMANAGER_H_

class PhysicsManager
{
	private:
	
		btBroadphaseInterface*						m_broadphase ;
		btSequentialImpulseConstraintSolver*		m_solver;
		btDefaultCollisionConfiguration*			m_collisionConfiguration;
		btCollisionDispatcher*						m_dispatcher;
		std::vector<PhysicModel*>					m_models;
		btCollisionWorld::ClosestRayResultCallback*	m_RayCallback;
	public:
		//physical world
		btDiscreteDynamicsWorld*				m_dynamicsWorld;
		btRigidBody* fallRigidBody;
		void Initialize();
		bool isModelOnGround(PhysicModel* model);
		void Update();
		void AddPhysicModel(PhysicModel* NewModel);
		glm::vec3 getRaycastHitPoint();
};

#endif