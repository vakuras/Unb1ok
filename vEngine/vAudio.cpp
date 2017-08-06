///
/// vAudio Implementation
///
/// Written by Vadim Kuras. 2010.
///

#pragma comment(lib, "OpenAL32.lib")

#include "vAudio.h"
#include <vorbis\vorbisenc.h>
#include <vorbis\vorbisfile.h>
#include <vorbis\codec.h>
#include "vOggMemory.h"

LPALGENEFFECTS alGenEffects = NULL;
LPALDELETEEFFECTS alDeleteEffects = NULL;
LPALISEFFECT alIsEffect = NULL;
LPALEFFECTI alEffecti = NULL;
LPALEFFECTIV alEffectiv = NULL;
LPALEFFECTF alEffectf = NULL;
LPALEFFECTFV alEffectfv = NULL;
LPALGETEFFECTI alGetEffecti = NULL;
LPALGETEFFECTIV alGetEffectiv = NULL;
LPALGETEFFECTF alGetEffectf = NULL;
LPALGETEFFECTFV alGetEffectfv = NULL;

//Filter objects
LPALGENFILTERS alGenFilters = NULL;
LPALDELETEFILTERS alDeleteFilters = NULL;
LPALISFILTER alIsFilter = NULL;
LPALFILTERI alFilteri = NULL;
LPALFILTERIV alFilteriv = NULL;
LPALFILTERF alFilterf = NULL;
LPALFILTERFV alFilterfv = NULL;
LPALGETFILTERI alGetFilteri = NULL;
LPALGETFILTERIV alGetFilteriv = NULL;
LPALGETFILTERF alGetFilterf = NULL;
LPALGETFILTERFV alGetFilterfv = NULL;

// Auxiliary slot object
LPALGENAUXILIARYEFFECTSLOTS alGenAuxiliaryEffectSlots = NULL;
LPALDELETEAUXILIARYEFFECTSLOTS alDeleteAuxiliaryEffectSlots = NULL;
LPALISAUXILIARYEFFECTSLOT alIsAuxiliaryEffectSlot = NULL;
LPALAUXILIARYEFFECTSLOTI alAuxiliaryEffectSloti = NULL;
LPALAUXILIARYEFFECTSLOTIV alAuxiliaryEffectSlotiv = NULL;
LPALAUXILIARYEFFECTSLOTF alAuxiliaryEffectSlotf = NULL;
LPALAUXILIARYEFFECTSLOTFV alAuxiliaryEffectSlotfv = NULL;
LPALGETAUXILIARYEFFECTSLOTI alGetAuxiliaryEffectSloti = NULL;
LPALGETAUXILIARYEFFECTSLOTIV alGetAuxiliaryEffectSlotiv = NULL;
LPALGETAUXILIARYEFFECTSLOTF alGetAuxiliaryEffectSlotf = NULL;
LPALGETAUXILIARYEFFECTSLOTFV alGetAuxiliaryEffectSlotfv = NULL;

// XRAM functions and enum values

LPEAXSETBUFFERMODE eaxSetBufferMode = NULL;
LPEAXGETBUFFERMODE eaxGetBufferMode = NULL;

ALenum eXRAMSize = 0;
ALenum eXRAMFree = 0;
ALenum eXRAMAuto = 0;
ALenum eXRAMHardware = 0;
ALenum eXRAMAccessible = 0;

#define BUFFER_SIZE (4096 * 8)

namespace vEngine
{
	vAudio::vAudio()
	{
		m_Inited = false;
	}

	ALDeviceList* vAudio::GetDeviceList()
	{
		return new ALDeviceList();
	}

	void vAudio::FreeDeviceList(ALDeviceList** list)
	{
		delete *list;
		*list = NULL;
	}

