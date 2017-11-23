#include "main_header.h"

#ifndef LIGHT_H_
#define LIGHT_H_

class Light
{

private:
	//ambient
    glm::vec3 ambientColor;
    float ambIntensity;
    //directional
    glm::vec3 dirColor;
    float dirIntensity;
   
    //specular
    float specInt;
	
	glm::mat4 WorldToLightShadow;
	glm::mat4 WorldToLightShadow_middle;
	glm::mat4 WorldToLightShadow_far;

public:
	glm::vec3 lightPosition;
	glm::vec3 lightPosition_middle;
	glm::vec3 lightPosition_far;
	glm::vec3 dir;
	glm::mat4 WorldToLight; // for shadow computing
	glm::mat4 WorldToLight_middle; // for shadow computing
	glm::mat4 WorldToLight_far; // for shadow computing
	glm::mat4 lightProjection;

	void DecreaseSpecInt();
	void DecreaseAmbInt();
	void DecreaseDirInt();
	void IncreaseSpecInt();
	void IncreaseAmbInt();
	void IncreaseDirInt();
	void InitDefault();
	void MapLightToShader(GLuint Shader, glm::vec3 vCameraPos, glm::vec3 vCameraCenter);
	glm::vec3 getLightPosition();
	glm::vec3 getLightDirection();
	glm::mat4 GetWorldToLightMatrix();
	float getAmbientIntensity();
	float getDirectionalIntensity();
	float getSpecularIntensity();
	glm::vec3 getAmbientColor();
	glm::vec3 getDirectionalColor();
	void setPosition(glm::vec3 position);
	void MoveLight(glm::vec3 vMove);
	void ChangeAmbientColor(glm::vec3 color);
	void ChangeDirectionalColor(glm::vec3 color);
	void changeDirection(glm::vec3 dir);
};

#endif