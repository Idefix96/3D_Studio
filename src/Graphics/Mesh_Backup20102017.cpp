#include "Mesh.h"
#include "glm/ext.hpp"

int Mesh::m_iNumberOfMeshes = 0;

glm::mat4 AiToGlmMatrix(aiMatrix4x4 aiMat)
{
	glm::mat4 ret;

	ret[0][0] = aiMat.a1;
	ret[0][1] = aiMat.b1;
	ret[0][2] = aiMat.c1;
	ret[0][3] = aiMat.d1;
	ret[1][0] = aiMat.a2;
	ret[1][1] = aiMat.b2;
	ret[1][2] = aiMat.c2;
	ret[1][3] = aiMat.d2;
	ret[2][0] = aiMat.a3;
	ret[2][1] = aiMat.b3;
	ret[2][2] = aiMat.c3;
	ret[2][3] = aiMat.d3;
	ret[3][0] = aiMat.a4;
	ret[3][1] = aiMat.b4;
	ret[3][2] = aiMat.c4;
	ret[3][3] = aiMat.d4;

	return ret;
}

glm::mat4 AiToGlmMatrix(aiMatrix3x3 aiMat)
{
	glm::mat4 ret;

	ret[0][0] = aiMat.a1;
	ret[0][1] = aiMat.b1;
	ret[0][2] = aiMat.c1;
	ret[0][3] = 0;
	ret[1][0] = aiMat.a2;
	ret[1][1] = aiMat.b2;
	ret[1][2] = aiMat.c2;
	ret[1][3] = 0;
	ret[2][0] = aiMat.a3;
	ret[2][1] = aiMat.b3;
	ret[2][2] = aiMat.c3;
	ret[2][3] = 0;
	ret[3][0] = 0;
	ret[3][1] = 0;
	ret[3][2] = 0;
	ret[3][3] = 1;

	return ret;
}

Mesh::Mesh()
{	
	//default parameters
	Mesh::mTranslationMatrix = glm::mat4(1.0f);
	Mesh::mRotationXMatrix = glm::mat4(1.0f);
	Mesh::mRotationYMatrix = glm::mat4(1.0f);
	Mesh::mRotationZMatrix  = glm::mat4(1.0f);
	Mesh::mScalingMatrix = glm::mat4(1.0f);
	
	Mesh::vTranslationVector = glm::vec3(0.0f);
	Mesh::iNumMeshes = 0;
	Mesh::iNumAnimations = 0;
	Mesh::fAnimationTime = 0;
	Mesh::vDirection = glm::vec3(0,0,1.0);
	Mesh::vLocalX =glm::vec3(1.0,0,0);
	Mesh::boundingBox.vTopRightBack = glm::vec3(0);
	Mesh::boundingBox.vBottomLeftFront = glm::vec3(0);
	Mesh::m_DefaultColor = glm::vec3(1.0,1.0,1.0);
	Mesh::shadowmap = 0;
}