	bool vAudio::Init(ALuint deviceId)
	{
		Trace(TEXT("Begin"));
		
		ALDeviceList* pDeviceList = new ALDeviceList();

		if (!pDeviceList)
		{
			Trace(TEXT("Unable to create device list"));
			return false;
		}

		if (!pDeviceList->GetNumDevices() || (deviceId != 0 && deviceId > pDeviceList->GetNumDevices()))
		{
			Trace(TEXT("No audio devices available"));
			delete pDeviceList;
			return false;
		}

		if (deviceId == 0)
			deviceId = pDeviceList->GetDefaultDevice();

		m_pDevice = alcOpenDevice(pDeviceList->GetDeviceName(deviceId));

		if (!m_pDevice)
		{
			Trace(TEXT("Unable to open device id:[%d]"), deviceId);
			delete pDeviceList;
			return false;
		}

		delete pDeviceList;

		m_pContext = alcCreateContext(m_pDevice, NULL);

		if (!m_pContext)
		{
			Trace(TEXT("Unable to allocate context"));
			Clean();
			return false;
		}

		if (!alcMakeContextCurrent(m_pContext))
		{
			Trace(TEXT("Unable to set context"));
			Clean();
			return false;
		}

		m_Inited = true;			

		Trace(TEXT("End"));

		return true;
	}

	void vAudio::Clean()
	{
		alcMakeContextCurrent(NULL);

		if (m_pContext)
		{
			alcDestroyContext(m_pContext);
			m_pContext = NULL;
		}

		if (m_pDevice)
		{
			alcCloseDevice(m_pDevice);
			m_pDevice = NULL;
		}
	}

	bool vAudio::IsXRAMSupported()
	{
		if (!m_Inited)
			return false;

		bool bXRAM = false;
	
		if (alIsExtensionPresent("EAX-RAM") == AL_TRUE)
		{
			// Get X-RAM Function pointers
			eaxSetBufferMode = (EAXSetBufferMode)alGetProcAddress("EAXSetBufferMode");
			eaxGetBufferMode = (EAXGetBufferMode)alGetProcAddress("EAXGetBufferMode");

			if (eaxSetBufferMode && eaxGetBufferMode)
			{
				eXRAMSize = alGetEnumValue("AL_EAX_RAM_SIZE");
				eXRAMFree = alGetEnumValue("AL_EAX_RAM_FREE");
				eXRAMAuto = alGetEnumValue("AL_STORAGE_AUTOMATIC");
				eXRAMHardware = alGetEnumValue("AL_STORAGE_HARDWARE");
				eXRAMAccessible = alGetEnumValue("AL_STORAGE_ACCESSIBLE");

				if (eXRAMSize && eXRAMFree && eXRAMAuto && eXRAMHardware && eXRAMAccessible)
					bXRAM = true;
			}
		}

		return bXRAM;
	}

