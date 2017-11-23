#include "main_header.h"
#include "PushButton.h"
#include "TextField.h"
#include "Mesh.h"
#include "GameObject.h"
#include "Camera.h"
#include "Light.h"
#include "PhysicsManager.h"

#ifndef FLOPWINDOW_H_
#define FLOPWINDOW_H_

class FlopWindow
{
	friend class SceneManager;

protected:
	

	std::vector<Camera*>		m_Cameras;
	std::vector<GameObject*>	m_Objects;
	std::vector<Light*>			m_Lights;
	std::vector<Mesh*>			m_Meshes;
	std::vector<PushButton*>	m_Buttons;	
	std::vector<sf::Text*>		m_TextList;
	std::vector<Textfield*>		m_Textfields;

	glm::vec4					m_ClearColor;

	GameObject*					m_Selected;
	//Light						m_Light;
	Mesh*						m_Skybox;

	bool						m_bInput;
	bool						m_bIsCreated;
	bool						m_bLMBreleased;
	bool						m_bRunning;

	int							m_iWidth;
	int							m_iHeight;

	std::string					m_strTitle;

	sf::Text					m_StatusText;
	sf::Font					m_StatusFont;
	sf::Text					m_SelectText;
	sf::Font					m_SelectFont;
	
	GLuint						m_iDepthTexture;
	GLuint						m_iDepthTexture_middle;
	GLuint						m_iDepthTexture_far;
	GLuint						m_iFramebuffer;
	GLuint						m_iFramebuffer_middle;
	GLuint						m_iFramebuffer_far;
	GLuint						m_iShadowmapShaderID;

public:
	sf::RenderWindow			m_Window;
	
	void Activate();
	void AddButton(PushButton& button);
	void AddCamera(Camera* camera);
	void AddLight(Light* light);
	void AddMesh(Mesh* mesh);
	void AddObject(GameObject& object);
	void AddSkybox(Mesh& skybox);
	void AddText(sf::Text* text);
	void AddTextfield(Textfield* textfield);
	void CheckRayCastClick(Camera* camera, PhysicsManager* physManager, int iMouseX, int iMouseY);
	void CreateShadowmap();
	void CreateNewWindow(int iWidth, int iHeight, std::string strTitle);
	virtual void Draw();
	void EndMainLoop();
	GameObject* getSelectedObject();
	sf::Text*	getStatusText();
	bool isCreated();
	void MapLightToShader(Light light, GLuint iShader);
	virtual void Run();
	void SetClearColor(glm::vec4 clearColor);
	void setShadowmapShaderID(GLuint iId);
	void setStatus(std::string strStatus);
};

#endif