void Mesh::SetupMesh(glm::vec3 *Vertices, int iNumVerts, int *Indices, int iNumIndices)
{
	for (int i=0; i<iNumVerts;i++)
	{
		meshes[0].pos.push_back(Vertices[i]);
	}
	for (int i=0; i<iNumIndices;i++)
	{
		mesh_indices[0].push_back(Indices[i]);
	}
	iNumMeshes++;

	std::cout << "Generating buffers..." << std::endl;
	glGenBuffers(1, &(Mesh::vbo_pos[0]));
	glGenBuffers(1, &(Mesh::vbo_uv[0]));
	glGenBuffers(1, &(Mesh::vbo_norm[0]));	
	glGenBuffers(1, &(Mesh::vbo_tan[0]));
	glGenBuffers(1, &(Mesh::ibo[0]));

	glBindBuffer(GL_ARRAY_BUFFER, vbo_pos[0]);
	glBufferData(GL_ARRAY_BUFFER, Mesh::meshes[0].pos.size()*3*sizeof(GLfloat), &(Mesh::meshes[0].pos[0]), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Mesh::ibo[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh_indices[0].size()*sizeof(int),&(Mesh::mesh_indices[0][0]), GL_STATIC_DRAW);
}

void Mesh::LoadAssimpFile(std::string strFile)
{
	 const aiMesh* paiMesh;
	 Assimp::Importer importer;
	 strFile = MODEL_PATH + strFile;

	 m_strStatus = "Loading Mesh from file... ";
	 std::cout << m_strStatus << strFile;
	 const aiScene* pScene = importer.ReadFile(strFile.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);
	 if (!pScene)
	 {
		 m_strStatus += "Error loading mesh";
		 std::cout << m_strStatus << std::endl;
		 m_bValid = false;
		 return;
	 }
	 m_strStatus += "Done!";
	 std::cout << m_strStatus << std::endl;
	 m_bValid = true;
	 iNumMeshes = pScene->mNumMeshes;

	 int iMaxBoneNum = 0;
	 unsigned int iMaxBoneIndex = 0;
	 for (unsigned int i = 0 ; i < pScene->mNumMeshes; i++) 
	 {
		  if ( pScene->mMeshes[i]->HasBones())
		  {
			if ( pScene->mMeshes[i]->mNumBones > iMaxBoneNum)
			{
				iMaxBoneNum =  pScene->mMeshes[i]->mNumBones;
				iMaxBoneIndex = i;
			}
		}
	 }

		  //bounding box setup
		  boundingBox.vTopRightBack.x  =  pScene->mMeshes[0]->mVertices[0].x;
		  boundingBox.vTopRightBack.y  =  pScene->mMeshes[0]->mVertices[0].y;
		  boundingBox.vTopRightBack.z  =  pScene->mMeshes[0]->mVertices[0].z;
		  boundingBox.vBottomLeftFront.x  =  pScene->mMeshes[0]->mVertices[0].x;
		  boundingBox.vBottomLeftFront.y  =  pScene->mMeshes[0]->mVertices[0].y;
		  boundingBox.vBottomLeftFront.z  =  pScene->mMeshes[0]->mVertices[0].z;

	 m_strStatus = "Reading model data...  ";
	 std::cout << m_strStatus << std::endl;
	 for (unsigned int i = 0 ; i < pScene->mNumMeshes; i++) 
	 {
		 int counter = i+1;
		 m_strStatus = "Reading model data...  Mesh " +  std::to_string((long double)counter) + " von " + std::to_string((long double)pScene->mNumMeshes);
		 std::cout << m_strStatus << std::endl;
		  paiMesh = pScene->mMeshes[i];
		  material_index.push_back(paiMesh->mMaterialIndex);

		 

		  for (int j = 0; j <= paiMesh->mNumVertices - 1; j++)
		  {	
			//Swap y and z coordinates
			meshes[i].pos.push_back(glm::vec3(paiMesh->mVertices[j].x ,paiMesh->mVertices[j].y, paiMesh->mVertices[j].z));
			meshes[i].nor.push_back(glm::vec3(paiMesh->mNormals[j].x, paiMesh->mNormals[j].y, paiMesh->mNormals[j].z));	

			if (paiMesh->HasTextureCoords(0))
			{
				meshes[i].uv.push_back(glm::vec2(paiMesh->mTextureCoords[0][j].x, paiMesh->mTextureCoords[0][j].y));
			}
			if (paiMesh->HasTangentsAndBitangents())
				meshes[i].tan.push_back(glm::vec3(paiMesh->mTangents[j].x, paiMesh->mTangents[j].y, paiMesh->mTangents[j].z));

			meshes[i].vBoneID.push_back(glm::vec4(0));
			meshes[i].fBoneWeight.push_back(glm::vec4(0));

			//bounding box boundaries
			if (paiMesh->mVertices[j].x > boundingBox.vTopRightBack.x)
				boundingBox.vTopRightBack.x = paiMesh->mVertices[j].x;
			if (paiMesh->mVertices[j].y > boundingBox.vTopRightBack.y)
				boundingBox.vTopRightBack.y = paiMesh->mVertices[j].y;
			if (paiMesh->mVertices[j].z > boundingBox.vTopRightBack.z)
				boundingBox.vTopRightBack.z = paiMesh->mVertices[j].z;

			if (paiMesh->mVertices[j].x < boundingBox.vBottomLeftFront.x)
				boundingBox.vBottomLeftFront.x = paiMesh->mVertices[j].x;
			if (paiMesh->mVertices[j].y < boundingBox.vBottomLeftFront.y)
				boundingBox.vBottomLeftFront.y = paiMesh->mVertices[j].y;
			if (paiMesh->mVertices[j].z < boundingBox.vBottomLeftFront.z)
				boundingBox.vBottomLeftFront.z = paiMesh->mVertices[j].z;

		  }
		

		for (unsigned int k = 0 ; k <= paiMesh->mNumFaces - 1; k++) 
		{
			Mesh::mesh_indices[i].push_back(paiMesh->mFaces[k].mIndices[0]);
			Mesh::mesh_indices[i].push_back(paiMesh->mFaces[k].mIndices[1]);
			Mesh::mesh_indices[i].push_back(paiMesh->mFaces[k].mIndices[2]);
		}


		if ((!paiMesh->HasTangentsAndBitangents()) && (Mesh::meshes[i].uv.size() > 0))
		{
		//write tangents
			for (unsigned int l = 0 ; l < Mesh::mesh_indices[i].size()-1 ; l += 3)
			{
		
				glm::vec3 Edge1 = Mesh::meshes[i].pos[Mesh::mesh_indices[i][l+1]] - Mesh::meshes[i].pos[Mesh::mesh_indices[i][l]]; 
				glm::vec3 Edge2 = Mesh::meshes[i].pos[Mesh::mesh_indices[i][l+2]] - Mesh::meshes[i].pos[Mesh::mesh_indices[i][l]];
		
				float DeltaU1 = Mesh::meshes[i].uv[Mesh::mesh_indices[i][l+1]].x - Mesh::meshes[i].uv[Mesh::mesh_indices[i][l]].x;
				float DeltaV1 = Mesh::meshes[i].uv[Mesh::mesh_indices[i][l+1]].y - Mesh::meshes[i].uv[Mesh::mesh_indices[i][l]].y;
				float DeltaU2 = Mesh::meshes[i].uv[Mesh::mesh_indices[i][l+2]].x - Mesh::meshes[i].uv[Mesh::mesh_indices[i][l]].x;
				float DeltaV2 = Mesh::meshes[i].uv[Mesh::mesh_indices[i][l+2]].y - Mesh::meshes[i].uv[Mesh::mesh_indices[i][l]].y;

				float f = 1.0f / (DeltaU1 * DeltaV2 - DeltaU2 * DeltaV1);
		
				glm::vec3 Tangent, Bitangent; 

				Tangent.x = f * (DeltaV2 * Edge1.x - DeltaV1 * Edge2.x);
				Tangent.y = f * (DeltaV2 * Edge1.y - DeltaV1 * Edge2.y);
				Tangent.z = f * (DeltaV2 * Edge1.z - DeltaV1 * Edge2.z);

				Bitangent.x = f * (-DeltaU2 * Edge1.x - DeltaU1 * Edge2.x);
				Bitangent.y = f * (-DeltaU2 * Edge1.y - DeltaU1 * Edge2.y);
				Bitangent.z = f * (-DeltaU2 * Edge1.z - DeltaU1 * Edge2.z);
		
				Mesh::meshes[i].tan.push_back(Tangent);
				Mesh::meshes[i].tan.push_back(Tangent);
				Mesh::meshes[i].tan.push_back(Tangent); 
			}
		
			for (unsigned int m = 0 ; m < Mesh::meshes[i].tan.size()-1 ; m++) 
			{
				Mesh::meshes[i].tan[m] = glm::normalize(Mesh::meshes[i].tan[m]);
			} 
		}
		
		if ((paiMesh->HasBones()) && (paiMesh->mNumBones == iMaxBoneNum ))
		{
			std:: cout << "Reading bone structure..." << std::endl;
			iNumBones = paiMesh->mNumBones;
			for (int j = 0; j <= paiMesh->mNumBones-1; j++)
			{
				boSkeleton[j].strName = paiMesh->mBones[j]->mName.data;
				boSkeleton[j].OffsetMatrix = AiToGlmMatrix(paiMesh->mBones[j]->mOffsetMatrix);
				boSkeleton[j].AnimationTransform = glm::mat4(1.0f);
				boSkeleton[j].parent = NULL;
				boSkeleton[j].TransformationMatrix = AiToGlmMatrix(pScene->mRootNode->FindNode(aiString(boSkeleton[j].strName))->mTransformation);
			}

			for (int j = 0; j <= paiMesh->mNumBones-1; j++)
			{
				aiNode* curNode = pScene->mRootNode->FindNode(aiString(boSkeleton[j].strName));
				aiNode* parentNode = curNode->mParent;
				//Find parent for every bone
				std::string strSearch = parentNode->mName.data;
				for (int m = 0; m <= paiMesh->mNumBones - 1; m++)
					if (strSearch.compare(boSkeleton[m].strName) == 0)
					{
						boSkeleton[j].parent = &(boSkeleton[m]);
						boSkeleton[j].parent->iIndex = m;
					}	

				mBoneTransformationMatrix[j] = boSkeleton[j].TransformationMatrix*boSkeleton[j].OffsetMatrix; 
			}

			//compute bind pose
			for (int k = 0; k <= paiMesh->mNumBones - 1; k++)
			{
				Bone* parent = boSkeleton[k].parent;
				while(parent != NULL)
				{
					mBoneTransformationMatrix[k] = parent->TransformationMatrix*mBoneTransformationMatrix[k];
					parent = parent->parent;
				}
			}

		/*	
			for (int j = 0; j <= paiMesh->mNumBones - 1; j++)
			{
				aiNode* curNode = pScene->mRootNode->FindNode(aiString(boSkeleton[j].strName));
				aiNode* parentNode = curNode->mParent;			
			}	*/

			
				/*
				So ist beispielhaft die Berechnung der einzelnen Transfomrationsmatrizen(kinematische kette von spine bis jaw):
				meshes[i].mBoneTransformationMatrix[0] = boSkeleton[0].TransformationMatrix* boSkeleton[0].OffsetMatrix; spine
				meshes[i].mBoneTransformationMatrix[1] = boSkeleton[0].TransformationMatrix*boSkeleton[1].TransformationMatrix* boSkeleton[1].OffsetMatrix; neck
				meshes[i].mBoneTransformationMatrix[2] = boSkeleton[0].TransformationMatrix*boSkeleton[1].TransformationMatrix* boSkeleton[2
				].TransformationMatrix* boSkeleton[2].OffsetMatrix; head
				meshes[i].mBoneTransformationMatrix[3] = boSkeleton[0].TransformationMatrix*boSkeleton[1].TransformationMatrix* boSkeleton[2].TransformationMatrix*boSkeleton[3].TransformationMatrix* boSkeleton[3].OffsetMatrix; jaw
				*/
		}	
	}

	  boundingBox.fWidth = boundingBox.vTopRightBack.x - boundingBox.vBottomLeftFront.x;
	  boundingBox.fHeight = boundingBox.vTopRightBack.y - boundingBox.vBottomLeftFront.y;
	  boundingBox.fDepth = boundingBox.vTopRightBack.z - boundingBox.vBottomLeftFront.z;
	  //bounding box center
	  boundingBox.vCenterPoint.x = (boundingBox.vTopRightBack.x + boundingBox.vBottomLeftFront.x)/2.0;
	  boundingBox.vCenterPoint.y = (boundingBox.vTopRightBack.y + boundingBox.vBottomLeftFront.y)/2.0;
	  boundingBox.vCenterPoint.z = (boundingBox.vTopRightBack.z + boundingBox.vBottomLeftFront.z)/2.0;

	//Get weights for every vertex in every mesh
	 for (unsigned int i = 0 ; i < pScene->mNumMeshes; i++) 
	 {
		if (pScene->mMeshes[i]->HasBones())
		for (int k = 0; k <= pScene->mMeshes[i]->mNumBones-1; k++)
		{
			for (int m = 0; m < iMaxBoneNum; m++)
				if (boSkeleton[m].strName.compare( pScene->mMeshes[i]->mBones[k]->mName.data)==0)
				{
					for (int j = 0; j <= pScene->mMeshes[i]->mBones[k]->mNumWeights - 1; j++)
					{
						for(int l = 0; l <= MAX_BONE_ID - 1; l++)
						{
							if ((meshes[i].fBoneWeight[int( pScene->mMeshes[i]->mBones[k]->mWeights[j].mVertexId)][l] == 0.0))
							{
								meshes[i].vBoneID[int( pScene->mMeshes[i]->mBones[k]->mWeights[j].mVertexId)][l] = float(m);
								meshes[i].fBoneWeight[int( pScene->mMeshes[i]->mBones[k]->mWeights[j].mVertexId)][l] = pScene->mMeshes[i]->mBones[k]->mWeights[j].mWeight;	
								break;
							}
						}				
					}
				}
		}
	}

	
	if (pScene->HasAnimations())
		{   
			std::cout << "Reading animation data..." << std::endl;
			Mesh::iNumAnimations = pScene->mNumAnimations;
			glm::mat4 temp = glm::mat4(0);
			//Find parent for every bone
			std::string strSearch;
			int iChannel;
			for (int j = 0; j<=pScene->mNumAnimations - 1; j++)
			{
				mAnimations[j].strName = pScene->mAnimations[j]->mName.data;
				mAnimations[j].fDuration = pScene->mAnimations[j]->mDuration;
				mAnimations[j].fTicksPerSecond = pScene->mAnimations[j]->mTicksPerSecond;
				mAnimations[j].iNumChannels = pScene->mAnimations[j]->mNumChannels;
			}

			for(int iAnim = 0; iAnim <= pScene->mNumAnimations - 1; iAnim++)
				 {
					 std::cout << pScene->mAnimations[iAnim]->mName.data << std::endl;
					for (int k = 0; k <=  pScene->mAnimations[iAnim]->mNumChannels - 1; k++)
					{
						strSearch = pScene->mAnimations[iAnim]->mChannels[k]->mNodeName.data;
						for (int j = 0; j <= iNumBones - 1; j++)
						{	
				
							if (strSearch.compare(boSkeleton[j].strName) == 0)
							{					
								iChannel = k;
		
							for (int k = 0; k <= pScene->mAnimations[iAnim]->mChannels[iChannel]->mNumPositionKeys - 1; k++)
							{
								mAnimations[iAnim].mChannels[j].vKeyPositions.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(pScene->mAnimations[iAnim]->mChannels[iChannel]->mPositionKeys[k].mValue.x, pScene->mAnimations[iAnim]->mChannels[iChannel]->mPositionKeys[k].mValue.y, pScene->mAnimations[iAnim]->mChannels[iChannel]->mPositionKeys[k].mValue.z)));
								mAnimations[iAnim].mChannels[j].mKeyRotations.push_back(AiToGlmMatrix(pScene->mAnimations[iAnim]->mChannels[iChannel]->mRotationKeys[k].mValue.GetMatrix()));				
								temp[0][0] = pScene->mAnimations[iAnim]->mChannels[iChannel]->mScalingKeys[k].mValue.x;
								temp[1][1] = pScene->mAnimations[iAnim]->mChannels[iChannel]->mScalingKeys[k].mValue.y;
								temp[2][2] = pScene->mAnimations[iAnim]->mChannels[iChannel]->mScalingKeys[k].mValue.z;
								temp[3][3] = 1.0;
								mAnimations[iAnim].mChannels[j].vKeyScales.push_back(temp);

								//new structure
								mAnimations[iAnim].mChannels[j].v3KeyPositions.push_back(glm::vec3(pScene->mAnimations[iAnim]->mChannels[iChannel]->mPositionKeys[k].mValue.x, pScene->mAnimations[iAnim]->mChannels[iChannel]->mPositionKeys[k].mValue.y, pScene->mAnimations[iAnim]->mChannels[iChannel]->mPositionKeys[k].mValue.z));
								mAnimations[iAnim].mChannels[j].qKeyRotations.push_back(glm::quat(pScene->mAnimations[iAnim]->mChannels[iChannel]->mRotationKeys[k].mValue.w, pScene->mAnimations[iAnim]->mChannels[iChannel]->mRotationKeys[k].mValue.x, pScene->mAnimations[iAnim]->mChannels[iChannel]->mRotationKeys[k].mValue.y, pScene->mAnimations[iAnim]->mChannels[iChannel]->mRotationKeys[k].mValue.z));
								mAnimations[iAnim].mChannels[j].v3KeyScales.push_back(glm::vec3( pScene->mAnimations[iAnim]->mChannels[iChannel]->mScalingKeys[k].mValue.x,  pScene->mAnimations[iAnim]->mChannels[iChannel]->mScalingKeys[k].mValue.y,  pScene->mAnimations[iAnim]->mChannels[iChannel]->mScalingKeys[k].mValue.z));
								mAnimations[iAnim].mChannels[j].fTimeStamp.push_back(pScene->mAnimations[iAnim]->mChannels[iChannel]->mPositionKeys[k].mTime);
							}
							break;
							}
						  
						}
					}
				} 

			} 



	//load the materials with textures
	std::cout << "Reading materials and textures..." << std::endl;

	 for (unsigned int i = 0 ; i < pScene->mNumMaterials; i++)
	 {
        const aiMaterial* pMaterial = pScene->mMaterials[i];
		Material material;
		material.Kd = glm::vec3(1.0);
		material.hasTexture = 0.0;
		material.hasNormalMap = 0.0f;

		aiColor4D diffuse;
		if (AI_SUCCESS == aiGetMaterialColor(pMaterial, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
			material.Kd = glm::vec3(diffuse.r, diffuse.g, diffuse.b);

		 if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) 
		 {
			 aiString Path;

            if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) 
			{
	
                std::string strTexFile = Path.data;
				std::string strTexPath = strFile.substr(0,strFile.find_last_of('/'));
				strTexPath = strTexPath + '/'; 
				strTexPath = strTexPath + strTexFile;
				material.texture_handle = SOIL_load_OGL_texture(strTexPath.c_str(), SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID, SOIL_FLAG_TEXTURE_REPEATS);
				material.hasTexture = 1.0f;	
				
			}

			
			if (pMaterial->GetTexture(aiTextureType_NORMALS, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) 
			{
                std::string strTexFile = Path.data;
				std::string strTexPath = strFile.substr(0,strFile.find_last_of('/'));
				strTexPath = strTexPath + '/'; 
				strTexPath = strTexPath + strTexFile;
				material.normal_map_handle = SOIL_load_OGL_texture(strTexPath.c_str(), SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID, SOIL_FLAG_TEXTURE_REPEATS);
				material.hasNormalMap = 1.0f;
			} 
		 }
		 
		 pMaterial->Get(AI_MATKEY_SHININESS,material.fNs);
		
		 material_list.push_back(material);
	 }

	 //generate Render Buffers
	std::cout << "Generating buffers..." << std::endl;
	for (int i = 0; i <= iNumMeshes - 1; i++)
	{
		glGenBuffers(1, &(Mesh::vbo_pos[i]));
		glGenBuffers(1, &(Mesh::vbo_uv[i]));
		glGenBuffers(1, &(Mesh::vbo_norm[i]));	
		glGenBuffers(1, &(Mesh::vbo_tan[i]));
		glGenBuffers(1, &(Mesh::ibo[i]));
		glGenBuffers(1, &(Mesh::vbo_boneid[i]));
		glGenBuffers(1, &(Mesh::vbo_boneweight[i]));

		glBindBuffer(GL_ARRAY_BUFFER, vbo_pos[i]);
		glBufferData(GL_ARRAY_BUFFER, Mesh::meshes[i].pos.size()*3*sizeof(GLfloat), &(Mesh::meshes[i].pos[0]), GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Mesh::ibo[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh_indices[i].size()*sizeof(int),&(Mesh::mesh_indices[i][0]), GL_STATIC_DRAW);

		if (meshes[i].nor.size() > 0)
		{
			glBindBuffer(GL_ARRAY_BUFFER, vbo_norm[i]);
			glBufferData(GL_ARRAY_BUFFER, Mesh::meshes[i].nor.size()*3*sizeof(GLfloat), &(Mesh::meshes[i].nor[0]), GL_STATIC_DRAW);
		}
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
		if (meshes[i].tan.size() > 0)
		{			
			
			glBindBuffer(GL_ARRAY_BUFFER, vbo_tan[i]);
			glBufferData(GL_ARRAY_BUFFER, Mesh::meshes[i].tan.size()*3*sizeof(GLfloat), &(Mesh::meshes[i].tan[0]), GL_STATIC_DRAW);

		}

	if (meshes[i].uv.size() > 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo_uv[i]);
		glBufferData(GL_ARRAY_BUFFER, Mesh::meshes[i].uv.size()*2*sizeof(GLfloat), &(Mesh::meshes[i].uv[0]), GL_STATIC_DRAW);
	}
	if (meshes[i].vBoneID.size() > 0)
		{	
			glBindBuffer(GL_ARRAY_BUFFER, vbo_boneid[i]);
			glBufferData(GL_ARRAY_BUFFER, Mesh::meshes[i].vBoneID.size()*MAX_BONE_ID*sizeof(GLfloat), &(Mesh::meshes[i].vBoneID[0]), GL_STATIC_DRAW);
		}
	if (meshes[i].fBoneWeight.size() > 0)
		{
			
			glBindBuffer(GL_ARRAY_BUFFER, vbo_boneweight[i]);
			glBufferData(GL_ARRAY_BUFFER, Mesh::meshes[i].fBoneWeight.size()*MAX_BONE_ID*sizeof(GLfloat), &(Mesh::meshes[i].fBoneWeight[0]), GL_STATIC_DRAW);
		}
	}
	std::cout << "Done!" << std::endl;
	std::cout << "Model loaded successfully!" << std::endl;
	Mesh::m_iNumberOfMeshes +=1;
	m_iMeshIndex = Mesh::m_iNumberOfMeshes - 1;
	
	glBindBuffer(GL_ARRAY_BUFFER, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
}

void Mesh::AdjustMeshToCenter()
{
	for (int k = 0; k < iNumMeshes; k++)
		for (int i = 0; i < meshes[k].pos.size(); i++)
			meshes[k].pos[i] -= boundingBox.vCenterPoint;
}

void Mesh::Render(GLuint iShader)
{
	iShaderID = iShader;
	glUseProgram(iShaderID);
    glEnableVertexAttribArray(0);

	//send material independent uniforms to shader
	glUniformMatrix4fv(glGetUniformLocation(iShaderID,"gScale"), 1, GL_FALSE, glm::value_ptr(Mesh::mScalingMatrix));
	glUniformMatrix4fv(glGetUniformLocation(iShaderID,"gRotationX"), 1, GL_FALSE, glm::value_ptr(Mesh::mRotationXMatrix));
	glUniformMatrix4fv(glGetUniformLocation(iShaderID,"gRotationY"), 1, GL_FALSE, glm::value_ptr(Mesh::mRotationYMatrix));
	glUniformMatrix4fv(glGetUniformLocation(iShaderID,"gRotationZ"), 1, GL_FALSE, glm::value_ptr(Mesh::mRotationZMatrix));
	glUniformMatrix4fv(glGetUniformLocation(iShaderID,"gTranslation"), 1, GL_FALSE, glm::value_ptr(Mesh::mTranslationMatrix));

	for (int i = 0; i <= iNumMeshes - 1; i++)
	{
		glUniformMatrix4fv(glGetUniformLocation(iShaderID,"gBoneTransformation"), 100, GL_FALSE, glm::value_ptr(Mesh::mBoneTransformationMatrix[0]));

		glBindBuffer(GL_ARRAY_BUFFER, vbo_pos[i]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		if (meshes[i].nor.size() > 0)
		{
			glEnableVertexAttribArray(2);
			glBindBuffer(GL_ARRAY_BUFFER, vbo_norm[i]);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
		if (meshes[i].tan.size() > 0)
		{			
			glEnableVertexAttribArray(3);
			glBindBuffer(GL_ARRAY_BUFFER, vbo_tan[i]);
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
		if (meshes[i].vBoneID.size() > 0)
		{
			glEnableVertexAttribArray(4);
			glBindBuffer(GL_ARRAY_BUFFER, vbo_boneid[i]);
			glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 0, 0);
		}
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 0, 0);
		if (meshes[i].fBoneWeight.size() > 0)
		{
			glEnableVertexAttribArray(5);
			glBindBuffer(GL_ARRAY_BUFFER, vbo_boneweight[i]);
			
			glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 0, 0);
		}
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 0, 0);
		
		//bind texture if exists
		if (material_list.size() > 0)
		{
			if (material_list[material_index[i]].hasTexture == 1.0)
			{
				
				glEnableVertexAttribArray(1);
				glBindBuffer(GL_ARRAY_BUFFER, vbo_uv[i]);
				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D,Mesh::material_list[material_index[i]].texture_handle);
				glUniform1i(glGetUniformLocation(iShaderID, "Sampler"),0);		
			}
			else 
			{
				glBindTexture(GL_TEXTURE_2D,NULL);
			}

			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
			//bind normal map if exists
			if (material_list[material_index[i]].hasNormalMap == 1.0)
			{
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D,Mesh::material_list[material_index[i]].normal_map_handle);
				glUniform1i(glGetUniformLocation(iShaderID, "Normalmap"),1);
			} 

		}
		else 
		{			
			glBindTexture(GL_TEXTURE_2D,NULL);
		}
		//bind shadow map if exists
		if (shadowmap > 0)
		{
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D,shadowmap);
			glUniform1i(glGetUniformLocation(iShaderID, "Shadowmap"),1);
		}

		//send material dependet uniforms to shader

		if (material_list.size() > 0)
		{
			glUniform1f(glGetUniformLocation(iShaderID, "SpecularPower"),material_list[material_index[i]].fNs);
			glUniform1f(glGetUniformLocation(iShaderID, "HasTexture"),material_list[material_index[i]].hasTexture);	
			glUniform1f(glGetUniformLocation(iShaderID, "HasNormalMap"),material_list[material_index[i]].hasNormalMap);	
			glUniform3fv(glGetUniformLocation(iShaderID, "MeshColor"),1,glm::value_ptr(material_list[material_index[i]].Kd));
		}
		else
		{
			glUniform1f(glGetUniformLocation(iShaderID, "SpecularPower"),0.0f);
			glUniform1f(glGetUniformLocation(iShaderID, "HasTexture"),0.0f);	
			glUniform1f(glGetUniformLocation(iShaderID, "HasNormalMap"),0.0f);	
			glUniform3fv(glGetUniformLocation(iShaderID, "MeshColor"),1,glm::value_ptr(m_DefaultColor));
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Mesh::ibo[i]);
	
		glDrawElements(GL_TRIANGLES,Mesh::mesh_indices[i].size(), GL_UNSIGNED_INT, 0);
	}

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);
	glDisableVertexAttribArray(5);

	glBindBuffer(GL_ARRAY_BUFFER, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);

	glUseProgram(NULL);
} 

