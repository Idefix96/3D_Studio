#include "Camera.h"

Camera::Camera(float fov, float width, float height, float zNear, float zFar)
{
	Camera::mPerspectiveMatrix = glm::perspective(fov, width / height, zNear, zFar);
	Camera::vPosition = glm::vec3(6.5,3,4);

	Camera::mRotY = glm::mat4(1.0f);
	Camera::mRotX = glm::mat4(1.0f);
	//OpenGL standard: camera looks along negative z-axis
	Camera::vCenter = glm::vec3(0,0,-1);
	Camera::vUp = glm::vec3(0,1,0);
	Camera::vLookDir = glm::normalize(Camera::vCenter - Camera::vPosition);
	Camera::mWorldToCameraMatrix = glm::lookAt(Camera::vPosition, Camera::vCenter, Camera::vUp);

	m_Fovy = fov;
	m_Width = width;
	m_Height = height;
	m_zNear = zNear;
	m_zFar = zFar;
}

void Camera::MapCameraToShader(GLuint shader)
{
	glUseProgram(shader);
	glUniformMatrix4fv(glGetUniformLocation(shader,"gPersp"), 1, GL_FALSE, glm::value_ptr(mPerspectiveMatrix));
	glUniformMatrix4fv(glGetUniformLocation(shader,"gWorldToCamera"), 1, GL_FALSE, glm::value_ptr(getWorldToCameraMatrix()));
	glm::mat4 CamBT =glm::lookAt(glm::vec3(0), Camera::vLookDir, Camera::vUp);
	glUniformMatrix4fv(glGetUniformLocation(shader,"gCameraRotation"), 1, GL_FALSE, glm::value_ptr(CamBT));
	glUniform3fv(glGetUniformLocation(shader, "EyePos"),1, glm::value_ptr(getPosition()));
	glUniform3fv(glGetUniformLocation(shader, "CameraRight"),1, glm::value_ptr(getXVector()));
	//glUniform3fv(glGetUniformLocation(shader, "CameraUp"),1, glm::value_ptr(vUp));
	glm::vec4 vTempUp =  Camera::mRotX*glm::vec4(Camera::vUp.x,Camera::vUp.y, Camera::vUp.z,0.0);
	glUniform3fv(glGetUniformLocation(shader, "CameraUp"),1, glm::value_ptr(glm::vec3(vTempUp.x,vTempUp.y,vTempUp.z)));
	glUniform3fv(glGetUniformLocation(shader, "CameraLook"),1, glm::value_ptr(vLookDir));
	
}

//sets the camera to a specific point vTranslation
void Camera::Translate(glm::vec3 vTranslation)
{
	Camera::vPosition += vTranslation;
	Camera::Update();
}

//rotates the Camera around global y-axis by fAlpha
void Camera::RotateAroundYaxis(float fAlpha)
{
	Camera::mRotY = glm::rotate(glm::mat4(1.0f),fAlpha,glm::vec3(0.0,1.0,0.0));
	
	glm::vec4 vTempLookDir = Camera::mRotY*glm::vec4(Camera::vLookDir.x, Camera::vLookDir.y, Camera::vLookDir.z, 0.0f);
	Camera::vLookDir = normalize(glm::vec3(vTempLookDir.x, vTempLookDir.y, vTempLookDir.z));
	Camera::vCenter = Camera::vPosition + Camera::vLookDir;
	Camera::Update();
}

//rotates the Camera around local x-axis by fAlpha
void Camera::RotateAroundXaxis(float fAlpha)
{
	static float fAngle = glm::acos(glm::dot(glm::normalize(glm::vec3(0,1,0)),glm::normalize(vLookDir)));

	if  ( ((fAngle <= 3.0*3.1415926/4.0) && (fAngle >= 3.1415926/4.0)) || ((fAngle > 3.0*3.1415926/4.0) && (fAlpha < 0))   || ((fAngle < 3.1415926/4.0) && (fAlpha > 0))  ) 
	{
		fAngle += fAlpha;
		Camera::mRotX = glm::rotate(glm::mat4(1.0f),fAlpha,glm::cross(Camera::vUp,Camera::vLookDir));
		
		glm::vec4 vTempLookDir = Camera::mRotX*glm::vec4(Camera::vLookDir.x, Camera::vLookDir.y, Camera::vLookDir.z, 0.0f);	
		Camera::vLookDir = glm::normalize(glm::vec3(vTempLookDir.x, vTempLookDir.y, vTempLookDir.z));
		Camera::vCenter = Camera::vPosition + Camera::vLookDir;

		Camera::Update();
	}

}

