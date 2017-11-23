#include "main_header.h"

#ifndef PARTICLESYSTEM_H_
#define PARTICLESYSTEM_H_

const int MAX_PARTICLES = 50000;

struct Particle
{
   glm::vec3 pos, speed;
	glm::vec4 color; // Color
	float size, angle, weight;
	float life; // Remaining life of the particle. if < 0 : dead and unused.
	
};

class ParticleSystem
{
public:

    bool InitParticleSystem();

    void Render(GLuint iShader);
	void Update(int DeltaMikros);
	bool ParticleSystem::setShader(GLuint shader_id);
	int  FindUnusedParticle();
private:
	GLuint billboard_vertex_buffer;
	GLuint particles_position_buffer;
	GLuint particles_color_buffer;
	GLint iParticlesInUse;
	GLfloat fTimer;
	GLfloat fFrequency;
	GLint iParticleTexture;
	glm::vec4 position_data[MAX_PARTICLES];
	glm::vec4 color_data[MAX_PARTICLES];
    
	Particle ParticlesContainer[MAX_PARTICLES];
	int LastUsedParticle;
	sf::Clock EmitterTimer;
}; 

#endif