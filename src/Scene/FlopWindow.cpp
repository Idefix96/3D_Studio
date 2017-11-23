#include "FlopWindow.h"

void FlopWindow::Activate()
{
	m_bRunning = true;
}

void FlopWindow::AddButton(PushButton &button)
{
	button.DrawText(m_iWidth, m_iHeight);
	m_Buttons.push_back(&button);
	m_TextList.push_back(&(button.m_Text));
}

void FlopWindow::AddCamera(Camera* camera)
{
	m_Cameras.push_back(camera);
}

void FlopWindow::AddLight(Light* light)
{
	m_Lights.push_back(light);
}

void FlopWindow::AddMesh(Mesh* mesh)
{
	m_Meshes.push_back(mesh);
}

void FlopWindow::AddObject(GameObject& object)
{
	m_Meshes.push_back(object.getMesh());
	m_Objects.push_back(&object);
}

void FlopWindow::AddSkybox(Mesh& skybox)
{
	m_Skybox = &skybox;
}

void FlopWindow::AddText(sf::Text* text)
{
	m_TextList.push_back(text);
}

void FlopWindow::AddTextfield(Textfield* textfield)
{
	m_Textfields.push_back(textfield);
}

void FlopWindow::CheckRayCastClick(Camera* camera, PhysicsManager* physicsManager, int iMouseX, int iMouseY)
{
	if (m_bLMBreleased)
	{
		glm::vec4 lRayStart_NDC(
		((float)iMouseX/(float)m_iWidth  - 0.5f) * 2.0f, // [0,1024] -> [-1,1]
		((float)iMouseY/(float)m_iHeight - 0.5f) * -2.0f, // [0, 768] -> [-1,1]
		-1.0, // The near plane maps to Z=-1 in Normalized Device Coordinates
		1.0f
		);

		glm::vec4 lRayEnd_NDC(
		((float)iMouseX/(float)m_iWidth  - 0.5f) * 2.0f,
		((float)iMouseY/(float)m_iHeight - 0.5f) * -2.0f,
		0.0,
		1.0f
		);

		glm::mat4 InverseProjectionMatrix = glm::inverse(camera->getPerspectiveMatrix());
		glm::mat4 InverseViewMatrix = glm::inverse(camera->getWorldToCameraMatrix());

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
		physicsManager->m_dynamicsWorld->rayTest(
		btVector3(out_origin.x, out_origin.y, out_origin.z), 
		btVector3(out_end.x, out_end.y, out_end.z), 
		RayCallback
		);

		if(RayCallback.hasHit()) 
		{
			 m_Selected = (GameObject*)RayCallback.m_collisionObject->getUserPointer();
			 m_SelectText.setString("Selected: " + m_Selected->getName());
			// setStatus("Selected: " + m_Selected->getName());
		}
		else
		{
			m_Selected = NULL;
			// setStatus("Selected: None");
			 m_SelectText.setString("Selected: None");
		}

	}
}

void FlopWindow::CreateShadowmap()
{
	    glGenFramebuffers(1, &m_iFramebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, m_iFramebuffer);
		
		glGenTextures(1, &m_iDepthTexture);
		glBindTexture(GL_TEXTURE_2D, m_iDepthTexture);
		glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT24, 4096, 4096, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_iDepthTexture, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer error" << std::endl;

		glGenFramebuffers(1, &m_iFramebuffer_middle);
		glBindFramebuffer(GL_FRAMEBUFFER, m_iFramebuffer_middle);
		
		glGenTextures(1, &m_iDepthTexture_middle);
		glBindTexture(GL_TEXTURE_2D, m_iDepthTexture_middle);
		glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT24, 4096, 4096, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_iDepthTexture_middle, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer error" << std::endl;

		 glGenFramebuffers(1, &m_iFramebuffer_far);
		glBindFramebuffer(GL_FRAMEBUFFER, m_iFramebuffer_far);
		
		glGenTextures(1, &m_iDepthTexture_far);
		glBindTexture(GL_TEXTURE_2D, m_iDepthTexture_far);
		glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT24, 4096, 4096, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_iDepthTexture_far, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer error" << std::endl;
}

