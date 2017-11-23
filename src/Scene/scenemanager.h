#ifndef SCENE_MANAGER_H_
#define SCENE_MANAGER_H_

#include "main_header.h"
#include "Camera/Camera.h"
#include "Graphics/Light.h"
#include "Graphics/Mesh.h"
#include "Graphics/Shader.h"
#include "Keyboard\Keyboard.h"
#include "Physics/PhysicsManager.h"
#include "Physics/GameObject.h"
#include "Graphics/ParticleSystem.h"


#define DEFAULT_VERTEX_SHADER	"VS.vs"
#define DEFAULT_FRAGMENT_SHADER	"FS.fs"
#define DEFAULT_2D_VERTEX_SHADER "VS_2D.vs"
#define DEFAULT_2D_FRAGMENT_SHADER "FS_2D.fs"
#define DEFAULT_SKYBOX_VERTEX_SHADER "VS_Skybox.vs"
#define DEFAULT_SKYBOX_FRAGMENT_SHADER "FS_Skybox.fs"
#define DEFAULT_SHADOWMAP_VERTEX_SHADER "VS_Shadow.vs"
#define DEFAULT_SHADOWMAP_FRAGMENT_SHADER "FS_Shadow.fs"

using namespace std;

class SceneManager
{
private:
	Camera					*m_CMainCamera;
	vector<Light>			m_CLights;
	vector<Mesh*>			m_CMeshes;
	vector<GameObject*>		m_Objects;
	GameObject*				m_Entity;
	GameObject*				m_Ground;

	vector<sf::Text*>		m_TextList;

	vector<Camera>			m_CCameras;
	Shader					*m_CActiveShader;
	vector<Shader>			m_CShaders;
	static SceneManager*	m_thisptr;
	Mesh					*m_Skybox;
	bool					m_bInput;
	bool					m_bRunning;
	long double				m_ldFps;
	sf::RenderWindow		m_window;
	PhysicsManager*			m_PhyMan;
	KeyboardInputHandler	m_Keyboard;

	int m_iScreenWidth;
	int m_iScreenHeight;

	std::string				m_strLoadFile;
	int						m_iLoadWindowIndex;
	int						m_iLoadShapeModel;
	sf::Thread				m_LoadingThread;

	GameObject*				m_Selected;
	sf::Font				m_Font;
	sf::Text				m_textFPS;


public:
	SceneManager() : m_LoadingThread(&SceneManager::LoadObject){m_bInput = false;
	m_Skybox = NULL;
	m_PhyMan = NULL;
	m_Selected = NULL;}
	void LoadScene();
	void LoadFromString(std::string strFileName, int iWindowIndex, int iShapeModel);
	static void LoadObject();
	void AddMeshToScene(std::string MeshFile);
	void AddSkyboxToScene(std::string MeshFile);
	void AddMeshToScene(Mesh* mesh);
	void AddObjectToScene(GameObject& object);
	void AddObject(GameObject& object, int iWindowID);
	void HandleChildWindows();
	void AddText(sf::Text text);
	void AddChildWindow(sf::RenderWindow* window);
	void AddShader(std::string VertexShaderFile, std::string FragmentShaderFile);
	static void Reshape(int width, int height);
	  void DrawScene();
	static void UpdateScene();
	static void CameraControl(int x, int y);
	static void MouseInput(int button, int state, int x, int y);
	static void DeleteAllObjects();
	void ManipulateObject(int iObjectNr);
	void ShowInput();
	void RunScene();
	void ActivateControls();
	void DeactivateControls();
	void SetShaderForMesh(int iMeshIndex, int iShaderID);
	GLuint getShaderID(int iIndex);
	void LinkWithPhysicsManager(PhysicsManager* PhyManager);
//	void LinkWithGameManager(GameManager* GameManager);
	void CameraFollow(GameObject& object);
	void ObjectClicked(int iMouseX, int iMouseY); 
	static void EndMainLoop();
	long double CalcFPS(int iElapsedTime);
	void DeleteLastMesh();
	void DeleteLastButton();
	void SetPlayable(GameObject& object);
	void setGround(GameObject* object);
};

#endif