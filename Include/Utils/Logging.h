/*
 *	Copyright 2026-2027 static - hiro
 *
 *	Use of this source code without explicit permission from owner is strictly prohibited.
 */

#pragma once

#include <string>
#include <fstream>
#include <mutex>
#include <memory>

namespace Utils
{
	enum class LogLevel
	{
		Info,
		Warning,
		Error
	};

	class Log
	{
	public:
		static Log& GetInstance();

		bool Initialize(const std::string& logFilePath = "static.log");
		void Shutdown();

		void Info(const std::string& message);
		void Warning(const std::string& message);
		void Error(const std::string& message);

		void LogMessage(LogLevel level, const std::string& message);

		void SetConsoleOutput(bool enabled) { m_consoleOutput = enabled; }
		bool GetConsoleOutput() const { return m_consoleOutput; }

		void SetFileOutput(bool enabled) { m_fileOutput = enabled; }
		bool GetFileOutput() const { return m_fileOutput; }

	private:
		Log() = default;
		~Log() = default;
		Log(const Log&) = delete;
		Log& operator=(const Log&) = delete;

		void WriteToConsole(LogLevel level, const std::string& message);
		void WriteToFile(LogLevel level, const std::string& message);
		std::string GetTimestamp();
		std::string GetLevelString(LogLevel level);

		std::ofstream m_logFile;
		std::mutex m_mutex;
		bool m_initialized = false;
		bool m_consoleOutput = true;
		bool m_fileOutput = true;
		std::string m_logFilePath;
	};

	#define LOG_INFO(msg) Utils::Log::GetInstance().Info(msg)
	#define LOG_WARNING(msg) Utils::Log::GetInstance().Warning(msg)
	#define LOG_ERROR(msg) Utils::Log::GetInstance().Error(msg)

	#define LOG_INFO_FMT(fmt, ...) Utils::Log::GetInstance().Info(std::format(fmt, __VA_ARGS__))
	#define LOG_WARNING_FMT(fmt, ...) Utils::Log::GetInstance().Warning(std::format(fmt, __VA_ARGS__))
	#define LOG_ERROR_FMT(fmt, ...) Utils::Log::GetInstance().Error(std::format(fmt, __VA_ARGS__))
}