void Mesh::Animate(float fTime, int iAnimationIndex)
{   
	if (Mesh::iNumAnimations > 0)
	{
		fAnimationTime += fTime;
		if (fAnimationTime > 1000000.0*mAnimations[iAnimationIndex].fDuration/mAnimations[iAnimationIndex].fTicksPerSecond)
			fAnimationTime -=  1000000.0*mAnimations[iAnimationIndex].fDuration/mAnimations[iAnimationIndex].fTicksPerSecond;

		for (unsigned int i = 0; i <= iNumBones - 1; i++)
		{
			boSkeleton[i].AnimationTransform =  mAnimations[iAnimationIndex].InterpolateKeyFrames(i, (float)m_AnimationTimer.getElapsedTime().asMicroseconds());
			//boSkeleton[i].AnimationTransform =  mAnimations[iAnimationIndex].InterpolateKeyFrames(i, fAnimationTime);
		}

		for (unsigned int i = 0; i <= iNumBones - 1; i++)
		{
			Bone* parent = boSkeleton[i].parent;
			Bone* child = &(boSkeleton[i]);
			boSkeleton[i].child = NULL;
			while(parent != NULL)
			{
				parent->child = child;
				child = parent;
				parent = parent->parent;
			}

			glm::mat4 mSkeletonTransformation = glm::mat4(1.0f);

			while (child != NULL)
			{
				mSkeletonTransformation *= child->AnimationTransform;
				child = child->child;
			}

			mBoneTransformationMatrix[i] = mSkeletonTransformation*boSkeleton[i].OffsetMatrix;
		}
	//keyvalues ersetzen beim animieren die alten Transformationsmatrizen!
		
		//Timer reset and animation restart
		if (m_AnimationTimer.getElapsedTime().asMicroseconds() > 1000000.0*mAnimations[iAnimationIndex].fDuration/mAnimations[iAnimationIndex].fTicksPerSecond)
		{
			m_AnimationTimer.restart();
		}
	}
}

