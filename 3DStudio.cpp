#include "GL\glew.h"

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>

#include "Scene\scenemanager.h"
#include "Graphics\Mesh.h"
#include "Physics\GameObject.h"
#include "Physics\PhysicsManager.h"
#include "Physics\physicmodel.h"

PhysicsManager worldPhysics;

GameObject		terrain;
GameObject		kugel;
GameObject		mesh;
Mesh		sky;

int main()
{
	sf::ContextSettings Settings;
	Settings.depthBits = 24;
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "My window", sf::Style::Default, Settings);
	glewInit();

	worldPhysics.Initialize();

	mesh.GenerateObject("Rex/Rex.fbx");

	mesh.ApplyCapsuleShape(mesh.getMesh()->getBoundingBoxWidth()/2.f, mesh.getMesh()->getBoundingBoxHeight() - mesh.getMesh()->getBoundingBoxWidth(), 1, glm::quat(0, 0, 0, 1), glm::vec3(0, 15, 0));

	//mesh.ApplySphereShape(mesh.getMesh()->getBoundingBoxHeight() / 2.f, 10, glm::quat(0, 0, 0, 1), glm::vec3(0, 10, 0));
	mesh.ApplyPhysicsManager(&worldPhysics);
	worldPhysics.AddPhysicModel(&mesh.getModel());
	

	kugel.GenerateObject("kugel.fbx");
	kugel.ApplySphereShape(kugel.getMesh()->getBoundingBoxHeight() / 2.f, 10, glm::quat(0, 0, 0, 1), glm::vec3(0, 10, 0));
	//kugel.ApplyPhysicsManager(&worldPhysics);
	//worldPhysics.AddPhysicModel(&kugel.getModel());

	sky.LoadAssimpFile("skydome/Skydome.fbx");
	terrain.GenerateObject("terrain/terrain.fbx");
	terrain.ApplyIndexedMeshShape();
	terrain.ApplyPhysicsManager(&worldPhysics);
	worldPhysics.AddPhysicModel(&terrain.getModel());

	Shader shader;
	shader.Init();
	std::cout << "Loading default 3D vertex shader..." << std::endl;
	shader.SetupVertexShader("VS.vs");
	std::cout << "Loading default 3D fragment shader..." << std::endl;
	shader.SetupFragmentShader("FS.fs");
	
	Light light;
	light.InitDefault();

	Camera camera(3.14159f/4.0f, 1920.0f, 1080.0f, 0.1f, 10000.0f);

	sf::Font font;

	if (!font.loadFromFile("arial.ttf"))
	{
		std::cout << "Font not found";
	}
	sf::Text lightText;
	sf::Text cameraText;
	sf::Text descriptionText;
	lightText.setFont(font);
	lightText.setPosition(50, 50);
	lightText.setCharacterSize(12);
	cameraText.setFont(font);
	cameraText.setPosition(50, 250);
	cameraText.setCharacterSize(12);
	descriptionText.setFont(font);
	descriptionText.setPosition(50, 450);
	descriptionText.setCharacterSize(12);
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	sf::Clock timer;

	// run the program as long as the window is open
	while (window.isOpen())
	{
		worldPhysics.Update();
		
		kugel.Update();
		mesh.StateMachine();
		mesh.Update();
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				window.close();

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			{
				camera.MoveForward(0.1);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			{				
				camera.MoveForward(-0.1);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y))
			{
				camera.MoveUp(0.1);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
			{
				camera.MoveUp(-0.1);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{
				camera.RotateAroundYaxis(-0.01);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				camera.RotateAroundYaxis(0.01);
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
			{
				//light.IncreaseAmbInt();
				kugel.getModel().getRigidBody()->setActivationState(DISABLE_DEACTIVATION);
				kugel.getModel().getRigidBody()->applyForce( btVector3(10,0,0),  btVector3(0,0,0));
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::K))
			{
				light.DecreaseAmbInt();
				//mesh.RotateBone("neck", -0.05);
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
			{
				light.IncreaseAmbInt();
				//mesh.RotateBone("spine", 0.05);
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
			{
				light.DecreaseAmbInt();
				//mesh.RotateBone("spine", -0.05);
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
			{
				light.changeDirection(glm::vec3(0.1,0,0));
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::O))
			{
				light.changeDirection(glm::vec3(-0.1, 0, 0));
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::N))
			{
				camera.IncreaseFovy(0.1);
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
			{
				camera.DecreaseFovy(0.1);
			}

			
			
			
		}


		window.clear();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera.MapCameraToShader(shader.getShaderID());
		light.MapLightToShader(shader.getShaderID(), camera.getPosition(), camera.getLookDir());
		mesh.DrawMesh(shader.getShaderID());
		kugel.DrawMesh(shader.getShaderID());
		terrain.DrawMesh(shader.getShaderID());
		
		sky.Render(shader.getShaderID());
		window.pushGLStates();

		
		lightText.setString("LIGHT \n\nPosition: " + to_string(light.getLightPosition().x) + " " + to_string(light.getLightPosition().y) + " " + to_string(light.getLightPosition().z) +
			"\nDirection: " + to_string(light.getLightDirection().x) + " " + to_string(light.getLightDirection().y) + " " + to_string(light.getLightDirection().z) +
			"\n\nIntensity \nAmbient: " + to_string(light.getAmbientIntensity()) +
			"\nDirectional: " + to_string(light.getDirectionalIntensity()) +
			"\nSpecular: " + to_string(light.getSpecularIntensity()) +
			"\n\nColor \n" +
			"Ambient: " + "R: " + to_string(light.getAmbientColor().x) + " G: " + to_string(light.getAmbientColor().y) + " B: " + to_string(light.getAmbientColor().z) +
			"\nDirectional: " + "R: " + to_string(light.getDirectionalColor().x) + " G: " + to_string(light.getDirectionalColor().y) + " B: " + to_string(light.getDirectionalColor().z)
		);
		cameraText.setString("CAMERA \n\nPosition: " + to_string(camera.getPosition().x) + " " + to_string(camera.getPosition().y) + " " + to_string(camera.getPosition().z) +
			"\nDirection: " + to_string(camera.getLookDir().x) + " " + to_string(camera.getLookDir().y) + " " + to_string(camera.getLookDir().z) +
			"\nData: \nField of View: " + to_string(camera.getFov()) +
			"\nWidth: " + to_string(camera.getWidth()) +
			"\nHeight: " + to_string(camera.getHeight()) +
			"\nZ Near: " + to_string(camera.getZNear()) +
			"\nZ Far: " + to_string(camera.getZFar())
		);
		descriptionText.setString("Camera Control \nForward: W \nBackward: S \nTurn Left: D \nTurn Right: A \nUp: Y \nDown: X \n\nLight Control \n Increase Ambient: L \n Decrease Ambient: K \nMove Right: O \n Move Left: P ");
		
		window.draw(lightText);
		window.draw(cameraText);
		window.draw(descriptionText);
		window.display();
		window.popGLStates();
		
		//mesh.getMesh()->Animate(timer.getElapsedTime().asMicroseconds(), 0);
	}

	return 0;
}