/*
 *	Copyright 2026-2027 static - hiro
 *
 *	Use of this source code without explicit permission from owner is strictly prohibited.
 */

#include "Base/Audio.h"

#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

#include <cstdio>
#include <string>

namespace Base
{
	AudioEngine::~AudioEngine()
	{
		Shutdown();
	}

	bool AudioEngine::Init()
	{
		Shutdown();

		m_Engine = new ma_engine{};
		if (ma_engine_init(nullptr, m_Engine) != MA_SUCCESS)
		{
			std::fprintf(stderr, "AudioEngine: ma_engine_init failed\n");
			delete m_Engine;
			m_Engine = nullptr;
			return false;
		}

		return true;
	}

	void AudioEngine::Shutdown()
	{
		if (m_Engine != nullptr)
		{
			ma_engine_uninit(m_Engine);
			delete m_Engine;
			m_Engine = nullptr;
		}
	}

	void AudioEngine::PlaySound(std::string_view path)
	{
		// plays a sound
		ma_engine_play_sound(m_Engine, std::string(path).c_str(), nullptr);
	}

	Sound::~Sound()
	{
		Destroy();
	}

	bool Sound::LoadFromFile(AudioEngine& engine, std::string_view path)
	{
		// loads a sound
		Destroy();

		m_Sound = new ma_sound{};
		if (ma_sound_init_from_file(engine.GetHandle(), std::string(path).c_str(), 0, nullptr, nullptr, m_Sound) != MA_SUCCESS)
		{
			std::fprintf(stderr, "Sound: failed to load \"%s\"\n", std::string(path).c_str());
			delete m_Sound;
			m_Sound = nullptr;
			return false;
		}

		return true;
	}

	void Sound::Destroy()
	{
		// kills the sound
		if (m_Sound != nullptr)
		{
			ma_sound_uninit(m_Sound);
			delete m_Sound;
			m_Sound = nullptr;
		}
	}

	void Sound::Play()
	{
		// plays it
		ma_sound_start(m_Sound);
	}

	void Sound::Pause()
	{
		// pauses the sound
		ma_sound_stop(m_Sound);
	}

	void Sound::Stop()
	{
		// stops the sound
		ma_sound_stop(m_Sound);
		ma_sound_seek_to_pcm_frame(m_Sound, 0);
	}

	void Sound::SetLooping(bool looping)
	{
		// sets it for if it should loop or not
		ma_sound_set_looping(m_Sound, looping ? MA_TRUE : MA_FALSE);
	}

	void Sound::SetVolume(float volume)
	{
		// changes the volume
		ma_sound_set_volume(m_Sound, volume);
	}

	bool Sound::IsPlaying() const
	{
		// bool for if any sound is playing
		return ma_sound_is_playing(m_Sound) == MA_TRUE;
	}
}

