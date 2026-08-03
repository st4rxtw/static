/*
 *	Copyright 2026-2027 static - hiro
 *
 *	Use of this source code without explicit permission from owner is strictly prohibited.
 */

#pragma once

#include <string_view>

struct ma_engine;
struct ma_sound;

namespace Base
{
	class AudioEngine
	{
	public:
		AudioEngine() = default;
		~AudioEngine();

		AudioEngine(const AudioEngine&) = delete;
		AudioEngine& operator=(const AudioEngine&) = delete;

		bool Init();
		void Shutdown();

		void PlayOneShot(std::string_view path);

		ma_engine* GetHandle() const { return m_Engine; }

	private:
		ma_engine* m_Engine = nullptr;
	};

	class Sound
	{
	public:
		Sound() = default;
		~Sound();

		Sound(const Sound&) = delete;
		Sound& operator=(const Sound&) = delete;

		bool LoadFromFile(AudioEngine& engine, std::string_view path);
		void Destroy();

		void Play();
		void Pause();
		void Stop();
		void SetLooping(bool looping);
		void SetVolume(float volume);
		bool IsPlaying() const;
		float GetPositionSeconds() const;

	private:
		ma_sound* m_Sound = nullptr;
		void* m_AudioBuffer = nullptr;
	};
}

