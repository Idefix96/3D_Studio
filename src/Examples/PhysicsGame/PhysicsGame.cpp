#include "GL\glew.h"

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>

#include "Keyboard\Keyboard.h"
#include "Graphics\Mesh.h"
#include "Graphics\Light.h"
#include "Camera\Camera.h"
#include "Physics\GameObject.h"
#include "Physics\PhysicsManager.h"
#include "Physics\physicmodel.h"

PhysicsManager worldPhysics;

GameObject		terrain;
GameObject		greenCorner;
GameObject		blueCorner;
GameObject		yellowCorner;
GameObject		redCorner;
GameObject		kugel;
GameObject		blueBall;
bool			doublePoints2 = false;
GameObject		greenBall;
bool			doublePoints3 = false;
GameObject		redBall;
bool			doublePoints4 = false;
GameObject		yellowBall;
bool			doublePoints5 = false;

sf::Font font;


int main()
{
	sf::ContextSettings Settings;
	Settings.depthBits = 24;
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "My window", sf::Style::Default, Settings);
	glewInit();

	worldPhysics.Initialize();

	kugel.GenerateObject("kugel.fbx");
	kugel.ApplySphereShape(kugel.getMesh()->getBoundingBoxHeight() / 2.f, 10, glm::quat(0, 0, 0, 1), glm::vec3(0, kugel.getMesh()->getBoundingBoxHeight() / 2.f , 0));
	kugel.getModel().getRigidBody()->setLinearVelocity(btVector3(0, 0, 0));
	kugel.ApplyPhysicsManager(&worldPhysics);
	worldPhysics.AddPhysicModel(&kugel.getModel());
	
	blueBall.GenerateObject("kugel.fbx");
	blueBall.getMesh()->setMaterialColor(0, glm::vec3(0,0,1.0f));
	blueBall.ApplySphereShape(blueBall.getMesh()->getBoundingBoxHeight() / 2.f, 10, glm::quat(0, 0, 0, 1), glm::vec3(10, blueBall.getMesh()->getBoundingBoxHeight() / 2.f, 10));
	blueBall.ApplyPhysicsManager(&worldPhysics);
	worldPhysics.AddPhysicModel(&blueBall.getModel());

	greenBall.GenerateObject("kugel.fbx");
	greenBall.getMesh()->setMaterialColor(0, glm::vec3(0, 1.f,0));
	greenBall.ApplySphereShape(greenBall.getMesh()->getBoundingBoxHeight() / 2.f, 10, glm::quat(0, 0, 0, 1), glm::vec3(10, greenBall.getMesh()->getBoundingBoxHeight() / 2.f, -10));
	greenBall.ApplyPhysicsManager(&worldPhysics);
	worldPhysics.AddPhysicModel(&greenBall.getModel());

	redBall.GenerateObject("kugel.fbx");
	redBall.getMesh()->setMaterialColor(0, glm::vec3(1.f, 0, 0));
	redBall.ApplySphereShape(redBall.getMesh()->getBoundingBoxHeight() / 2.f, 10, glm::quat(0, 0, 0, 1), glm::vec3(-10, redBall.getMesh()->getBoundingBoxHeight() / 2.f, -10));
	redBall.ApplyPhysicsManager(&worldPhysics);
	worldPhysics.AddPhysicModel(&redBall.getModel());

	yellowBall.GenerateObject("kugel.fbx");
	yellowBall.getMesh()->setMaterialColor(0, glm::vec3(1.f, 1.f, 0));
	yellowBall.ApplySphereShape(yellowBall.getMesh()->getBoundingBoxHeight() / 2.f, 10, glm::quat(0, 0, 0, 1), glm::vec3(-10, yellowBall.getMesh()->getBoundingBoxHeight() / 2.f, 10));
	yellowBall.ApplyPhysicsManager(&worldPhysics);
	worldPhysics.AddPhysicModel(&yellowBall.getModel());

	terrain.GenerateObject("field.fbx");
	terrain.ApplyIndexedMeshShape();
	terrain.ApplyPhysicsManager(&worldPhysics);
	worldPhysics.AddPhysicModel(&terrain.getModel());
	greenCorner.GenerateObject("greenCorner.fbx");
	greenCorner.ApplyIndexedMeshShape();
	greenCorner.ApplyPhysicsManager(&worldPhysics);
	worldPhysics.AddPhysicModel(&greenCorner.getModel());
	
	blueCorner.GenerateObject("blueCorner.fbx");
	blueCorner.ApplyIndexedMeshShape();
	blueCorner.ApplyPhysicsManager(&worldPhysics);
	worldPhysics.AddPhysicModel(&blueCorner.getModel());
	yellowCorner.GenerateObject("yellowCorner.fbx");
	yellowCorner.ApplyIndexedMeshShape();
	yellowCorner.ApplyPhysicsManager(&worldPhysics);
	worldPhysics.AddPhysicModel(&yellowCorner.getModel());
	redCorner.GenerateObject("redCorner.fbx");
	redCorner.ApplyIndexedMeshShape();
	redCorner.ApplyPhysicsManager(&worldPhysics);
	worldPhysics.AddPhysicModel(&redCorner.getModel());
	
	Shader shader;
	shader.Init();
	std::cout << "Loading default 3D vertex shader..." << std::endl;
	shader.SetupVertexShader("VS.vs");
	std::cout << "Loading default 3D fragment shader..." << std::endl;
	shader.SetupFragmentShader("FS.fs");

	Light light;
	light.InitDefault();
	light.setDirection(glm::vec3(-1.f, -1.f, 0));

	Camera camera(3.14159f / 4.0f, 1920.0f, 1080.0f, 0.1f, 10000.0f);

	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	font.loadFromFile("data/Fonts/arial.ttf");
	sf::Clock gameTimer;
	sf::Text time;
	sf::Text points;
	sf::Text result;
	points.setFont(font);
	time.setFont(font);
	result.setFont(font);
	result.setString("");
	result.setPosition(sf::Vector2f(1500, 600));
	time.setPosition(sf::Vector2f(500,20));
	points.setPosition(sf::Vector2f(1000, 20));
	int iPoints = 0;
	bool bDoublePointsBlue = false;
	bool bDoublePointsYellow = false;
	bool bDoublePointsRed = false;
	bool bDoublePointsGreen = false;
	bool bBlueInGame = true;
	bool bRedInGame = true;
	bool bYellowInGame = true;
	bool bGreenInGame = true;
	bool bHasResult = false;

	// run the program as long as the window is open
	while (window.isOpen())
	{
		//get the time in Format min:sec:millis
		int minutes = (int)gameTimer.getElapsedTime().asSeconds() / 60;
		int seconds = (int)gameTimer.getElapsedTime().asSeconds() % 60;
		int millis =(int)(gameTimer.getElapsedTime().asMilliseconds()) - (1000*seconds) - (60000*minutes);

		if ((!bYellowInGame) && (!bBlueInGame) && (!bGreenInGame) && (!bRedInGame) && (!bHasResult))
		{
			bHasResult = true;
			result.setString("Your Result: " + std::to_string(iPoints) + "\n" + "Your Time: " + time.getString());
		}

		//check if the ball is in the right corner
		redBall.RaycastToGround();
		if (redBall.getRaycast()->hasHit())
		{
			GameObject*				m_Selected;
			m_Selected = (GameObject*)(redBall.getRaycast()->m_collisionObject->getUserPointer());
			if (m_Selected->getName().compare("redCorner") == 0)
				bDoublePointsRed = true;
			else
				bDoublePointsRed = false;
		}
		//check if the ball is in the right corner
		yellowBall.RaycastToGround();
		if (yellowBall.getRaycast()->hasHit())
		{
			GameObject*				m_Selected;
			m_Selected = (GameObject*)(yellowBall.getRaycast()->m_collisionObject->getUserPointer());
			if (m_Selected->getName().compare("yellowCorner") == 0)
				bDoublePointsYellow = true;
			else
				bDoublePointsYellow = false;
		}
		//check if the ball is in the right corner
		greenBall.RaycastToGround();
		if (greenBall.getRaycast()->hasHit())
		{
			GameObject*				m_Selected;
			m_Selected = (GameObject*)(greenBall.getRaycast()->m_collisionObject->getUserPointer());
			if (m_Selected->getName().compare("greenCorner") == 0)
				bDoublePointsGreen = true;
			else
				bDoublePointsGreen = false;
		}
		//check if the ball is in the right corner
		blueBall.RaycastToGround();
		if (blueBall.getRaycast()->hasHit())
		{
			GameObject*				m_Selected;
			m_Selected = (GameObject*)(blueBall.getRaycast()->m_collisionObject->getUserPointer());
			if (m_Selected->getName().compare("blueCorner") == 0)
				bDoublePointsBlue = true;
			else
				bDoublePointsBlue = false;
		}

		if (bBlueInGame)
			if (blueBall.getModel().getRigidBody()->getLinearVelocity().y() < -1.f)
			{
				iPoints += 10 + bDoublePointsBlue*10;
				bBlueInGame = false;
			}
		if (bGreenInGame)
			if (greenBall.getModel().getRigidBody()->getLinearVelocity().y() < -1.f)
			{
				iPoints += 10 + bDoublePointsGreen * 10;
				bGreenInGame = false;
			}
		if(bRedInGame)
			if (redBall.getModel().getRigidBody()->getLinearVelocity().y() < -1.f)
			{
				iPoints += 10 + bDoublePointsRed * 10;
				bRedInGame =  false;
			}
		if(bYellowInGame)
			if (yellowBall.getModel().getRigidBody()->getLinearVelocity().y() < -1.f)
			{
				iPoints += 10 + bDoublePointsYellow * 10;
				bYellowInGame = false;
			}

		time.setString(std::to_string(minutes) + ":" + std::to_string(seconds) + ":" +  std::to_string(millis));
		points.setString(std::to_string(iPoints));
		
		worldPhysics.Update();

		if ((sf::Keyboard::isKeyPressed(sf::Keyboard::W)) && (kugel.getModel().getRigidBody()->getLinearVelocity().y() <= abs(0.05)))
			kugel.getModel().getRigidBody()->applyCentralForce(btVector3(0, 0, -1.f));	
		if ((sf::Keyboard::isKeyPressed(sf::Keyboard::A)) && (kugel.getModel().getRigidBody()->getLinearVelocity().y() <= abs(0.05)))
			kugel.getModel().getRigidBody()->applyCentralForce(btVector3(-1.f, 0, 0));
		if ((sf::Keyboard::isKeyPressed(sf::Keyboard::S)) && (kugel.getModel().getRigidBody()->getLinearVelocity().y() <= abs(0.05)))
			kugel.getModel().getRigidBody()->applyCentralForce(btVector3(0, 0, 1.f));
		if ((sf::Keyboard::isKeyPressed(sf::Keyboard::D)) && (kugel.getModel().getRigidBody()->getLinearVelocity().y() <= abs(0.05)))
			kugel.getModel().getRigidBody()->applyCentralForce(btVector3(1.f, 0, 0));

		kugel.Update();
		blueBall.Update();
		greenBall.Update();
		redBall.Update();
		yellowBall.Update();
		
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.clear();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera.MapCameraToShader(shader.getShaderID());
		light.MapLightToShader(shader.getShaderID(), camera.getPosition(), camera.getLookDir());
		
		//draw OpenGL stuff
		kugel.DrawMesh(shader.getShaderID());
		blueBall.DrawMesh(shader.getShaderID());
		greenBall.DrawMesh(shader.getShaderID());
		redBall.DrawMesh(shader.getShaderID());
		yellowBall.DrawMesh(shader.getShaderID());
		terrain.DrawMesh(shader.getShaderID());
		greenCorner.DrawMesh(shader.getShaderID());
		redCorner.DrawMesh(shader.getShaderID());
		yellowCorner.DrawMesh(shader.getShaderID());
		blueCorner.DrawMesh(shader.getShaderID());
		
		//draw SFML stuff
		window.pushGLStates();
		window.draw(time);
		window.draw(points);
		window.draw(result);
		window.display();
		window.popGLStates();
	}

	return 0;
}