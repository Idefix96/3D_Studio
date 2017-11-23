#include "GameObject.h"

//GameObject::GameObject()
//{
//
//}
//
//GameObject::GameObject(const GameObject &obj)
//{
//	m_mesh = obj.m_mesh;
//	m_model = new PhysicModel(*obj.m_model);
//}

void GameObject::GenerateObject(std::string strMeshFile)
{	

	m_mesh.LoadAssimpFile(strMeshFile);

	std::size_t findFS = strMeshFile.find(".");
	m_strName = strMeshFile.substr(0, findFS);
	
	m_model = new PhysicModel();
	m_iState = IDLE;

	m_qAll = btQuaternion(0,0,0,1);
	m_fAngleAll = 0.0f;
}

void GameObject::CopyMesh(const GameObject &obj)
{
	m_mesh = obj.m_mesh;
	m_model = new PhysicModel();

}

void GameObject::DrawMesh(int Shader)
{
	m_mesh.Render(Shader);
}


void GameObject::Update()
{
	btTransform trans;
	m_model->getRigidBody()->getMotionState()->getWorldTransform(trans);

	glm::vec3 vMiddleOfMesh = m_mesh.getBoundingCenter();
	
	//Synchronize physical model and mesh
	if (m_model->getRigidBody()->getInvMass() > 0)
	{
		m_mesh.Translate(glm::vec3(trans.getOrigin().getX() - vMiddleOfMesh.x, trans.getOrigin().getY() - vMiddleOfMesh.y, trans.getOrigin().getZ() - vMiddleOfMesh.z));
		m_mesh.Rotate(glm::quat(m_qAll.getW(),m_qAll.getX(),m_qAll.getY(),m_qAll.getZ()));
	}
}

Mesh* GameObject::getMesh()
{
	return  &m_mesh;
}

PhysicModel GameObject::getModel()
{
	return *m_model;
}

void GameObject::ApplyCapsuleShape(float fRadius, float fHeight, float fMass, glm::quat quatInit, glm::vec3 posInit)
{
	m_model->ApplyCapsuleShape(fRadius,fHeight, fMass, quatInit, posInit);
	m_model->getRigidBody()->setUserPointer((void*)this);
}

void GameObject::ApplySphereShape(float fRadius, float fMass, glm::quat quatInit, glm::vec3 posInit)
{
	m_model->ApplySphereShape(fRadius, fMass, quatInit, posInit);
	m_model->getRigidBody()->setUserPointer((void*)this);
}

void GameObject::ApplyIndexedMeshShape()
{
	m_model->ApplyIndexedMeshShape(m_mesh.getNumTriangles(), m_mesh.getNumVertices(), m_mesh.getIndexArray(),m_mesh.getVertexArray());
	m_model->getRigidBody()->setUserPointer(this);
}

void GameObject::ApplyPhysicsManager(PhysicsManager* manager)
{
	m_myPhysicsManager = manager;
}

void GameObject::MoveForward()
{
	m_model->getRigidBody()->forceActivationState(DISABLE_DEACTIVATION );
	m_model->getRigidBody()->applyForce(btVector3(0,0,-10), btVector3(0,0,0));
	//m_model->getRigidBody()->translate(btVector3(0,0,10));
	btTransform trans;
	m_model->getRigidBody()->getMotionState()->getWorldTransform(trans);

}

void GameObject::Raycast(glm::vec3 vStart, glm::vec3 vEnd,  btCollisionWorld::ClosestRayResultCallback* RayCallback)
{
	btVector3 vBtStart = btVector3(vStart.x, vStart.y, vStart.z);
	btVector3 vBtEnd = btVector3(vEnd.x, vEnd.y, vEnd.z);

	RayCallback = new btCollisionWorld::ClosestRayResultCallback(vBtStart, vBtEnd);
	m_myPhysicsManager->m_dynamicsWorld->rayTest(
	vBtStart, vBtEnd,
	*RayCallback//RayCallback
	);
}

void GameObject::RaycastToGround()
{
	btVector3 lRayStart = getModel().getRigidBody()->getWorldTransform().getOrigin() ;
	btVector3 lRayEnd = getModel().getRigidBody()->getWorldTransform().getOrigin() + btVector3(0,-1000,0);
	m_RayCallback = new btCollisionWorld::ClosestRayResultCallback(lRayStart, lRayEnd);
	m_myPhysicsManager->m_dynamicsWorld->rayTest(
	lRayStart, lRayEnd,
	*m_RayCallback//RayCallback
	);
}

