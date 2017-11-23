//Class for loading and integrating 3D files into program
//Triangulated faces only supported, so be sure to mark this option when exporting your file from your modelling software


#include "main_header.h"
#include "Shader.h"
#include "Animation.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h> 
#include <assimp/postprocess.h>

#ifndef MESH_H_
#define MESH_H_

#define ANIM_BLEND_TIME_MS		250000.0f

struct Bone
{
	unsigned int			iIndex;
	std::string				strName;
	glm::mat4				OffsetMatrix;
	glm::mat4				TransformationMatrix;
	Bone*					parent;
	glm::mat4				AnimationTransform;
	int						iLevel;
	std::vector<Bone*>		children;
}; 

class Mesh
{
	friend class GameObject;
protected:

	static int				m_iNumberOfMeshes;
	int						m_iMeshIndex;
	bool					m_bValid;
	//Buffers
	GLuint					vbo_pos[1000];	//Position Buffer
	GLuint					vbo_uv[1000];	//UV Buffer
	GLuint					vbo_norm[1000];	//Normal Buffer
	GLuint					vbo_tan[1000];	//tangent buffer for normal mapping
	GLuint					ibo[1000];		//Index Buffer
	GLuint					vbo_boneid[1000];
	GLuint					vbo_boneweight[1000];
	GLuint					vbo_boneid2[1000];
	GLuint					vbo_boneweight2[1000];
	GLuint					vbo_boneTransform[1000];
	GLuint					iShaderID;
	GLuint					m_iFramebuffer;
	GLuint					shadowmap;
	GLuint					shadowmap_middle;
	GLuint					shadowmap_far;

	//transformation data
	glm::mat4				mTranslationMatrix;
	glm::vec3				vTranslationVector;
	glm::mat4				mRotationXMatrix;
	glm::mat4				mRotationYMatrix;
	glm::mat4				mRotationZMatrix;
	glm::mat4				mScalingMatrix;
	glm::mat4				mBoneTransformationMatrix[200];

	//mesh structure with data for rendering mesh
	VertexList				meshes[300];
	std::vector<GLuint>		mesh_indices[300];
	GLuint					iNumMeshes;
	GLuint					iNumBones;

	//materials
	std::vector<Material>	material_list;
	std::vector<GLuint>		material_index;
	glm::vec3				m_DefaultColor;

	//animation data
	Animation				mAnimations[10];
	unsigned int			iNumAnimations;
	Bone					boSkeleton[200];
	Bone* boRoot;
	//std::vector<Bone> Skeleton;
	std::map<std::string,Bone> Skeleton;
	aiNode* RootBone;
	float					fAnimationTime;
	sf::Clock				m_AnimationTimer;
	sf::Clock				m_AnimationBlendingTimer;
	//controlling
	glm::vec3				vDirection;
	glm::vec3				vCenter;
	glm::vec3				vLocalX;
	 struct
	 {
							glm::vec3 vTopRightBack;
							glm::vec3 vBottomLeftFront;
							float fWidth;
							float fHeight;
							float fDepth;
							glm::vec3 vCenterPoint;
	 } boundingBox;

	 //loading
	 std::string			m_strStatus;
	 const aiScene* pScene;

public:

	//constructor
	Mesh();

	void LoadAssimpFile(std::string file_name);
	void SetupMesh(glm::vec3 *Vertices, int iNumVerts, int *Indices, int iNumIndices);

	//select Shader
	void setShader(GLuint shader_id);
	void setShadowmap(GLuint iShadowmap);
	GLuint getShader();
	//scale
	void Scale(float fScale);
	void ScaleXZ(float fScale);
	//step by step translation
	void Translate(glm::vec3 vTranslation);
	void Rotate(glm::vec3 vRotation);
	void Rotate(glm::quat qRotation);
	//continous translation
	void ContTranslate(glm::vec3 vTranslation);
	//continous rotation
	void ContRotate(glm::vec3 vRotation);
	void ContRotate(glm::quat qRotation);

	//void Animate(unsigned int iAnimID, float fTime);
	void Animate(float fTime, int iAnimationIndex);
	void Animate(float fTime, std::string strSubstrName);
	void RotateBone(std::string boneName, float angle, glm::vec3 axis = glm::vec3(0,0,1));
	void StartAnimationTimer();
	void StartAnimationBlendingTimer();
	bool AnimationBlending(int SourceAnimation, int DestinationAnimation);
	void Render(GLuint iShader);
	void CreateShadowMap();
	Bone* getParentOfBone(Bone bone);

	glm::mat4		getTranslationMatrix();
	glm::mat4		getRotationMatrix();

	glm::vec3		getDirection();
	glm::vec3		getLocalX();
	float			getBoundingBoxWidth();
	glm::vec3		getBoundingCorner1();
	glm::vec3		getBoundingCorner2();
	glm::vec3		getBoundingCenter();
	float			getBoundingBoxHeight();
	float			getBoundingBoxDepth();
	unsigned int	getNumAnimations();
	std::string		getAnimationName(int iIndex);
	int				getNumTriangles();
	int				getNumVertices();
	GLuint*			getIndexArray();
	glm::vec3*		getVertexArray();
	//bounding box, defined by two corners
	CollisionBox	BoundingBox;

	void setMaterialColor(int iMatIndex, glm::vec3 vColor);
	glm::vec3 getMaterialColor(int iMatIndex);

	bool isValid();

	std::string getStatus();

};

#endif