void Mesh::Animate(float fTime, std::string strSubstrName)
{
	int iAnimIndex;
	if (Mesh::iNumAnimations > 0)
	{
		bool bAnimFound = false;
		for (int i = 0; i < iNumAnimations; i++)
		{
			if (mAnimations[i].strName.find(strSubstrName))
			{
				iAnimIndex = i;
				bAnimFound = true;
				break;
			}
		}

		if (!bAnimFound)
		{
			std::cout << "Could not find Animation '" << strSubstrName << "'" << std::endl;
			return;
		}

		fAnimationTime += fTime;

		if (fAnimationTime > 1000000.0*mAnimations[iAnimIndex].fDuration/mAnimations[iAnimIndex].fTicksPerSecond)
			fAnimationTime -=  1000000.0*mAnimations[iAnimIndex].fDuration/mAnimations[iAnimIndex].fTicksPerSecond;

		for (unsigned int i = 0; i <= iNumBones - 1; i++)
		{
			boSkeleton[i].AnimationTransform =  mAnimations[iAnimIndex].InterpolateKeyFrames(i, fAnimationTime);
		}

		for (unsigned int i = 0; i <= iNumBones - 1; i++)
		{
			Bone* parent = boSkeleton[i].parent;
			Bone* child = &(boSkeleton[i]);
			boSkeleton[i].child = NULL;
			while(parent != NULL)
			{
				parent->child = child;
				child = parent;
				parent = parent->parent;
			}

			glm::mat4 mSkeletonTransformation = glm::mat4(1.0f);

			while (child != NULL)
			{
				mSkeletonTransformation *= child->AnimationTransform;
				child = child->child;
			}

			mBoneTransformationMatrix[i] = mSkeletonTransformation*boSkeleton[i].OffsetMatrix;
		}
	}
}

