/*
 *	Copyright 2026-2027 static - hiro
 *
 *	Use of this source code without explicit permission from owner is strictly prohibited.
 */

#include "Base/Audio.h"
#include "Utils/Paths.h"

#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

#define STB_VORBIS_HEADER_ONLY
#include <stb_vorbis.c>

#include <cstdio>
#include <cstdlib>
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
		ma_engine_play_sound(m_Engine, Utils::Paths::Resolve(path).c_str(), nullptr);
	}

	Sound::~Sound()
	{
		Destroy();
	}

	bool Sound::LoadFromFile(AudioEngine& engine, std::string_view path)
	{
		// loads a sound
		Destroy();

		std::string resolvedPath = Utils::Paths::Resolve(path);

		m_Sound = new ma_sound{};
		if (ma_sound_init_from_file(engine.GetHandle(), resolvedPath.c_str(), 0, nullptr, nullptr, m_Sound) == MA_SUCCESS)
		{
			return true;
		}

		delete m_Sound;
		m_Sound = nullptr;

		int channels = 0;
		int sampleRate = 0;
		short* pcm = nullptr;
		int frameCount = stb_vorbis_decode_filename(resolvedPath.c_str(), &channels, &sampleRate, &pcm);
		if (frameCount <= 0)
		{
			std::fprintf(stderr, "Sound: failed to load \"%s\"\n", resolvedPath.c_str());
			return false;
		}

		ma_audio_buffer_config config = ma_audio_buffer_config_init(ma_format_s16, static_cast<ma_uint32>(channels), static_cast<ma_uint64>(frameCount), pcm, nullptr);
		config.sampleRate = static_cast<ma_uint32>(sampleRate);

		auto* audioBuffer = new ma_audio_buffer{};
		ma_result bufferResult = ma_audio_buffer_init_copy(&config, audioBuffer);
		std::free(pcm);

		if (bufferResult != MA_SUCCESS)
		{
			std::fprintf(stderr, "Sound: failed to buffer \"%s\"\n", resolvedPath.c_str());
			delete audioBuffer;
			return false;
		}

		m_AudioBuffer = audioBuffer;

		m_Sound = new ma_sound{};
		if (ma_sound_init_from_data_source(engine.GetHandle(), reinterpret_cast<ma_data_source*>(&audioBuffer->ref), 0, nullptr, m_Sound) != MA_SUCCESS)
		{
			std::fprintf(stderr, "Sound: failed to init sound from \"%s\"\n", resolvedPath.c_str());
			Destroy();
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

		if (m_AudioBuffer != nullptr)
		{
			auto* audioBuffer = static_cast<ma_audio_buffer*>(m_AudioBuffer);
			ma_audio_buffer_uninit(audioBuffer);
			delete audioBuffer;
			m_AudioBuffer = nullptr;
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

