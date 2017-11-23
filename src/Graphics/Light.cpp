#include "Light.h"

void Light::InitDefault()
{
		//Define light parameters
    ambientColor = glm::vec3(1);
    ambIntensity = 0.7;
    dirColor = glm::vec3(1);;
    dirIntensity = 0.3;
    dir = glm::vec3(0,-1, 0);
    specInt = 0.2;
	lightPosition = glm::vec3(0,10,0);

	float near_plane = 0.1f, far_plane =100.0f;
 lightProjection = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, near_plane, far_plane);  
	glm::mat4 biasMatrix(
0.5, 0.0, 0.0, 0.0,
0.0, 0.5, 0.0, 0.0,
0.0, 0.0, 0.5, 0.0,
0.5, 0.5, 0.5, 1.0
);
	WorldToLight = lightProjection*glm::lookAt(lightPosition, lightPosition+dir,glm::vec3(0,0,1));//glm::cross(glm::vec3(0,1,0),lightPosition+dir));
	WorldToLightShadow = biasMatrix*lightProjection*glm::lookAt(lightPosition, lightPosition+dir,glm::vec3(0,0,1));//glm::cross(glm::vec3(0,1,0),lightPosition+dir));

}


void Light::MapLightToShader(GLuint Shader, glm::vec3 vCameraPos, glm::vec3 vCameraCenter)
{
	//lightPosition = vCameraPos + glm::vec3(20)*vCameraCenter + glm::vec3(0,20,0);
	float near_plane = 0.1f, far_plane =110.0f;
	lightProjection = glm::ortho(-40.0f, 40.0f, -40.0f, 40.0f, near_plane, far_plane);  
	glm::mat4 biasMatrix(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	);
	WorldToLight = lightProjection*glm::lookAt(lightPosition, lightPosition+dir,glm::vec3(0,0,1));//glm::cross(glm::vec3(0,1,0),lightPosition+dir));
	WorldToLightShadow = biasMatrix*lightProjection*glm::lookAt(lightPosition, lightPosition+dir,glm::vec3(0,0,1));//glm::cross(glm::vec3(0,1,0),lightPosition+dir));

	glUseProgram(Shader);
	glUniformMatrix4fv(glGetUniformLocation(Shader,"gLightWVP"), 1, GL_FALSE, glm::value_ptr(WorldToLight));
	glUniformMatrix4fv(glGetUniformLocation(Shader,"gLightWVP2"), 1, GL_FALSE, glm::value_ptr(WorldToLightShadow));
    glUniform3fv(glGetUniformLocation(Shader, "AmbCol"), 1, glm::value_ptr(ambientColor));
    glUniform1f(glGetUniformLocation(Shader, "AmbInt"), ambIntensity);
    glUniform3fv(glGetUniformLocation(Shader, "DirCol"),1, glm::value_ptr(dirColor));
    glUniform1f(glGetUniformLocation(Shader, "DirInt"),dirIntensity);
    glUniform3fv(glGetUniformLocation(Shader, "Dir"),1, glm::value_ptr(dir));
    glUniform1f(glGetUniformLocation(Shader, "SpecularIntensity"),specInt);
}

void Light::MoveLight(glm::vec3 vMove)
{

	lightPosition += vMove;
	WorldToLight = glm::lookAt(lightPosition, lightPosition+dir,glm::cross(glm::vec3(1,0,0),lightPosition+dir));
	glm::mat4 biasMatrix(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	);
	WorldToLightShadow = biasMatrix*lightProjection*glm::lookAt(lightPosition, lightPosition + dir, glm::vec3(0, 0, 1));
}

//*************Get-functions

glm::vec3 Light::getLightPosition()
{
	return Light::lightPosition;
}

glm::vec3 Light::getLightDirection()
{
	return Light::dir;
}

glm::mat4 Light::GetWorldToLightMatrix()
{
	return Light::WorldToLight;
}

void Light::IncreaseAmbInt()
{
	if (ambIntensity < 1.0)
		ambIntensity += 0.1;
}

void Light::DecreaseAmbInt()
{
	if (ambIntensity > 0.0)
		ambIntensity -= 0.1;
}

void Light::IncreaseDirInt()
{
	if (dirIntensity < 1.0)
		dirIntensity += 0.1;
}

void Light::DecreaseDirInt()
{
	if (dirIntensity > 0.0)
		dirIntensity -= 0.1;
}

void Light::IncreaseSpecInt()
{
	if (specInt < 1.0)
		specInt += 0.1;
}

void Light::DecreaseSpecInt()
{
	if (specInt > 0.0)
		specInt -= 0.1;
}

float Light::getAmbientIntensity()
{
	return ambIntensity;
}

float Light::getDirectionalIntensity()
{
	return dirIntensity;
}

float Light::getSpecularIntensity()
{
	return specInt;
}

glm::vec3 Light::getAmbientColor()
{
	return ambientColor;
}

glm::vec3 Light::getDirectionalColor()
{
	return dirColor;
}

void  Light::setPosition(glm::vec3 position)
{
	lightPosition = position;
	WorldToLight = glm::lookAt(lightPosition, lightPosition + dir, glm::cross(glm::vec3(1, 0, 0), lightPosition + dir));
	glm::mat4 biasMatrix(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	);
	WorldToLightShadow = biasMatrix*lightProjection*glm::lookAt(lightPosition, lightPosition + dir, glm::vec3(0, 0, 1));
}

void Light::ChangeAmbientColor(glm::vec3 color)
{
	ambientColor += color;
}

void Light::ChangeDirectionalColor(glm::vec3 color)
{
	dirColor += color;
}

void Light::changeDirection(glm::vec3 dir)
{
	Light::dir += dir;
	WorldToLight = glm::lookAt(lightPosition, lightPosition + dir, glm::cross(glm::vec3(1, 0, 0), lightPosition + dir));
	glm::mat4 biasMatrix(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	);
	WorldToLightShadow = biasMatrix*lightProjection*glm::lookAt(lightPosition, lightPosition + dir, glm::vec3(0, 0, 1));
}