	bool vAudio::IsEFXSupported()
	{
		if (!m_Inited)
			return false;

		bool bEFXSupport = false;

		if (alcIsExtensionPresent(m_pDevice, (ALCchar*)ALC_EXT_EFX_NAME))
		{
			// Get function pointers
			alGenEffects = (LPALGENEFFECTS)alGetProcAddress("alGenEffects");
			alDeleteEffects = (LPALDELETEEFFECTS )alGetProcAddress("alDeleteEffects");
			alIsEffect = (LPALISEFFECT )alGetProcAddress("alIsEffect");
			alEffecti = (LPALEFFECTI)alGetProcAddress("alEffecti");
			alEffectiv = (LPALEFFECTIV)alGetProcAddress("alEffectiv");
			alEffectf = (LPALEFFECTF)alGetProcAddress("alEffectf");
			alEffectfv = (LPALEFFECTFV)alGetProcAddress("alEffectfv");
			alGetEffecti = (LPALGETEFFECTI)alGetProcAddress("alGetEffecti");
			alGetEffectiv = (LPALGETEFFECTIV)alGetProcAddress("alGetEffectiv");
			alGetEffectf = (LPALGETEFFECTF)alGetProcAddress("alGetEffectf");
			alGetEffectfv = (LPALGETEFFECTFV)alGetProcAddress("alGetEffectfv");
			alGenFilters = (LPALGENFILTERS)alGetProcAddress("alGenFilters");
			alDeleteFilters = (LPALDELETEFILTERS)alGetProcAddress("alDeleteFilters");
			alIsFilter = (LPALISFILTER)alGetProcAddress("alIsFilter");
			alFilteri = (LPALFILTERI)alGetProcAddress("alFilteri");
			alFilteriv = (LPALFILTERIV)alGetProcAddress("alFilteriv");
			alFilterf = (LPALFILTERF)alGetProcAddress("alFilterf");
			alFilterfv = (LPALFILTERFV)alGetProcAddress("alFilterfv");
			alGetFilteri = (LPALGETFILTERI )alGetProcAddress("alGetFilteri");
			alGetFilteriv= (LPALGETFILTERIV )alGetProcAddress("alGetFilteriv");
			alGetFilterf = (LPALGETFILTERF )alGetProcAddress("alGetFilterf");
			alGetFilterfv= (LPALGETFILTERFV )alGetProcAddress("alGetFilterfv");
			alGenAuxiliaryEffectSlots = (LPALGENAUXILIARYEFFECTSLOTS)alGetProcAddress("alGenAuxiliaryEffectSlots");
			alDeleteAuxiliaryEffectSlots = (LPALDELETEAUXILIARYEFFECTSLOTS)alGetProcAddress("alDeleteAuxiliaryEffectSlots");
			alIsAuxiliaryEffectSlot = (LPALISAUXILIARYEFFECTSLOT)alGetProcAddress("alIsAuxiliaryEffectSlot");
			alAuxiliaryEffectSloti = (LPALAUXILIARYEFFECTSLOTI)alGetProcAddress("alAuxiliaryEffectSloti");
			alAuxiliaryEffectSlotiv = (LPALAUXILIARYEFFECTSLOTIV)alGetProcAddress("alAuxiliaryEffectSlotiv");
			alAuxiliaryEffectSlotf = (LPALAUXILIARYEFFECTSLOTF)alGetProcAddress("alAuxiliaryEffectSlotf");
			alAuxiliaryEffectSlotfv = (LPALAUXILIARYEFFECTSLOTFV)alGetProcAddress("alAuxiliaryEffectSlotfv");
			alGetAuxiliaryEffectSloti = (LPALGETAUXILIARYEFFECTSLOTI)alGetProcAddress("alGetAuxiliaryEffectSloti");
			alGetAuxiliaryEffectSlotiv = (LPALGETAUXILIARYEFFECTSLOTIV)alGetProcAddress("alGetAuxiliaryEffectSlotiv");
			alGetAuxiliaryEffectSlotf = (LPALGETAUXILIARYEFFECTSLOTF)alGetProcAddress("alGetAuxiliaryEffectSlotf");
			alGetAuxiliaryEffectSlotfv = (LPALGETAUXILIARYEFFECTSLOTFV)alGetProcAddress("alGetAuxiliaryEffectSlotfv");

			if (alGenEffects &&	alDeleteEffects && alIsEffect && alEffecti && alEffectiv &&	alEffectf &&
				alEffectfv && alGetEffecti && alGetEffectiv && alGetEffectf && alGetEffectfv &&	alGenFilters &&
				alDeleteFilters && alIsFilter && alFilteri && alFilteriv &&	alFilterf && alFilterfv &&
				alGetFilteri &&	alGetFilteriv && alGetFilterf && alGetFilterfv && alGenAuxiliaryEffectSlots &&
				alDeleteAuxiliaryEffectSlots &&	alIsAuxiliaryEffectSlot && alAuxiliaryEffectSloti &&
				alAuxiliaryEffectSlotiv && alAuxiliaryEffectSlotf && alAuxiliaryEffectSlotfv &&
				alGetAuxiliaryEffectSloti && alGetAuxiliaryEffectSlotiv && alGetAuxiliaryEffectSlotf &&
				alGetAuxiliaryEffectSlotfv)
				bEFXSupport = true;
		}

		return bEFXSupport;
	}

	vAudio::~vAudio()
	{
		__try
		{
			Trace(TEXT("Begin"));
		
			Clean();
		}
		__finally
		{
			Trace(TEXT("End"));
		}
	}

	void vAudio::Play(const vAudioSource& source)
	{
		alSourceStop(source.source);
		alSourceUnqueueBuffers(source.source, source.bufferCount, source.buffers);
		alSourceQueueBuffers(source.source, source.bufferCount, source.buffers);
		alSourcePlay(source.source);
	}