void GameObject::RayCastDirection()
{
	btVector3 lRayStart = getModel().getRigidBody()->getWorldTransform().getOrigin() + m_mesh.getBoundingBoxWidth()/2.0f*btVector3(m_mesh.getDirection().x, m_mesh.getDirection().y, m_mesh.getDirection().z) ;
	btVector3 lRayEnd = getModel().getRigidBody()->getWorldTransform().getOrigin() + (m_mesh.getBoundingBoxWidth()/2.0f + 0.5)*btVector3(m_mesh.getDirection().x, m_mesh.getDirection().y, m_mesh.getDirection().z) ;
	m_RayCallbackDirection1 = new btCollisionWorld::ClosestRayResultCallback(lRayStart, lRayEnd);
	m_myPhysicsManager->m_dynamicsWorld->rayTest(
	lRayStart, lRayEnd,
	*m_RayCallbackDirection1//RayCallback
	);
	lRayStart = getModel().getRigidBody()->getWorldTransform().getOrigin() - (m_mesh.getBoundingBoxWidth()+0.5)/2.0*btVector3(m_mesh.getLocalX().x, m_mesh.getLocalX().y, m_mesh.getLocalX().z)  ;
	 lRayEnd = getModel().getRigidBody()->getWorldTransform().getOrigin() -    (m_mesh.getBoundingBoxWidth()+0.5)/2.0*btVector3(m_mesh.getLocalX().x, m_mesh.getLocalX().y, m_mesh.getLocalX().z) +  (m_mesh.getBoundingBoxWidth()/2.0f + 0.5)*btVector3(m_mesh.getDirection().x, m_mesh.getDirection().y, m_mesh.getDirection().z) ;
	m_RayCallbackDirection2 = new btCollisionWorld::ClosestRayResultCallback(lRayStart, lRayEnd);
	m_myPhysicsManager->m_dynamicsWorld->rayTest(
	lRayStart, lRayEnd,
	*m_RayCallbackDirection2//RayCallback
	);
	 lRayStart = getModel().getRigidBody()->getWorldTransform().getOrigin() +   (m_mesh.getBoundingBoxWidth()+0.5)/2.0*btVector3(m_mesh.getLocalX().x, m_mesh.getLocalX().y, m_mesh.getLocalX().z) ;
	 lRayEnd = getModel().getRigidBody()->getWorldTransform().getOrigin() +   (m_mesh.getBoundingBoxWidth()+0.5)/2.0*btVector3(m_mesh.getLocalX().x, m_mesh.getLocalX().y, m_mesh.getLocalX().z) + (m_mesh.getBoundingBoxWidth()/2.0f + 0.5)*btVector3(m_mesh.getDirection().x, m_mesh.getDirection().y, m_mesh.getDirection().z) ;
	m_RayCallbackDirection3 = new btCollisionWorld::ClosestRayResultCallback(lRayStart, lRayEnd);
	m_myPhysicsManager->m_dynamicsWorld->rayTest(
	lRayStart, lRayEnd,
	*m_RayCallbackDirection3//RayCallback
	);
}

bool GameObject::isOnGround()
{
	if(m_RayCallback->hasHit()) 
	{
		if (getModel().getRigidBody()->getWorldTransform().getOrigin().getY() - m_RayCallback->m_hitPointWorld.y() > getMesh()->getBoundingBoxHeight()/2.0 + 0.1)	
				return false;
		else
			return true;
	}
	else
		return false;
}

