#include "main_header.h"

#ifndef SHADER_H_
#define SHADER_H_

class Shader
{
private:
	GLuint VertexShaderID;
	GLuint FragmentShaderID;
	GLuint ProgramID;

public:

	void Init();

	//load and compile shaders
	void Shader::CompileShaders(GLenum ShaderType,  GLuint ID, std::string file_name);
	void SetupVertexShader(std::string file_name);
	void SetupFragmentShader(std::string file_name);
	GLuint getShaderID();
	GLuint getProgramID();
};

#endif