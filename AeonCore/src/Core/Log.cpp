#include "acpch.h"
/*!***************************************************************************
	\file			Log.cpp
	\author			Leong Wai Kit
	\par			l.waikit\@digipen.edu
	\date			Oct 1, 2023
	\brief			This source file consists of function definitions for
					logging and initialization of logging functionality.

	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/

#include "Core/Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace AeonCore
{
	void log::init()
	{
		// Initialize the core logger
		spdlog::set_pattern("%^[%T] %n: %v%$");
		m_core_logger = spdlog::stdout_color_mt("AeonCore");

		// Initialize the script logger
		m_script_logger = spdlog::stdout_color_mt("AeonScript");
	}

	void log::shutdown()
	{
		spdlog::shutdown();
	}

	void log::registerCoreSink(const spdlog::sink_ptr& newSink, const std::string& pattern) {
		newSink->set_pattern(pattern);
		m_core_logger->sinks().emplace_back(newSink);
	}

	void log::registerScriptSink(const spdlog::sink_ptr& newSink, const std::string& pattern)
	{
		newSink->set_pattern(pattern);
		m_script_logger->sinks().emplace_back(newSink);
	}
}

