#include "Animation.h"

Animation::Animation()
{
	mSpeed = 1.0f;
}

glm::mat4 Animation::InterpolateKeyFrames(std::string strChannel, float fTime)
{
    glm::vec3 vInterPos;
	glm::vec3 vInterScale;
	glm::quat qInterRot;
	float fTicksInFloat =  (fTicksPerSecond/1000000.0)*fTime;
	
	int iChannel = 0;
	
	for (int i = 0; i < iNumChannels; i++)
	{
		if (mChannels[i].strName.compare(strChannel) == 0)
		{
			
			iChannel = i;
			break;
		}

		if (i == iNumChannels - 1)
		{
	
			return glm::mat4(1.0f);
		}
	}
	
	//check if time for animation is over then interpolate between last and first frame
	if (fTicksInFloat > mChannels[iChannel].fTimeStamp[mChannels[iChannel].v3KeyPositions.size() - 1])
	{
		
		float fDelta = fTicksInFloat - mChannels[iChannel].fTimeStamp[mChannels[iChannel].v3KeyPositions.size() - 1];
		float fFactor = fDelta/(float(mChannels[iChannel].fTimeStamp[0] - mChannels[iChannel].fTimeStamp[mChannels[iChannel].v3KeyPositions.size() - 1]));
		vInterPos = fFactor * mChannels[iChannel].v3KeyPositions[0] + (1-fFactor)*mChannels[iChannel].v3KeyPositions[mChannels[iChannel].v3KeyPositions.size() - 1];
		vInterScale = fFactor * mChannels[iChannel].v3KeyScales[0] + (1-fFactor)*mChannels[iChannel].v3KeyScales[mChannels[iChannel].v3KeyPositions.size() - 1];
		qInterRot = glm::slerp(mChannels[iChannel].qKeyRotations[mChannels[iChannel].v3KeyPositions.size() - 1],mChannels[iChannel].qKeyRotations[0],fFactor);
	}
	//else interpolate between last and next frame
	else
    for (unsigned int i = 1; i <= mChannels[iChannel].v3KeyPositions.size() - 1; i++)
	{
		
	   if (mChannels[iChannel].fTimeStamp[i] > fTicksInFloat)
		{
		
	
			float fDelta = fTicksInFloat - mChannels[iChannel].fTimeStamp[i-1];
			float fFactor = fDelta/(float(mChannels[iChannel].fTimeStamp[i] - mChannels[iChannel].fTimeStamp[i-1]));
	
			vInterPos = fFactor * mChannels[iChannel].v3KeyPositions[i] + (1-fFactor)*mChannels[iChannel].v3KeyPositions[i-1];
			vInterScale = fFactor * mChannels[iChannel].v3KeyScales[i] + (1-fFactor)*mChannels[iChannel].v3KeyScales[i-1];
			qInterRot = glm::slerp(mChannels[iChannel].qKeyRotations[i-1],mChannels[iChannel].qKeyRotations[i],fFactor);
			break;
		}
	  
	}
	
    return glm::translate(glm::mat4(1.0f),vInterPos)*glm::toMat4(qInterRot)*glm::scale(vInterScale);
}