void FlopWindow::CreateNewWindow(int iWidth, int iHeight, std::string strTitle)
{
		m_iWidth = iWidth;
		m_iHeight = iHeight;
		m_strTitle = strTitle;
		m_Window.create(sf::VideoMode(iWidth, iHeight), m_strTitle, sf::Style::Titlebar | sf::Style::Close, sf::ContextSettings(32));
		m_bRunning = true;
		m_Selected = NULL;
		m_Skybox = NULL;
		//m_Light.InitDefault();
		m_bIsCreated = true;
		m_StatusFont.loadFromFile("../arial.ttf");
		m_StatusText.setFont(m_StatusFont);
		m_StatusText.setColor(sf::Color::Red);
		m_StatusText.setCharacterSize(24);
		sf::FloatRect textRect = m_StatusText.getLocalBounds();
		m_StatusText.setOrigin(textRect.left + textRect.width/2.0f,textRect.top  + textRect.height/2.0f);
		m_SelectText = m_StatusText;
		m_StatusText.setPosition(WIDTH - 500, 0.7*HEIGHT);
		m_SelectText.setPosition(0.85*WIDTH, 0.2*HEIGHT);
		m_SelectText.setString("Selected: None");
		//m_StatusText.setPosition(iWidth - textRect.width/2.0f - 10, 0.2*iHeight);
		m_TextList.push_back(&m_StatusText);
		m_TextList.push_back(&m_SelectText);

		glewInit();
		CreateShadowmap();
}

