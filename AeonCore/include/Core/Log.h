#pragma once
/*!***************************************************************************
	\file			Log.h
	\author			Leong Wai Kit
	\par			l.waikit\@digipen.edu
	\date			Oct 1, 2023
	\brief			This source file consists of function declarations for
					logging and initialization of logging functionality.

	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include "Core/AeonAPI.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace AeonCore
{
	class AEON_API log
	{
	public:
		static void init();

		inline static std::shared_ptr <spdlog::logger>& get_core_logger() { return m_core_logger; }
		inline static std::shared_ptr <spdlog::logger>& get_script_logger() { return m_script_logger; }

		static void shutdown();

		static void registerCoreSink(const spdlog::sink_ptr& newSink, const std::string& pattern = "[%T] [Engine]: %v%$");
		static void registerScriptSink(const spdlog::sink_ptr& newSink, const std::string& pattern = "[%T] [Script]: %v%$");
	private:
		inline static std::shared_ptr<spdlog::logger> m_core_logger;
		inline static std::shared_ptr<spdlog::logger> m_script_logger;
	};
}


/*
 *	Different levels for assertion to log (arranged from lowest priority to highest)
 *	Trace: Used for detailed and low-priority informational messages.
 *
 *	Info:  Used for general informational messages about the application's state or events.
 *
 *	Warn:  Used for warning messages indicating potential issues or non-critical problems.
 *
 *	Error: Used for reporting errors or failures that may affect the application's functionality.
 *
 *	Fatal: Used for critical errors that lead to the termination of the application.
 *
 */

 //	Core logger
#define AeonCore_TRACE(...) ::AeonCore::log::get_core_logger()->trace(__VA_ARGS__)
#define AeonCore_INFO(...)  ::AeonCore::log::get_core_logger()->info(__VA_ARGS__)
#define AeonCore_WARN(...)  ::AeonCore::log::get_core_logger()->warn(__VA_ARGS__)
#define AeonCore_ERROR(...) ::AeonCore::log::get_core_logger()->error(__VA_ARGS__)
//#define AeonCore_FATAL(...) ::AeonCore::log::get_core_logger()->fatal(__VA_ARGS__)
#define AeonCore_FATAL(...) ::AeonCore::log::get_core_logger()->critical(__VA_ARGS__)
 //	Assertions
#define AeonCore_ASSERT(x, ...) { if(!(x)) { AeonCore_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();} }

//	Script logger
#define AeonCore_SCRIPT_LOG(...) ::AeonCore::log::get_script_logger()->info(__VA_ARGS__)
#define AeonCore_SCRIPT_WARN(...) ::AeonCore::log::get_script_logger()->warn(__VA_ARGS__)
#define AeonCore_SCRIPT_ERROR(...) ::AeonCore::log::get_script_logger()->error(__VA_ARGS__)


