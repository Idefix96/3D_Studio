#include "PhysicsManager.h"

void PhysicsManager::Initialize()
{
	//world physics
	m_broadphase = new btDbvtBroadphase();
	m_solver = new btSequentialImpulseConstraintSolver;
	m_collisionConfiguration = new btDefaultCollisionConfiguration();
	m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
	m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_broadphase, m_solver, m_collisionConfiguration);

	//world gravity	
    m_dynamicsWorld->setGravity(btVector3(0, -10, 0));

	btCollisionShape* fallShape = new btSphereShape(1);

	btDefaultMotionState* fallMotionState =
		new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 50, 0)));
	btScalar mass = 1;
	btVector3 fallInertia(0, 0, 0);
	fallShape->calculateLocalInertia(mass, fallInertia);
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, fallShape, fallInertia);
	fallRigidBody = new btRigidBody(fallRigidBodyCI);
	m_dynamicsWorld->addRigidBody(fallRigidBody);
}

bool PhysicsManager::isModelOnGround(PhysicModel* model)
{
	btVector3 lRayStart = btVector3(model->getRigidBody()->getWorldTransform().getOrigin().getX(), model->getRigidBody()->getWorldTransform().getOrigin().getY(), model->getRigidBody()->getWorldTransform().getOrigin().getZ());
	btVector3 lRayEnd = btVector3(model->getRigidBody()->getWorldTransform().getOrigin().getX(), model->getRigidBody()->getWorldTransform().getOrigin().getY() - 1000.0, model->getRigidBody()->getWorldTransform().getOrigin().getZ());
	
	m_RayCallback = new btCollisionWorld::ClosestRayResultCallback(lRayStart, lRayEnd);
	m_dynamicsWorld->rayTest(
	lRayStart, lRayEnd,
	*m_RayCallback//RayCallback
	);

	if(m_RayCallback->hasHit()) 
	{
		if (lRayStart.y() - m_RayCallback->m_hitPointWorld.y() >10.0)	
				return false;
		else
			return true;
	}
	else
		return false;
}

glm::vec3 PhysicsManager::getRaycastHitPoint()
{
	return glm::vec3( m_RayCallback->m_hitPointWorld.x(), m_RayCallback->m_hitPointWorld.y(), m_RayCallback->m_hitPointWorld.z());
}

void PhysicsManager::Update()
{ 
	 m_dynamicsWorld->stepSimulation(1 / 60.f, 10);
}

void PhysicsManager::AddPhysicModel(PhysicModel* NewModel)
{
	m_dynamicsWorld->addRigidBody(NewModel->getRigidBody());

	m_models.push_back(NewModel);
}