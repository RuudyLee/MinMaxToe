#pragma once
#include <vector>
#include "fmod\fmod.hpp"
#include "fmod\fmod_errors.h"
#include "glm\glm.hpp"

const float DISTANCEFACTOR = 1.0f;          // Units per meter.  I.e feet would = 3.28.  centimeters would = 100.
const float ROLLOFFFACTOR = 1.0f;

typedef FMOD::Sound* SoundClass;

class SoundSystem
{
public:
	SoundSystem();
	~SoundSystem();

	void init();
	void createSound(const char* pFile, int i, glm::vec3 chanPos);
	void update();
	void setListener(glm::vec3 pos, glm::vec3 fwd, glm::vec3 up);
	void setChannelPos(int i, float x, float y, float z);

private:
	void setChannel(SoundClass pSound, int i, glm::vec3 chanPos);

public: // making this public for simplicity
	FMOD::System    *system;
	FMOD::Channel *channels[36];
	FMOD_RESULT      result;
	int              key = 0, numdrivers;
	FMOD_VECTOR      listenerpos;
	unsigned int     version;
	FMOD_SPEAKERMODE speakermode;
	char             name[256];

};