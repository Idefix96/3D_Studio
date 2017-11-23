#include "SceneManager.h"

SceneManager* SceneManager::m_thisptr = NULL;

void SceneManager::DrawScene()
{
	// clear the buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_CULL_FACE);
	for (int i=0; i<  m_thisptr->m_CShaders.size(); i++)
	{
		m_thisptr->m_CMainCamera->MapCameraToShader(m_thisptr->m_CShaders[i].getShaderID());
		m_thisptr->m_CLights[0].MapLightToShader(m_thisptr->m_CShaders[i].getShaderID(), glm::vec3(0), glm::vec3(0));
	}

	if (m_thisptr->m_CMeshes.size() > 0)
		for (int index = 0; index < m_thisptr->m_CMeshes.size(); index++)
			m_thisptr->m_CMeshes[index]->Render(m_thisptr->m_CActiveShader->getShaderID());

	if (m_thisptr->m_Skybox != NULL)
	{
		glDepthFunc(GL_LEQUAL);
		m_thisptr->m_Skybox->Render(m_thisptr->m_CActiveShader->getShaderID());
		glDepthFunc(GL_LESS);
	}	

	if (m_thisptr->m_Objects.size() > 0)
	{
		for (int index = 0; index < m_thisptr->m_Objects.size(); index++)
			m_thisptr->m_Objects[index]->DrawMesh();
	}


}

void SceneManager::Reshape(int width, int height)
{
		glViewport(0,0,WIDTH,HEIGHT);
}

void SceneManager::LoadScene()
{	
	
	glewInit();
	std::cout << "Starting program..." << std::endl;

	m_iScreenWidth = WIDTH;
	m_iScreenHeight = HEIGHT;
		
	 //setup shader
	std::cout << "Setting up shaders..." << std::endl;

	m_CActiveShader = new Shader();
	m_CActiveShader->Init();
	std::cout << "Loading default 3D vertex shader..." << std::endl;
	m_CActiveShader->SetupVertexShader(DEFAULT_VERTEX_SHADER);
	std::cout << "Loading default 3D fragment shader..." << std::endl;
	m_CActiveShader->SetupFragmentShader(DEFAULT_FRAGMENT_SHADER); 
	m_CShaders.push_back(*m_CActiveShader);

	////enable depth test
    glEnable(GL_DEPTH_TEST);
	
	//enable culling
	//glEnable(GL_CULL_FACE);

	Light defaultLight;
	defaultLight.InitDefault();
	m_CLights.push_back(defaultLight);

	m_CMainCamera = new Camera(45.0f, 1920.0f, 1080.0f, 0.1f, 10000.0f);
	
	m_thisptr = this;

//	m_CMainCamera->setPosition(glm::vec3(0,0,0));

	m_Keyboard.Initialize();

	m_Entity = NULL;
}

//follow mouse
void  SceneManager::CameraControl(int x, int y)
{
	m_thisptr->m_CMainCamera->ControlWithMouse(x, y);
}

void SceneManager::LoadObject()
{
	sf::Context context;
	GameObject *gObj = new GameObject();
	//m_thisptr->m_Windows[0]->setStatus("Loading New Object... ");
	gObj->GenerateObject(m_thisptr->m_strLoadFile);
	if (gObj->getMesh()->isValid())
	{	
		//m_thisptr->m_Windows[0]->setStatus("Loading New Object... Applying Shader...");
		gObj->getMesh()->setShader(m_thisptr->getShaderID(0));
		//m_thisptr->m_Windows[0]->setStatus("Loading New Object... Applying Physics...");
		float fDiameter;
		switch(m_thisptr->m_iLoadShapeModel)
		{
			case 0:
				if (gObj->getMesh()->getBoundingBoxWidth() > gObj->getMesh()->getBoundingBoxDepth())
					fDiameter = gObj->getMesh()->getBoundingBoxWidth();
				else
					fDiameter = gObj->getMesh()->getBoundingBoxDepth();

				gObj->ApplyCapsuleShape(fDiameter/2.0, gObj->getMesh()->getBoundingBoxHeight() - fDiameter, 120, glm::quat(0,0,0,1),gObj->getMesh()->getBoundingCenter());
				break;
			case 1:
				gObj->ApplyIndexedMeshShape();
				m_thisptr->setGround(gObj);
				break;
			case 2:
				gObj->ApplySphereShape(gObj->getMesh()->getBoundingBoxHeight()/2.0,20,glm::quat(0,0,0,1),glm::vec3(5,1.5,0));
				break;
		}
//		m_thisptr->m_Windows[0]->setStatus("Loading New Object... Adding To Scene...");
		m_thisptr->AddObject(*gObj, m_thisptr->m_iLoadWindowIndex);
	//	m_thisptr->m_Windows[0]->setStatus("Loading New Object... Adding To Physical World...");

		//sometimes addphysics throws exception due to parallel thread issues
		try
		{
			gObj->ApplyPhysicsManager(m_thisptr->m_PhyMan);
			m_thisptr->m_PhyMan->AddPhysicModel(&gObj->getModel());
		}
		catch(exception e)
		{
//			m_thisptr->m_Windows[0]->setStatus("Loading New Object... Random Access Violation, this shit happens, try again!");
		}

//		m_thisptr->m_Windows[0]->setStatus("Loading New Object... Done!");
	}
	//else
//		m_thisptr->m_Windows[0]->setStatus("Loading New Object... Error!");
}