void FlopWindow::Draw()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_iFramebuffer);
	glClear(GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, 4096, 4096);
	if (m_Objects.size() > 0)
	{
		for (int index = 0; index < m_Objects.size(); index++)
		{
			//m_Light.lightPosition.x = m_Objects[index]->getModel().getRigidBody()->getWorldTransform().getOrigin().x() - m_Light.getLightDirection().x;
			//m_Light.lightPosition.z = m_Objects[index]->getModel().getRigidBody()->getWorldTransform().getOrigin().z() - m_Light.getLightDirection().z;
			
			GLuint old_Shader = m_Objects[index]->getMesh()->getShader();
			m_Objects[index]->getMesh()->setShader(m_iShadowmapShaderID);
			//m_Light.MapLightToShader(m_Objects[index]->getMesh()->getShader());
			glUseProgram(m_Objects[index]->getMesh()->getShader());
			m_Objects[index]->DrawMesh();
			m_Objects[index]->getMesh()->setShader(old_Shader);
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, NULL);

	glClearColor(m_ClearColor.x, m_ClearColor.y, m_ClearColor.z, m_ClearColor.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	glDisable(GL_CULL_FACE);
	glViewport(0, 0, 1920, 1017);
	if (m_Skybox != NULL)
	{
		glDepthFunc(GL_LEQUAL);
		glUseProgram(m_Skybox->getShader());
		m_Skybox->Render(0);
		glDepthFunc(GL_LESS);
	}	

	if (m_Objects.size() > 0)
	{
		for (int index = 0; index < m_Objects.size(); index++)
		{
			m_Objects[index]->getMesh()->setShadowmap(m_iDepthTexture);
			glUseProgram(m_Objects[index]->getMesh()->getShader());
			m_Objects[index]->DrawMesh();
		}
	}

	if (m_Buttons.size() > 0)
		for (int index = 0; index < m_Buttons.size(); index++)
		{
			glUseProgram(m_Buttons[index]->getShader());
			m_Buttons[index]->Render(0);
		}

	 //unbind opengl stuff
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray( 0 );
	glUseProgram( 0 );
	glBindTexture(GL_TEXTURE_2D,NULL);
	glActiveTexture(GL_TEXTURE0);
	//glBindFramebuffer(GL_FRAMEBUFFER, NULL);
	//prepare window for drawing sfml stuff
	m_Window.pushGLStates();
	
	//draw sfml stuff
	if (m_TextList.size() > 0)
		for (unsigned int i = 0; i < m_TextList.size(); i++)
		{
			m_Window.draw(*m_TextList[i]);	
		}

	if (m_Textfields.size() > 0)
		for (unsigned int i = 0; i < m_Textfields.size(); i++)
		{
			m_Window.draw(*(m_Textfields[i]->getField()));
			m_Window.draw(*(m_Textfields[i]->getCaption()));
			m_Window.draw(*(m_Textfields[i]->getText()));
			if (m_Textfields[i]->hasFocus())
			{
				m_Textfields[i]->Update();
				m_Window.draw(m_Textfields[i]->getCursor(), 2, sf::Lines);
			}
		}
			
    // end the current frame (internally swaps the front and back buffers)
    m_Window.display();
	//restore GL states
	m_Window.popGLStates();
}

void FlopWindow::EndMainLoop()
{
	m_bRunning = false;
}

GameObject* FlopWindow::getSelectedObject()
{
	return m_Selected;
}

sf::Text* FlopWindow::getStatusText()
{
	return &m_StatusText; 
}

bool FlopWindow::isCreated()
{
	return m_bIsCreated;
}

void FlopWindow::MapLightToShader(Light light, GLuint iShader)
{

}

void FlopWindow::Run()
{
	if (m_bRunning)
    {
		m_Window.setActive(true);
		m_Window.clear();

		sf::Event event;
	   	float fMouseX = ((float)sf::Mouse::getPosition(m_Window).x/(float)m_iWidth  - 0.5f) * 2.0f;
		float fMouseY = ((float)sf::Mouse::getPosition(m_Window).y/(float)m_iHeight  - 0.5f) * -2.0f;
		std::string temp;
		m_bLMBreleased = false;

       while (m_Window.pollEvent(event))
	   {
		   switch(event.type)
		   {
			   case sf::Event::Closed:
					 m_bRunning = false;	
					 m_Window.close();
					 break;

			   case sf::Event::Resized:
				    // adjust the viewport when the window is resized
					glViewport(0, 0, event.size.width, event.size.height);
					m_Window.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
					m_iWidth = event.size.width;
					m_iHeight = event.size.height;
					for (unsigned int index = 0; index < m_Buttons.size(); index++)
					{
						m_Buttons[index]->DrawText(m_iWidth, m_iHeight);
					}
					break;
				
			   case sf::Event::TextEntered:
				   for (unsigned int i = 0; i < m_Textfields.size(); i++)
					   if(m_Textfields[i]->hasFocus())
						{
							if (event.text.unicode < 128)
							{
								switch(event.text.unicode)
								{
								case 8: //backspace
									m_Textfields[i]->DeleteLast();
									break;
								case 13: //enter
									break;
								default:							
									m_Textfields[i]->fetchInput(event.text.unicode);
									break;
								}
							}
						}
					break;
			
			   case  sf::Event::MouseButtonPressed:
				    break;

			   case sf::Event::MouseButtonReleased:
				   if (event.mouseButton.button == sf::Mouse::Left)
						m_bLMBreleased = true;
				    for (int i = 0; i < m_Buttons.size(); i++)
					{
						if (m_Buttons[i]->MouseOverCallback(fMouseX, fMouseY))
						{
							if (event.mouseButton.button == sf::Mouse::Left)
							{
								m_Buttons[i]->onClick();
								
							}
						}			
					}
					for (unsigned int i = 0; i < m_Textfields.size(); i++)
					if(m_Textfields[i]->MouseOverCallback((float)sf::Mouse::getPosition(m_Window).x, (float)sf::Mouse::getPosition(m_Window).y))
					   {
						   m_Textfields[i]->setFocus(true);
					   }
					   else
					   {
						   m_Textfields[i]->setFocus(false);
					   }

				    break;

			   case sf::Event::MouseMoved:
				    fMouseX = ((float)sf::Mouse::getPosition(m_Window).x/(float)m_iWidth  - 0.5f) * 2.0f;
					fMouseY = ((float)sf::Mouse::getPosition(m_Window).y/(float)m_iHeight  - 0.5f) * -2.0f;
					for (int i = 0; i < m_Buttons.size(); i++)
					{
						m_Buttons[i]->MouseOverCallback(fMouseX, fMouseY);
					}
					
				   break;

			   default:
				   break;
		   }
        }

	    Draw();
	   
	}
}

void FlopWindow::setShadowmapShaderID(GLuint iId)
{
	m_iShadowmapShaderID = iId;
}

void FlopWindow::setStatus(std::string strStatus)
{
	m_StatusText.setString(strStatus);
	//m_StatusText.setPosition(m_iWidth - m_StatusText.getLocalBounds().width - 10, 0.2*m_iHeight);
}

void FlopWindow::SetClearColor(glm::vec4 clearColor)
{
	m_ClearColor = clearColor;
}