void Mesh::StartAnimationTimer()
{
	m_AnimationTimer.restart();
}

void Mesh::StartAnimationBlendingTimer()
{
	m_AnimationBlendingTimer.restart();
}

//restart timer before blending
bool Mesh::AnimationBlending(int SourceAnimation, int DestinationAnimation)
{
	//restart after 1 second
	if (m_AnimationBlendingTimer.getElapsedTime().asMicroseconds() >= ANIM_BLEND_TIME_MS)
	{
		return true;
	}
	
	//get the current bone transformation matrices from the source animation and interpolate with the first frame of new animation
	glm::vec3 vInterPos;
	glm::vec3 vInterScale;
	glm::quat qInterRot;

	if (m_AnimationTimer.getElapsedTime().asMicroseconds() > 1000000.0*mAnimations[SourceAnimation].fDuration/mAnimations[SourceAnimation].fTicksPerSecond)
	{
		m_AnimationTimer.restart();
	}

	float fFactor = m_AnimationBlendingTimer.getElapsedTime().asMicroseconds()/ANIM_BLEND_TIME_MS;
	
	for (unsigned int iChannel = 0; iChannel <= iNumBones - 1; iChannel++)
	{
		for (unsigned int i = 0; i <= mAnimations[SourceAnimation].mChannels[iChannel].v3KeyPositions.size() - 1; i++)
		{
			
		float fTicksInFloat =  (mAnimations[SourceAnimation].fTicksPerSecond/ANIM_BLEND_TIME_MS)*m_AnimationTimer.getElapsedTime().asMicroseconds();

		   if (mAnimations[SourceAnimation].mChannels[iChannel].fTimeStamp[i] > fTicksInFloat)
			{
				vInterPos = fFactor * mAnimations[DestinationAnimation].mChannels[iChannel].v3KeyPositions[0] + (1 - fFactor)*mAnimations[SourceAnimation].mChannels[iChannel].v3KeyPositions[i-1];
				vInterScale = fFactor * mAnimations[DestinationAnimation].mChannels[iChannel].v3KeyScales[0] + (1 - fFactor)*mAnimations[SourceAnimation].mChannels[iChannel].v3KeyScales[i-1];
				qInterRot = glm::slerp(mAnimations[SourceAnimation].mChannels[iChannel].qKeyRotations[i-1],mAnimations[DestinationAnimation].mChannels[iChannel].qKeyRotations[0],fFactor);
				break;
			}	
		    vInterPos = fFactor * mAnimations[DestinationAnimation].mChannels[iChannel].v3KeyPositions[0] + (1 - fFactor)*mAnimations[SourceAnimation].mChannels[iChannel].v3KeyPositions[mAnimations[SourceAnimation].mChannels[iChannel].v3KeyPositions.size() - 1];
			vInterScale = fFactor * mAnimations[DestinationAnimation].mChannels[iChannel].v3KeyScales[0] + (1 - fFactor)*mAnimations[SourceAnimation].mChannels[iChannel].v3KeyScales[mAnimations[SourceAnimation].mChannels[iChannel].v3KeyPositions.size() - 1];
			qInterRot = glm::slerp(mAnimations[SourceAnimation].mChannels[iChannel].qKeyRotations[mAnimations[SourceAnimation].mChannels[iChannel].v3KeyPositions.size() - 1],mAnimations[DestinationAnimation].mChannels[iChannel].qKeyRotations[0],fFactor);

		}
		boSkeleton[iChannel].AnimationTransform =  glm::translate(glm::mat4(1.0f),vInterPos)*glm::toMat4(qInterRot)*glm::scale(vInterScale);
	
	}

	for (unsigned int i = 0; i <= iNumBones - 1; i++)
	{
			Bone* parent = boSkeleton[i].parent;
			Bone* child = &(boSkeleton[i]);
			boSkeleton[i].child = NULL;
			while(parent != NULL)
			{
				parent->child = child;
				child = parent;
				parent = parent->parent;
			}

			glm::mat4 mSkeletonTransformation = glm::mat4(1.0f);

			while (child != NULL)
			{
				mSkeletonTransformation *= child->AnimationTransform;
				child = child->child;
			}

			mBoneTransformationMatrix[i] = mSkeletonTransformation*boSkeleton[i].OffsetMatrix;
	}


	return false;
}