void SceneManager::LoadFromString(std::string strFileName, int iWindowIndex, int iShapeModel)
{
	m_strLoadFile = strFileName;
	m_iLoadWindowIndex = iWindowIndex;
	m_iLoadShapeModel = iShapeModel;
	
	m_LoadingThread.launch();
}

void SceneManager::UpdateScene()
{
	//Update the Physics
	if (m_thisptr->m_PhyMan != NULL)
		m_thisptr->m_PhyMan->Update();

}

void SceneManager::AddMeshToScene(std::string MeshFile)
{
	Mesh CObject;
	CObject.LoadAssimpFile(MeshFile);
	CObject.setShader(getShaderID(0));
	m_CMeshes.push_back(&CObject);
}

void SceneManager::AddSkyboxToScene(std::string MeshFile)
{
	m_Skybox = new Mesh();
	m_Skybox->LoadAssimpFile(MeshFile);
	m_Skybox->setShader(getShaderID(2));
}

void SceneManager::AddMeshToScene(Mesh* mesh)
{
	m_CMeshes.push_back(mesh);
}

void SceneManager::AddObjectToScene(GameObject& object)
{
	m_CMeshes.push_back(object.getMesh());

	m_Objects.push_back(&object);
}

void SceneManager::AddObject(GameObject& object, int iWindowID)
{
	
}



void SceneManager::AddText(sf::Text text)
{
	m_TextList.push_back(&text);
}


void SceneManager::AddShader(std::string VertexShaderFile, std::string FragmentShaderFile)
{
	Shader *newShader = new Shader();
	newShader->Init();
	newShader->SetupVertexShader(VertexShaderFile);
	newShader->SetupFragmentShader(FragmentShaderFile); 
	m_CShaders.push_back(*newShader);
}

void SceneManager::SetShaderForMesh(int iIndex, int iShaderID)
{
	m_CMeshes[iIndex]->setShader(iShaderID);
}

sf::Clock UpdateTimer;
sf::Time oldTime = sf::microseconds(0);

