/*
 *	Copyright 2026-2027 static - hiro
 *
 *	Use of this source code without explicit permission from owner is strictly prohibited.
 */

#include "Utils/Logging.h"

#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>

#ifdef _WIN32
#include <windows.h>
#endif

namespace Utils
{
	Log& Log::GetInstance()
	{
		static Log instance;
		return instance;
	}

	bool Log::Initialize(const std::string& logFilePath)
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		if (m_initialized)
		{
			return true;
		}

		m_logFilePath = logFilePath;

		if (m_fileOutput)
		{
			m_logFile.open(logFilePath, std::ios::app);
			if (!m_logFile.is_open())
			{
				std::cerr << "Failed to open log file: " << logFilePath << std::endl;
				return false;
			}
		}

		m_initialized = true;
		return true;
	}

	void Log::Shutdown()
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		if (m_initialized)
		{
			Info("Log system shutting down");
			if (m_logFile.is_open())
			{
				m_logFile.close();
			}
			m_initialized = false;
		}
	}

	void Log::Info(const std::string& message)
	{
		// log an info message
		LogMessage(LogLevel::Info, message);
	}

	void Log::Warning(const std::string& message)
	{
		// log a warning
		LogMessage(LogLevel::Warning, message);
	}

	void Log::Error(const std::string& message)
	{
		// log an error
		LogMessage(LogLevel::Error, message);
	}

	void Log::LogMessage(LogLevel level, const std::string& message)
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		if (!m_initialized)
		{
			std::cerr << "[" << GetLevelString(level) << "] " << message << std::endl;
			return;
		}

		if (m_consoleOutput)
		{
			WriteToConsole(level, message);
		}

		if (m_fileOutput && m_logFile.is_open())
		{
			WriteToFile(level, message);
		}
	}

	void Log::WriteToConsole(LogLevel level, const std::string& message)
	{
		std::string timestamp = GetTimestamp();
		std::string levelStr = GetLevelString(level);

#ifdef _WIN32
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		WORD originalColor;
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(hConsole, &csbi);
		originalColor = csbi.wAttributes;

		switch (level)
		{
			case LogLevel::Info:
				SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
				break;
			case LogLevel::Warning:
				SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN);
				break;
			case LogLevel::Error:
				SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
				break;
		}

		std::cout << "[" << timestamp << "] [" << levelStr << "] " << message << std::endl;

		SetConsoleTextAttribute(hConsole, originalColor);
#else
		std::string colorCode;
		switch (level)
		{
			case LogLevel::Info:
				colorCode = "\033[37m";
				break;
			case LogLevel::Warning:
				colorCode = "\033[33m";
				break;
			case LogLevel::Error:
				colorCode = "\033[31m";
				break;
		}

		std::cout << colorCode << "[" << timestamp << "] [" << levelStr << "] " << message << "\033[0m" << std::endl;
#endif
	}

	void Log::WriteToFile(LogLevel level, const std::string& message)
	{
		std::string timestamp = GetTimestamp();
		std::string levelStr = GetLevelString(level);

		m_logFile << "[" << timestamp << "] [" << levelStr << "] " << message << std::endl;
		m_logFile.flush();
	}

	std::string Log::GetTimestamp()
	{
		auto now = std::chrono::system_clock::now();
		auto time_t = std::chrono::system_clock::to_time_t(now);
		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
			now.time_since_epoch()) % 1000;

		std::stringstream ss;

#ifdef _WIN32
		struct tm timeinfo;
		localtime_s(&timeinfo, &time_t);
		ss << std::put_time(&timeinfo, "%Y-%m-%d %H:%M:%S");
#else
		ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
#endif

		ss << '.' << std::setfill('0') << std::setw(3) << ms.count();

		return ss.str();
	}

	std::string Log::GetLevelString(LogLevel level)
	{
		switch (level)
		{
			case LogLevel::Info:
				return "INFO";
			case LogLevel::Warning:
				return "WARN";
			case LogLevel::Error:
				return "ERROR";
			default:
				return "UNKNOWN";
		}
	}
}