void Mesh::Scale(float fScale)
{
	Mesh::mScalingMatrix[0][0] = fScale;
	Mesh::mScalingMatrix[1][1] = fScale;
	Mesh::mScalingMatrix[2][2] = fScale;
}

void Mesh::ScaleXZ(float fScale)
{
	Mesh::mScalingMatrix[0][0] = fScale;
	Mesh::mScalingMatrix[1][1] = 1.0;
	Mesh::mScalingMatrix[2][2] = fScale;
}

void Mesh::Rotate(glm::vec3 vRotation)
{
	Mesh::mRotationXMatrix = glm::rotate(glm::mat4(1.0f),vRotation.x, glm::vec3(1, 0, 0) );
	Mesh::mRotationYMatrix = glm::rotate(glm::mat4(1.0f),vRotation.y, glm::vec3(0, 1, 0) );
	Mesh::mRotationZMatrix = glm::rotate(glm::mat4(1.0f),vRotation.z, glm::vec3(0, 0, 1) );

	glm::vec4 temp_direction = glm::vec4(vDirection.x, vDirection.y, vDirection.z, 0);
	temp_direction = mRotationXMatrix*mRotationYMatrix*mRotationZMatrix*temp_direction;
	vDirection = glm::vec3(temp_direction.x, temp_direction.y, temp_direction.z);
}