void SceneManager::RunScene()
{
	//position of mouse in window 
	sf::Vector2i localPosition;

	sf::Font font;
	font.loadFromFile("../arial.ttf");

	sf::Text input;

	input.setCharacterSize(24); // in pixels, not points!
	input.setFillColor(sf::Color::Red);
	input.setStyle(sf::Text::Bold );
	//input.setPosition(10,10);
	input.setFont(font);
	m_textFPS.setString("");
	m_textFPS.setFont(font); 
	m_textFPS.setCharacterSize(24); // in pixels, not points!
	m_textFPS.setFillColor(sf::Color::White);
	m_textFPS.setStyle(sf::Text::Bold );
	m_textFPS.setPosition(10,10);
	if (m_thisptr->m_Skybox != NULL)
	{
		//AddSkyboxToScene(*m_Skybox);
	}
	
	m_bRunning = true;
	
	/***************************************Main Application Loop*******************************************************/
	while (m_bRunning)
    {
		
		ActivateControls();

		//calculate and display fps
		int elapsedTime = UpdateTimer.getElapsedTime().asMicroseconds() - oldTime.asMicroseconds();
		oldTime =  UpdateTimer.getElapsedTime();
		m_ldFps = CalcFPS(elapsedTime); 

		
	
				for (int j=0; j<  m_thisptr->m_CShaders.size(); j++)
				{
					m_thisptr->m_CMainCamera->MapCameraToShader(m_thisptr->m_CShaders[j].getShaderID());
					m_thisptr->m_CLights[0].MapLightToShader(m_thisptr->m_CShaders[j].getShaderID(),m_CMainCamera->getPosition(), m_CMainCamera->getLookDir() );
				}
			
			

		bool bClickDetect = true;
	
		
	   //camera control with mouse and raycast into scene
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			if (m_bInput)
			{
				m_CMainCamera->TriggerRotationWithMouse();	
			}
		}
		m_CMainCamera->ControlWithMouse(localPosition.x, localPosition.y);
		
		
	   //camera control with keys	
 	   if (m_thisptr->m_bInput)
	   {
			
			if ( m_Keyboard.MoveForwardKeyPressed())
			{	
				m_thisptr->m_CMainCamera->MoveForward(0.6f);
			}

			if (m_Keyboard.MoveBackwardKeyPressed())
			{
				m_thisptr->m_CMainCamera->MoveForward(-0.6f);
			}

			if (m_Keyboard.MoveUpKeyPressed())
				m_thisptr->m_CMainCamera->MoveUp(0.6f);

		
			
				btVector3 velocity(0,0,0);
				float fAnimationTimeScalingFactor = 0.1;

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
				{	


					velocity += btVector3(0,0,-25);		
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
				{				

			

					velocity += btVector3(0,0,-0.5);					
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
				{	
					velocity += btVector3(0,0,0.5);					
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
				{	
					velocity += btVector3(0,0,25);						
				}

				if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
					m_Entity = NULL;
				
				if (m_Entity != NULL)
				{
					
					//m_thisptr->m_Entity->StateMachine();
					m_thisptr->CameraFollow(*m_Entity);
				}
			}	
		static sf::Clock FPSupdateTimer;
		//draw texts and sfml stuff
		if (FPSupdateTimer.getElapsedTime().asSeconds() > 1.0)
		{
			m_textFPS.setString(std::to_string(m_ldFps) + " FPS");
			FPSupdateTimer.restart();
		}
		//m_textFPS.setPosition(10,10);
		UpdateScene();
	 }	
	/***************************************Main Application Loop*******************************************************/
}



void  SceneManager::ActivateControls()
{
	m_bInput = true;
}

void  SceneManager::DeactivateControls()
{
	m_bInput = false;
	std::cout << m_bInput << std::endl;
}

void SceneManager::DeleteAllObjects()
{
	if (m_thisptr->m_CMeshes.size() > 0)
		for (int i = 0; i < m_thisptr->m_CMeshes.size(); i++)
			m_thisptr->m_CMeshes.pop_back();
}

void SceneManager::DeleteLastMesh()
{
	m_CMeshes.pop_back();
}

void SceneManager::LinkWithPhysicsManager(PhysicsManager* PhyManager)
{
	m_PhyMan = PhyManager;
}

void SceneManager::CameraFollow(GameObject& object)
{
	m_CMainCamera->setCenter(object.getPosition() + glm::vec3(4.0)*object.getMesh()->getDirection());
	m_CMainCamera->setPosition(object.getPosition() - glm::vec3(8.0)*glm::normalize(object.getMesh()->getDirection()) + glm::vec3(0,object.getMesh()->getBoundingBoxHeight()/4.0 + 1.0,0));
}

void SceneManager::ObjectClicked(int iMouseX, int iMouseY)
{
	glm::vec4 lRayStart_NDC(
	((float)iMouseX/(float)m_iScreenWidth  - 0.5f) * 2.0f, // [0,1024] -> [-1,1]
	((float)iMouseY/(float)m_iScreenHeight - 0.5f) * -2.0f, // [0, 768] -> [-1,1]
	-1.0, // The near plane maps to Z=-1 in Normalized Device Coordinates
	1.0f
	);

	glm::vec4 lRayEnd_NDC(
	((float)iMouseX/(float)m_iScreenWidth  - 0.5f) * 2.0f,
	((float)iMouseY/(float)m_iScreenHeight - 0.5f) * -2.0f,
	0.0,
	1.0f
	);

	glm::mat4 InverseProjectionMatrix = glm::inverse(m_CMainCamera->getPerspectiveMatrix());
	glm::mat4 InverseViewMatrix = glm::inverse(m_CMainCamera->getWorldToCameraMatrix());

	glm::vec4 lRayStart_camera = InverseProjectionMatrix * lRayStart_NDC;    
	lRayStart_camera/=lRayStart_camera.w;
	glm::vec4 lRayStart_world  = InverseViewMatrix       * lRayStart_camera; 
	lRayStart_world /=lRayStart_world .w;
	
	glm::vec4 lRayEnd_camera   = InverseProjectionMatrix * lRayEnd_NDC;      
	lRayEnd_camera  /=lRayEnd_camera  .w;
	glm::vec4 lRayEnd_world    = InverseViewMatrix       * lRayEnd_camera;   
	lRayEnd_world   /=lRayEnd_world   .w;

	glm::vec3 lRayDir_world(lRayEnd_world - lRayStart_world);
	lRayDir_world = glm::normalize(lRayDir_world);

	glm::vec3 out_origin = glm::vec3(lRayEnd_world.x, lRayEnd_world.y, lRayEnd_world.z);
	glm::vec3 out_end = out_origin + lRayDir_world*1000.0f;

	btCollisionWorld::ClosestRayResultCallback RayCallback( btVector3(out_origin.x, out_origin.y, out_origin.z), btVector3(out_end.x, out_end.y, out_end.z));
	m_PhyMan->m_dynamicsWorld->rayTest(
	btVector3(out_origin.x, out_origin.y, out_origin.z), 
	btVector3(out_end.x, out_end.y, out_end.z), 
	RayCallback
	);

	
		if(RayCallback.hasHit()) 
		{
			m_Selected = (GameObject*)RayCallback.m_collisionObject->getUserPointer();
			std::cout << "Auswahl: " << m_Selected->getName() << std::endl;
		}
}

long double SceneManager::CalcFPS(int iElapsedTime)
{
	long double fps = 1000000.f / long double(iElapsedTime);
	return fps;
}

void SceneManager::EndMainLoop()
{
	m_thisptr->m_bRunning = false;
}
GLuint SceneManager::getShaderID(int iIndex)
{
	return m_CShaders[iIndex].getShaderID();
}

void SceneManager::SetPlayable(GameObject& object)
{
	m_Entity = &object;
}

void SceneManager::setGround(GameObject* object)
{
	m_Ground = object;
}