	void vAudio::FreeAudioSource(const vAudioSource& audioSource)
	{
		alSourceStop(audioSource.source);

		alDeleteSources(1, &audioSource.source);
		alDeleteBuffers(audioSource.bufferCount, audioSource.buffers);
	}

	ALint vAudio::GetState(const vAudioSource& audioSource)
	{
		ALint iState;
		alGetSourcei(audioSource.source, AL_SOURCE_STATE, &iState);
		return iState;
	}

	bool vAudio::ErrorTest()
	{
		int error = alGetError();

		if (error == AL_NO_ERROR)
			return false;

		TraceErrWithCode(error, TEXT("OpenAL error [%d]"));
		return true;
	}

	bool vAudio::LoadOgg(HZIP zip, PTCHAR fileName, vAudioSource& audioSource, ALenum eXRAMBufferMode)
	{
		LPVOID				buffer;
		LONG				size;
		OggVorbis_File		ogg;
		ZRESULT				result = vutil::UnZip(zip, &buffer, fileName, &size);
		ov_callbacks		callbacks;
		OGGBUFFER			oggBuffer;
		
		Trace(TEXT("Begin"));

		if (result != ZR_OK)
		{
			TraceErrWithCode(result, TEXT("vutil::UnZip failed, error [%d]"));
			return false;
		}

		__try
		{
			oggBuffer.DeleteOnClose = FALSE;
			oggBuffer.Read = 0;
			oggBuffer.Size = size;
			oggBuffer.pBuffer = buffer;
			callbacks.close_func = oggbuffer_close;
			callbacks.read_func = oggbuffer_read;
			callbacks.seek_func = oggbuffer_seek;
			callbacks.tell_func = oggbuffer_tell;

			if (ov_open_callbacks(&oggBuffer, &ogg, NULL, 0, callbacks))
				return false;

			return LoadOgg(&ogg, audioSource, eXRAMBufferMode);
		}
		__finally
		{
			vutil::FreeBuffer(&buffer);
			Trace(TEXT("End"));
		}		
	}

	bool vAudio::LoadOgg(const char * fileName, vAudioSource& audioSource, ALenum eXRAMBufferMode)
	{
		__try
		{
			Trace(TEXT("Begin"));

			OggVorbis_File ogg;

			if (ov_fopen(fileName, &ogg))
				return false;

			return LoadOgg(&ogg, audioSource, eXRAMBufferMode);
		}
		__finally
		{
			Trace(TEXT("End"));
		}
	}

	bool vAudio::LoadOgg(LPVOID ogg, vAudioSource& audioSource, ALenum eXRAMBufferMode)
	{
		ALenum				format;
		int					error;
		int					section;
		long				result;
		long				size = 0;
		CHAR				buffer[4096];
		PCHAR				pDecoded = NULL;
		OggVorbis_File*		pOgg = (OggVorbis_File*)ogg;

		__try
		{
			Trace(TEXT("Begin"));

			if(pOgg->vi->channels == 1)
				format = AL_FORMAT_MONO16;
			else
				format = AL_FORMAT_STEREO16;

			ZeroMemory(&audioSource, sizeof(vAudioSource));
			audioSource.bufferCount = 1;
			audioSource.buffers = new UINT[1];

			alGenBuffers(1, audioSource.buffers);
			if (ErrorTest())
				return false;

			alGenSources(1, &audioSource.source);
			if (ErrorTest())
				return false;		
			
			while((result = ov_read(pOgg, buffer, 4096, 0, 2, 1, &section)) > 0)
			{
				if (result < 0)
					return false;

				size += result;
				pDecoded = (PCHAR)realloc(pDecoded, size);
				memcpy(pDecoded + size - result, buffer, result);
			}

			if (size == 0)
				return false;

			alBufferData(audioSource.buffers[0], format, pDecoded, size, pOgg->vi->rate);
			if (ErrorTest())
				return false;

			return true;
		}
		__finally
		{
			if (pDecoded)
				free(pDecoded);

			ov_clear(pOgg);

			Trace(TEXT("End"));
		}
	}
}
