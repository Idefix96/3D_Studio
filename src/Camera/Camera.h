#include "main_header.h"

#ifndef CAMERA_H_
#define CAMERA_H_

class Camera
{
private:
	glm::mat4 mPerspectiveMatrix;
	glm::mat4 mWorldToCameraMatrix;
	glm::mat4 mRotX;
	glm::mat4 mRotY;
	glm::vec3 vPosition;
	glm::vec3 vCenter;
	glm::vec3 vLookDir;
	glm::vec3 vUp;
	glm::vec3 vXaxis;

	int iTriggerCoordinateForRotation;
	bool bMouseClickDetected;

	float m_Fovy;
	float m_Width;
	float m_Height;
	float m_zNear;
	float m_zFar;

public:
	Camera(float fov, float width, float height, float zNear, float zFar);

	void Translate(glm::vec3 vTranslation);
	void RotateAroundYaxis(float fAlpha);
	void RotateAroundXaxis(float fAlpha);
	void MoveForward(float fWay);
	void MoveUp(float fWay);
	void MoveOnGround(float fWay);
	void TriggerRotationWithMouse();
	void ControlWithMouse(int x, int y);
	void DecreaseFovy(float fovy);
	void IncreaseFovy(float fovy);
	void MapCameraToShader(GLuint shader);

	void Update();

	void setPosition(glm::vec3 vNewPosition);
	void setCenter(glm::vec3 vNewCenter);
	void setDefaultProjectionMatrix();
	void setOrthoProjection();

	glm::mat4 getPerspectiveMatrix();
	glm::mat4 getWorldToCameraMatrix();
	glm::vec3 getXVector();
	glm::vec3 getYVector();
	glm::vec3 getZVector();
	glm::vec3 getPosition();
	glm::vec3 getLookDir();
	float getFov();
	float getWidth();
	float getHeight();
	float getZNear();
	float getZFar();
};

#endif