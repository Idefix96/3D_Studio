#include "main_header.h"
#ifndef ANIMATION_H_
#define ANIMATION_H_
struct Channel
{
	std::string strName;
    std::vector<glm::mat4> vKeyPositions;
    std::vector<glm::mat4> mKeyRotations;    
    std::vector<glm::mat4> vKeyScales;
    std::vector<glm::vec3> v3KeyPositions;
    std::vector<glm::quat> qKeyRotations;    
    std::vector<glm::vec3> v3KeyScales;
    std::vector<double> fTimeStamp;
};
class Animation
{

public:
    unsigned int iID;
    std::string strName;
    int iNumChannels;
    Channel mChannels[150];
    double fDuration;
    double fTicksPerSecond;
    glm::mat4 InterpolateKeyFrames(std::string strChannel, float fTime);
	float mSpeed;
	Animation();


};

#endif
