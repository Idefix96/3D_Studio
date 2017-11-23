#include "ParticleSystem.h"
#include <ctime>
#include <cstdlib>

 bool ParticleSystem::InitParticleSystem()
 {
	
	 // The VBO containing the 4 vertices of the particles.
	// Thanks to instancing, they will be shared by all particles.
	static const GLfloat g_vertex_buffer_data[] = {
					-0.125f, -0.125f, -1.5f,
					0.125f, -0.125f, -1.5f,
					-0.125f, 0.125f, -1.5f,
					0.125f, 0.125f, -1.5f
					};

	glGenBuffers(1, &billboard_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	// The VBO containing the positions and sizes of the particles

	glGenBuffers(1, &particles_position_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);


	// The VBO containing the colors of the particles
	glGenBuffers(1, &particles_color_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);

	for(int i=0; i<MAX_PARTICLES; i++)
	{
		ParticlesContainer[i].life = -1.0f;
	}

	fTimer = 0.0f;		//ms
	fFrequency = 50.0f; //ms
	iParticlesInUse = 0;
	LastUsedParticle = 0;
	srand(time(0));

	std::string texture = "C:/Users/Florian/Desktop/Visual C++ Projekte/MeshLib/data/particles/textures/glow2.png";

	std::string texture_file = /*TEXTURE_PATH + */texture;
	iParticleTexture = SOIL_load_OGL_texture(texture_file.c_str(), SOIL_LOAD_RGBA,SOIL_CREATE_NEW_ID, SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_INVERT_Y);
	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	return true;
}

void ParticleSystem::Update(int DeltaMikros)
{	
	

	if (iParticlesInUse < MAX_PARTICLES ) // == 0)
	{
		
		if (EmitterTimer.getElapsedTime().asMilliseconds() > 30)
		{
			iParticlesInUse += 50; // = 500;//100000;
		
			for(int i=0; i<50; i++)
			{
				int index = FindUnusedParticle();
	
				ParticlesContainer[index].life = 1500000.0f; //15000.0f;// + rand()%5000 - 2500 ; //ms
				ParticlesContainer[index].pos = glm::vec3(0,0,-2.0f);
				ParticlesContainer[index].speed = glm::vec3( float(rand() % 500 - 250)/100000000.0f,float(rand() % 500 - 250)/100000000.0f,float(rand() % 500 - 250)/100000000.0f);
				ParticlesContainer[index].color = glm::vec4( float(rand() % 100)/100,float(rand() % 100)/100,float(rand() % 100)/100, 1.0);
			}
			EmitterTimer.restart();
		}
	}
	static int iUsedIndex = 0;
	for(int i = 0; i<= MAX_PARTICLES - 1; i++)
	{
		
		if (ParticlesContainer[i].life >= 0.f)
		{
			ParticlesContainer[i].life -= float(DeltaMikros);
			ParticlesContainer[i].pos += ParticlesContainer[i].speed * glm::vec3(float(DeltaMikros));
			position_data[iUsedIndex] = glm::vec4(ParticlesContainer[i].pos.x, ParticlesContainer[i].pos.y, ParticlesContainer[i].pos.z, 1.0);
			color_data[iUsedIndex] = ParticlesContainer[i].color;

			if (ParticlesContainer[i].life <= 150000.0f) 
				color_data[iUsedIndex].w = ParticlesContainer[i].life/150000.0f;	

			if (ParticlesContainer[i].life < 0.0f)
				iParticlesInUse = iParticlesInUse-1;

			iUsedIndex++;
		}	

	}
	iUsedIndex = 0;
	
	glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
	glBufferData(GL_ARRAY_BUFFER, iParticlesInUse * 4 * sizeof(GLfloat), &position_data[0], GL_DYNAMIC_DRAW); 
	
	glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
	glBufferData(GL_ARRAY_BUFFER, iParticlesInUse * 4 * sizeof(GLfloat), &color_data[0], GL_DYNAMIC_DRAW); 

	
}

void ParticleSystem::Render(GLuint iShader)
{
	
	glUseProgram(iShader);
	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void*)0);

	// 2nd attribute buffer : positions of particles' centers
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// 3rd attribute buffer : particles' colors
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
	glVertexAttribDivisor(1, 1); // positions : one per quad (it's center) -> 1/
	glVertexAttribDivisor(2, 1); // color : one per quad -> 1

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, iParticleTexture);
	glUniform1i(glGetUniformLocation(iShader, "Sampler"),0);

	glEnable(GL_BLEND);
	glEnable(GL_ALPHA);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glDepthMask(GL_FALSE);
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4,iParticlesInUse);
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);

	//clean up
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2); 

	glVertexAttribDivisor(0, 0);
	glVertexAttribDivisor(1, 0); 
	glVertexAttribDivisor(2, 0); 

	
}

bool ParticleSystem::setShader(GLuint shader_id)
{
	return true;
}


// Finds a Particle in ParticlesContainer which isn't used yet.
// (i.e. life < 0);
int ParticleSystem::FindUnusedParticle(){
	
    for(int i=LastUsedParticle; i<MAX_PARTICLES; i++){
        if (ParticlesContainer[i].life < 0.f){
            LastUsedParticle = i;
            return i;
        }
    }

    for(int i=0; i<LastUsedParticle; i++){
        if (ParticlesContainer[i].life < 0.f){
            LastUsedParticle = i;
            return i;
        }
    }
    return 0; // All particles are taken, override the first one
}
