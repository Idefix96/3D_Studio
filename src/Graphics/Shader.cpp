#include "Shader.h"

void Shader::Init()
{
	Shader::ProgramID = glCreateProgram();
}

void Shader::SetupVertexShader(std::string file_name)
{
	std::string ShaderText;
	std::ifstream file;
	std::string line;

	Shader::VertexShaderID = glCreateShader(GL_VERTEX_SHADER);

	file_name = SHADER_PATH + file_name;

	file.open(file_name, std::ios::in);	
	if(file.is_open())
	{
		while (getline(file, line))
		{
			ShaderText += line;
			if (line.find("version") != std::string::npos)
				ShaderText += '\n';
		}

	GLint Lengths;

    Lengths = strlen(ShaderText.c_str());
	const char *c_str = ShaderText.c_str();
    glShaderSource(Shader::VertexShaderID, 1, &c_str, &Lengths);    
    glCompileShader(Shader::VertexShaderID);

    //check for compilation errors
    GLint success;
    glGetShaderiv(Shader::VertexShaderID, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        GLchar InfoLog[1024];
        glGetShaderInfoLog(Shader::VertexShaderID, sizeof(InfoLog), NULL, InfoLog);
        fprintf(stderr, "Error compiling shader type %d: '%s'\n", GL_VERTEX_SHADER, InfoLog);
    }
    glAttachShader(Shader::ProgramID, Shader::VertexShaderID);
	//setup final shader program
    glLinkProgram(Shader::ProgramID);
    glValidateProgram(Shader::ProgramID);
	}
	else std::cout << "Could not find Vertex Shader" << std::endl;
}

void Shader::SetupFragmentShader(std::string file_name)
{
	std::string ShaderText;
	std::ifstream file;
	std::string line;

	Shader::FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	file_name = SHADER_PATH + file_name;

	file.open(file_name, std::ios::in);	
	if(file.is_open())
	{
		while (getline(file, line))
		{
			ShaderText += line;
			if (line.find("version") != std::string::npos)
				ShaderText += '\n';
		}

	GLint Lengths;

    Lengths = strlen(ShaderText.c_str());
	const char *c_str = ShaderText.c_str();
    glShaderSource(Shader::FragmentShaderID, 1, &c_str, &Lengths);    
    glCompileShader(Shader::FragmentShaderID);

    //check for compilation errors
    GLint success;
    glGetShaderiv(Shader::FragmentShaderID, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        GLchar InfoLog[1024];
        glGetShaderInfoLog(Shader::FragmentShaderID, sizeof(InfoLog), NULL, InfoLog);
        fprintf(stderr, "Error compiling shader type %d: '%s'\n", GL_FRAGMENT_SHADER, InfoLog);
    }
    glAttachShader(Shader::ProgramID, Shader::FragmentShaderID);

	//setup final shader program
    glLinkProgram(Shader::ProgramID);
    glValidateProgram(Shader::ProgramID);
	}
	else std::cout << "Could not find Fragment Shader" << file_name << std::endl;
}

GLuint Shader::getShaderID()
{
	return Shader::ProgramID;
}