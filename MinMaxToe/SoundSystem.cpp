#include "SoundSystem.h"
#include "Errors.h"

//////////////////////////
//// PUBLIC FUNCTIONS ////
//////////////////////////
SoundSystem::SoundSystem() {
	listenerpos = { 0.0f, 0.0f, 0.0f * DISTANCEFACTOR };
}


SoundSystem::~SoundSystem() {
	result = system->close();
	if (result != FMOD_OK) {
		FatalError(FMOD_ErrorString(result));
	}

	result = system->release();
	if (result != FMOD_OK) {
		FatalError(FMOD_ErrorString(result));
	}
}

void SoundSystem::init() {
	result = FMOD::System_Create(&system);
	if (result != FMOD_OK) {
		FatalError(FMOD_ErrorString(result));
	}

	result = system->getVersion(&version);
	if (result != FMOD_OK) {
		FatalError(FMOD_ErrorString(result));
	}

	if (version < FMOD_VERSION) {
		std::cout << "Error!  You are using an old version of FMOD " << version << "This program requires " << FMOD_VERSION << std::endl;
		exit(1);
	}

	result = system->getNumDrivers(&numdrivers);
	if (result != FMOD_OK) {
		FatalError(FMOD_ErrorString(result));
	}

	if (numdrivers == 0) { 
		result = system->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
		if (result != FMOD_OK) {
			FatalError(FMOD_ErrorString(result));
		}
	}

	result = system->init(100, FMOD_INIT_NORMAL, 0);

	/*
	Set the distance units. (meters/feet etc).
	*/
	result = system->set3DSettings(1.0, DISTANCEFACTOR, ROLLOFFFACTOR);
	if (result != FMOD_OK) {
		FatalError(FMOD_ErrorString(result));
	}
}

void SoundSystem::createSound(const char* pFile, int i, glm::vec3 chanPos) {
	SoundClass sound;
	result = system->createSound(pFile, FMOD_3D, 0, &sound);
	if (result != FMOD_OK) {
		FatalError(FMOD_ErrorString(result));
	}
	
	result = sound->set3DMinMaxDistance(0.02f * DISTANCEFACTOR, 10000.0f * DISTANCEFACTOR);
	if (result != FMOD_OK) {
		FatalError(FMOD_ErrorString(result));
	}

	result = sound->setMode(FMOD_LOOP_NORMAL);
	if (result != FMOD_OK) {
		FatalError(FMOD_ErrorString(result));
	}

	setChannel(sound, i, chanPos);
}

void SoundSystem::update() {
	system->update();
}

void SoundSystem::setListener(glm::vec3 pos, glm::vec3 fwd, glm::vec3 up) {
	FMOD_VECTOR _pos;
	_pos.x = pos.x;
	_pos.y = pos.y;
	_pos.z = pos.z;

	FMOD_VECTOR _fwd;
	_fwd.x = fwd.x;
	_fwd.y = fwd.y;
	_fwd.z = fwd.z;

	FMOD_VECTOR _up;
	_up.x = up.x;
	_up.y = up.y;
	_up.z = up.z;

	system->set3DListenerAttributes(0, &_pos, NULL, &_fwd, &_up);
}

void SoundSystem::setChannelPos(int i, float x, float y, float z) {
	FMOD_VECTOR channelpos;
	channelpos.x = x;
	channelpos.y = y;
	channelpos.z = z;

	result = channels[i]->set3DAttributes(&channelpos, NULL);
	if (result != FMOD_OK) {
		FatalError(FMOD_ErrorString(result));
	}
}

/////////////////////////
/// PRIVATE FUNCTIONS ///
/////////////////////////
void SoundSystem::setChannel(SoundClass pSound, int i, glm::vec3 chanPos) {
	result = system->playSound(pSound, FMOD_DEFAULT, true, &channels[i]);
	if (result != FMOD_OK) {
		FatalError(FMOD_ErrorString(result));
	}

	FMOD_VECTOR channelpos;
	channelpos.x = chanPos.x;
	channelpos.y = chanPos.y;
	channelpos.z = chanPos.z;
	result = channels[i]->set3DAttributes(&channelpos, NULL);
	if (result != FMOD_OK) {
		FatalError(FMOD_ErrorString(result));
	}

	result = channels[i]->setPaused(false);
	if (result != FMOD_OK) {
		FatalError(FMOD_ErrorString(result));
	}

	//channels[i]->setMode(FMOD_3D_LINEARROLLOFF);
}