void Mesh::Rotate(glm::quat qRotation)
{
	glm::quat qRot;
	qRot.w = qRotation.w;
	qRot.x = qRotation.x;
	qRot.y = qRotation.y;
	qRot.z = qRotation.z;
	Mesh::mRotationYMatrix = glm::toMat4(qRot);
	Mesh::mRotationXMatrix = glm::mat4(1.0f);
	Mesh::mRotationZMatrix = glm::mat4(1.0f);
	glm::vec4 temp_direction = glm::vec4(0,0,1, 0);
	glm::vec4 temp_localX = glm::vec4(1,0,0,0);
	temp_localX = mRotationYMatrix*temp_localX;
	temp_direction = mRotationYMatrix*temp_direction;
	vDirection = glm::vec3(temp_direction.x, temp_direction.y, temp_direction.z);
	vLocalX = glm::vec3(temp_localX.x, temp_localX.y, temp_localX.z);
}

//single translation
//moves the mesh to point vTranslation
//use for initial positioning or teleportation ;)
void Mesh::Translate(glm::vec3 vTranslation)
{
	Mesh::vTranslationVector = vTranslation;
	Mesh::mTranslationMatrix = glm::translate(glm::mat4(1.0f),vTranslation);
//	Mesh::BoundingBox.vLeftUpFront += vTranslation;
//	Mesh::BoundingBox.vRightDownBack += vTranslation;
	Mesh::vCenter = boundingBox.vCenterPoint + vTranslation;
}