//moves the camera along vLookDir by fWay
void Camera::MoveForward(float fWay)
{
		vPosition += glm::vec3(fWay*vLookDir.x, fWay*vLookDir.y, fWay*vLookDir.z);
		vCenter +=  glm::vec3(fWay*vLookDir.x, fWay*vLookDir.y, fWay*vLookDir.z);
		vLookDir = glm::normalize(Camera::vCenter - Camera::vPosition);

		Camera::Update();
}

void Camera::MoveUp(float fWay)
{
		vPosition += glm::vec3(0, fWay, 0);
		vCenter +=  glm::vec3(0, fWay, 0);
		vLookDir = glm::normalize(Camera::vCenter - Camera::vPosition);

		Camera::Update();
}

void Camera::MoveOnGround(float fWay)
{
		glm::vec2 vLookDir2D = glm::normalize(glm::vec2(vLookDir.x, vLookDir.z));
		vPosition += glm::vec3(fWay*vLookDir2D.x, 0, fWay*vLookDir2D.y);
		vCenter +=  glm::vec3(fWay*vLookDir2D.x, 0, fWay*vLookDir2D.y);
		vLookDir = glm::normalize(Camera::vCenter - Camera::vPosition);

		Camera::Update();
}

void Camera::TriggerRotationWithMouse()
{
	bMouseClickDetected = true;
}

void Camera::ControlWithMouse(int x, int y)
{
	static int iMouseLastPositionX = x;
	static int iMouseLastPositionY = y;

	if (!bMouseClickDetected)
	{
		iMouseLastPositionX = x;
		iMouseLastPositionY = y;
	}	

	this->RotateAroundYaxis(float(iMouseLastPositionX - x)/100.0f);
	this->RotateAroundXaxis(float(iMouseLastPositionY - y)/100.0f);

	Camera::Update();
	iMouseLastPositionX = x;	
	iMouseLastPositionY = y;
	bMouseClickDetected = false;
}

void Camera::DecreaseFovy(float fovy)
{
	m_Fovy -= fovy;
	Camera::mPerspectiveMatrix = glm::perspective(m_Fovy, m_Width / m_Height, m_zNear, m_zFar);
}

void Camera::IncreaseFovy(float fovy)
{
	m_Fovy += fovy;
	Camera::mPerspectiveMatrix = glm::perspective(m_Fovy, m_Width / m_Height, m_zNear, m_zFar);
}

void Camera::Update()
{
	Camera::mWorldToCameraMatrix = glm::lookAt(Camera::vPosition, Camera::vCenter, Camera::vUp);
}

 void Camera::setPosition(glm::vec3 vNewPosition)
 {
	 vPosition = vNewPosition;
	 vLookDir = glm::normalize(Camera::vCenter - Camera::vPosition);
	 Camera::Update();
 }

 void  Camera::setCenter(glm::vec3 vNewCenter)
 {
	 vCenter = vNewCenter;
	 vLookDir = glm::normalize(Camera::vCenter - Camera::vPosition);
	 Camera::Update();
 }

glm::mat4 Camera::getPerspectiveMatrix()
{
	return Camera::mPerspectiveMatrix;
}

glm::vec3 Camera::getPosition()
{
	return Camera::vPosition;
}

glm::vec3 Camera::getLookDir()
{
	return Camera::vLookDir;
}

glm::mat4 Camera::getWorldToCameraMatrix()
{
	return Camera::mWorldToCameraMatrix;
}

glm::vec3 Camera::getXVector()
{
	return glm::normalize(glm::cross(Camera::vUp,Camera::vLookDir));
}
	
glm::vec3 Camera::getYVector()
{
	return Camera::vUp;
}

glm::vec3 Camera::getZVector()
{
	return Camera::vLookDir;
}

float Camera::getFov()
{
	return m_Fovy;
}

float Camera::getWidth()
{
	return m_Width;
}

float Camera::getHeight()
{
	return m_Height;
}

float Camera::getZNear()
{
	return m_zNear;
}

float Camera::getZFar()
{
	return m_zFar;
}

void Camera::setDefaultProjectionMatrix()
{
	Camera::mPerspectiveMatrix = glm::perspective(45.0f,1920.0f/1080.0f,0.1f,10000.0f);
}

void Camera::setOrthoProjection()
{
	Camera::mPerspectiveMatrix = glm::mat4(1.0f);
}
