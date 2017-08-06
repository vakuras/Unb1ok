///
/// vAudio Header
///
/// Written by Vadim Kuras. 2010.
///

#ifndef vAudio_H
#define vAudio_H

#include "..\vutil\vutil.h"
#include "Variables.h"
#include "OpenAL\aldlist.h"
#include"alc.h"
#include"efx.h"
#include"efx-creative.h"
#include"xram.h"

// Imported EFX functions
typedef ALboolean (__cdecl *LPEAXSETBUFFERMODE)(ALsizei n, ALuint *buffers, ALint value);
typedef ALenum    (__cdecl *LPEAXGETBUFFERMODE)(ALuint buffer, ALint *value);

namespace vEngine
{
	class vAudio
	{
	public:
		vAudio();
		~vAudio();

		ALDeviceList* GetDeviceList();
		void FreeDeviceList(ALDeviceList** list);

		bool Init(ALuint deviceId);
		bool LoadOgg(const char* fileName, vAudioSource& audioSource, ALenum eXRAMBufferMode);
		bool LoadOgg(HZIP zip, PTCHAR fileName, vAudioSource& audioSource, ALenum eXRAMBufferMode);
		bool LoadOgg(LPVOID ogg, vAudioSource& audioSource, ALenum eXRAMBufferMode);
		void FreeAudioSource(const vAudioSource& audioSource);
		void Play(const vAudioSource& source);
		ALint GetState(const vAudioSource& source);

		bool IsXRAMSupported();
		bool IsEFXSupported();

	protected:
		bool ErrorTest();
		void Clean();

	private:
		bool				m_Inited;
		ALCcontext*			m_pContext;
		ALCdevice*			m_pDevice;
	};
}

#endif