//continous translation
//moves mesh from last position to vTranslation 
//use for continious movement by a delta
void Mesh::ContTranslate(glm::vec3 vTranslation)
{
	Mesh::mTranslationMatrix = glm::translate(Mesh::mTranslationMatrix,vTranslation);
	Mesh::vTranslationVector += vTranslation;
//	Mesh::BoundingBox.vLeftUpFront += vTranslation;
//	Mesh::BoundingBox.vRightDownBack += vTranslation;
}

void Mesh::ContRotate(glm::vec3 vRotation)
{
	Mesh::mRotationYMatrix = glm::rotate(Mesh::mRotationYMatrix, vRotation.y, glm::vec3(0, 1, 0));
}

void Mesh::ContRotate(glm::quat qRotation)
{
	glm::quat qRot;
	qRot.w = qRotation.w;
	qRot.x = qRotation.x;
	qRot.y = qRotation.y;
	qRot.z = qRotation.z;

	Mesh::mRotationYMatrix = glm::toMat4(qRot);
	Mesh::mRotationXMatrix = glm::mat4(1.0f);
	Mesh::mRotationZMatrix = glm::mat4(1.0f);
	glm::vec4 temp_direction = glm::vec4(vDirection.x, vDirection.y, vDirection.z, 0);
	temp_direction = mRotationYMatrix*temp_direction;
	vDirection = glm::vec3(temp_direction.x, temp_direction.y, temp_direction.z);
}


//***************Set-Functions

void Mesh::setMaterialColor(int iMatIndex, glm::vec3 vColor)
{
	material_list[iMatIndex].Kd = vColor;
}

void Mesh::setShader(GLuint shader_id)
{
	iShaderID = shader_id;
}

void Mesh::setShadowmap(GLuint iShadowmap)
{
	shadowmap = iShadowmap;
}

//***************Get-Functions

glm::mat4 Mesh::getTranslationMatrix()
{
	return Mesh::mTranslationMatrix;
}

glm::mat4 Mesh::getRotationMatrix()
{
	return Mesh::mRotationZMatrix*Mesh::mRotationYMatrix*Mesh::mRotationXMatrix;
}

glm::vec3 Mesh::getDirection()
{
	return Mesh::vDirection;
}

glm::vec3 Mesh::getLocalX()
{
	return Mesh::vLocalX;
}	

GLuint Mesh::getShader()
{
	return iShaderID;	
}

float Mesh::getBoundingBoxWidth()
{
	return Mesh::boundingBox.fWidth;
}

glm::vec3 Mesh::getBoundingCorner1()
{
	return boundingBox.vTopRightBack;
}

glm::vec3 Mesh::getBoundingCorner2()
{
	return boundingBox.vBottomLeftFront;
}

float Mesh::getBoundingBoxHeight()
{
	return boundingBox.fHeight;
}

float Mesh::getBoundingBoxDepth()
{
	return boundingBox.fDepth;
}

glm::vec3 Mesh::getBoundingCenter()
{
	return boundingBox.vCenterPoint;
}

glm::vec3 Mesh::getMaterialColor(int iMatIndex)
{
	return material_list[iMatIndex].Kd;
}

int	Mesh::getNumTriangles()
{
	return mesh_indices[0].size()/3;
}

int	Mesh::getNumVertices()
{
	return meshes[0].pos.size();
}

GLuint* Mesh::getIndexArray()
{
	return &mesh_indices[0][0];
}

glm::vec3*	Mesh::getVertexArray()
{
	return &meshes[0].pos[0];
}

unsigned int Mesh::getNumAnimations()
{
	return iNumAnimations;
}

std::string	Mesh::getAnimationName(int iIndex)
{
	return mAnimations[iIndex].strName;
}

std::string Mesh::getStatus()
{
	return m_strStatus;
}

//***************Has-Functions


//**************is-Functions

bool Mesh::isValid()
{
	return m_bValid;
}