void GameObject::StateMachine()
{
	btTransform newTrans;
	static bool restarted = false;
	getModel().getRigidBody()->getMotionState()->getWorldTransform(newTrans);
	
	switch(m_iState)
	{
		
		case FALLING:
			
			if (!restarted)
			{
				m_FallingTimer.restart();	
				restarted = true;
			}
			RaycastToGround();

			if (isOnGround())
			{
				m_mesh.StartAnimationTimer();
				m_iState = ON_GROUND;
				restarted = false;
			}
			else
			{
				getModel().getRigidBody()->getMotionState()->getWorldTransform(newTrans);
				newTrans.setOrigin(newTrans.getOrigin() + btVector3(0, -0.5*GRAVITY/2.0*m_FallingTimer.getElapsedTime().asMicroseconds()/(pow(10.0,6.0))/**m_FallingTimer.getElapsedTime().asMicroseconds()/(pow(10.0,6.0))*/, 0));			
			}

			break;

		case ON_GROUND:
			
			RaycastToGround();		
			newTrans.setOrigin(btVector3(newTrans.getOrigin().getX(), m_RayCallback->m_hitPointWorld.y() + getMesh()->getBoundingBoxHeight()/2.0f, newTrans.getOrigin().getZ()));
			m_mesh.StartAnimationTimer();
			m_iState = IDLE;
			break;

		case MOVING:
			if(!sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			{
				
				m_iState = IDLE;
				m_mesh.StartAnimationBlendingTimer();
				break;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{		
				btQuaternion quat2(btVector3(0,1,0), fAngleInc);
					
				newTrans.setRotation(quat2);
				m_fAngleAll += fAngleInc;
				m_qAll = btQuaternion(btVector3(0,1,0), m_fAngleAll);				
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{		
				btQuaternion quat2(btVector3(0,1,0), -fAngleInc);
				
				newTrans.setRotation(quat2);
				m_fAngleAll += -fAngleInc;
				m_qAll = btQuaternion(btVector3(0,1,0), m_fAngleAll);
			}

			RayCastDirection();
	
			RaycastToGround();	
			if (!isOnGround())
			{
				m_mesh.StartAnimationTimer();
				m_iState = FALLING;
				std::cout << "FALLING" << std::endl;
				break;
			}
			if((!m_RayCallbackDirection1->hasHit()) && (!m_RayCallbackDirection2->hasHit()) && (!m_RayCallbackDirection3->hasHit())) 
				newTrans.setOrigin(btVector3(newTrans.getOrigin().getX(), m_RayCallback->m_hitPointWorld.y()  + getMesh()->getBoundingBoxHeight()/2.0f, newTrans.getOrigin().getZ()) + btVector3(0.1*getMesh()->getDirection().x,0.1*getMesh()->getDirection().y,0.1*getMesh()->getDirection().z ));		
		//	m_mesh.Animate(0,1);
			break;

		case JUMPING:
			m_mesh.StartAnimationTimer();
			break;
		case START_WALKING:
			if (m_mesh.AnimationBlending(0,1))
				{
					m_mesh.StartAnimationTimer();
					m_iState = MOVING;
					break;
				}
			RaycastToGround();
			RayCastDirection();
			if((!m_RayCallbackDirection1->hasHit()) && (!m_RayCallbackDirection2->hasHit()) && (!m_RayCallbackDirection3->hasHit())) 
				newTrans.setOrigin(btVector3(newTrans.getOrigin().getX(), m_RayCallback->m_hitPointWorld.y()  + getMesh()->getBoundingBoxHeight()/2.0f, newTrans.getOrigin().getZ()) + btVector3(0.1*getMesh()->getDirection().x,0.1*getMesh()->getDirection().y,0.1*getMesh()->getDirection().z ));		
			break;
		case STOP_WALKING:
			if (m_mesh.AnimationBlending(1,0))
				{
					m_mesh.StartAnimationTimer();
					m_iState = IDLE;
					break;
				}
			RaycastToGround();
			RayCastDirection();
			if((!m_RayCallbackDirection1->hasHit()) && (!m_RayCallbackDirection2->hasHit()) && (!m_RayCallbackDirection3->hasHit()))  
				newTrans.setOrigin(btVector3(newTrans.getOrigin().getX(), m_RayCallback->m_hitPointWorld.y()  + getMesh()->getBoundingBoxHeight()/2.0f, newTrans.getOrigin().getZ()) + btVector3(0.1*getMesh()->getDirection().x,0.1*getMesh()->getDirection().y,0.1*getMesh()->getDirection().z ));		
			break;
		case IDLE:
			
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			{
				m_iState = MOVING;
			//	m_mesh.StartAnimationBlendingTimer();
				break;
			}
			RaycastToGround();
			RayCastDirection();
				if (!isOnGround())
				{
					m_mesh.StartAnimationTimer();
					m_FallingTimer.restart();
					m_iState = FALLING;
				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
				{		
					btQuaternion quat2(btVector3(0,1,0), fAngleInc);
					
					newTrans.setRotation(quat2);
					m_fAngleAll += fAngleInc;
					m_qAll = btQuaternion(btVector3(0,1,0), m_fAngleAll);
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
				{		
					btQuaternion quat2(btVector3(0,1,0), -fAngleInc);
					
					newTrans.setRotation(quat2);
					m_fAngleAll += -fAngleInc;
					m_qAll = btQuaternion(btVector3(0,1,0), m_fAngleAll);
				}
				else
				{
					btQuaternion quat2(btVector3(0,1,0), 0.0);
					newTrans.setRotation(quat2);
					
				}
				m_mesh.Animate(0,0);
				break;
	}

	getModel().getRigidBody()->getMotionState()->setWorldTransform(newTrans);
}

void GameObject::setState(int iState)
{
	m_iState = iState;
}

void GameObject::setName(std::string strName)
{
	m_strName = strName;
}

std::string GameObject::getName()
{
	return m_strName;
}

glm::vec3 GameObject::getPosition()
{
	btTransform trans;
	m_model->getRigidBody()->getMotionState()->getWorldTransform(trans);
	return glm::vec3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());
}

unsigned int GameObject::getNumAnimations()
{
	return m_mesh.getNumAnimations();
}