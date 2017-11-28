#include "GL\glew.h"

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>
#include "Scene\scenemanager.h"
#include "Camera/Camera.h"
#include "Graphics/Light.h"
#include "Graphics/Mesh.h"
#include "Graphics/Shader.h"
#include "Keyboard\Keyboard.h"

using namespace std;

Mesh		terrain;
Mesh		kugel;

int main()
{
	sf::ContextSettings Settings;
	Settings.depthBits = 24;
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "My window", sf::Style::Default, Settings);
	glewInit();

	kugel.LoadAssimpFile("kugel.fbx");

	terrain.LoadAssimpFile("terrain/terrain.fbx");


	Shader shader;
	shader.Init();
	std::cout << "Loading default 3D vertex shader..." << std::endl;
	shader.SetupVertexShader("VS.vs");
	std::cout << "Loading default 3D fragment shader..." << std::endl;
	shader.SetupFragmentShader("FS.fs");

	Light light;
	light.InitDefault();

	Camera camera(3.14159f / 4.0f, 1920.0f, 1080.0f, 0.1f, 10000.0f);

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

	// run the program as long as the window is open
	while (window.isOpen())
	{
		//camera Control
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			camera.MoveForward(0.1);
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


		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				window.close();

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
			{
				light.IncreaseAmbInt();
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::K))
			{
				light.DecreaseAmbInt();
			}

		

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
			{
				light.changeDirection(glm::vec3(0.1, 0, 0));
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

		kugel.Render(shader.getShaderID());
		terrain.Render(shader.getShaderID());

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

		

	}
	

	return 0;
}