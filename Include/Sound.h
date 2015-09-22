#pragma once

#include "fmod.h"
#include "fmod.hpp"

#pragma comment(lib, "fmodex_vc.lib")

#define MAX_SOUNDS	50

namespace Fugui
{
	class Sound
	{
	public:
		bool init();
		void shutdown();

		int loadSound(const char* soundNme);
		FMOD::Sound* loadStream(const char* streamNme);

		void playStream(FMOD::Sound* streamToPlay);
		void stopStream(FMOD::Sound* streamToStop);
		void playSound(int id);
		inline void update();
		
		Sound();
		~Sound();

	private:
		FMOD::System*		system;
		FMOD::Sound*		sounds[MAX_SOUNDS];
		FMOD_RESULT			result;

		char             name[256];

		unsigned int	version;
		int					numdrivers;
		static int		soundId;

		FMOD_SPEAKERMODE speakermode;
		FMOD_CAPS        caps;

		Sound(const Sound&);
		Sound& operator=(const Sound&);
	};

	void Sound::update() { system->update(); }

}  // end namespace Fugui

extern Fugui::Sound* g_sound;
