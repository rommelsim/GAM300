#pragma once
/*!***************************************************************************
	\file			DebugConsole.h
	\author			Chew Shi Ru
	\par			shiru.chew\@digipen.edu
	\date			Oct 1, 2023
	\brief			This header file consists of function declaration to show 
					debug console on the editor.
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include <spdlog/sinks/base_sink.h>
#include <iostream>
#include <map>
namespace Editor {

	class DebugConsole {

	public:
		/*!***************************************************************************
		*   \brief			Show log message in function declaration
			\param			nil
			\return			log messages to the console
		*******************************************************************************/
		static void Show();
		/*!***************************************************************************
		*   \brief			Push message back
			\param			nil
			\return			m_LogMessage
		*******************************************************************************/
		static void LogMessage(std::string name,std::string message, int messageId);
		static void ShutDown();
		static bool m_Scroll;

	};
	template<typename Mutex>
	class ImGuiSink : public spdlog::sinks::base_sink <Mutex>
	{
		std::string log;
	public:
		void sink_it_(const spdlog::details::log_msg& msg) override
		{	// log_msg is a struct containing the log entry info like level, timestamp, thread id etc.
			// msg.raw contains pre formatted log

			// If needed (very likely but not mandatory), the sink formats the message before sending it to its final destination:
			spdlog::memory_buf_t formatted;
			spdlog::sinks::base_sink<Mutex>::formatter_->format(msg, formatted);
			//Console::LogMessage(std::move(std::make_pair<std::string, int>(fmt::to_string(formatted), msg.level)));
			log = fmt::to_string(formatted);


			//get the name yo 
				// Parse the formatted log message
			std::istringstream iss(log);
			std::string timestamp, logger, level, message;

			iss.ignore(1); // Skip '['
			std::getline(iss, timestamp, ']'); // Read timestamp
			iss.ignore(2); // Skip " ["
			std::getline(iss, logger, ']'); // Read logger name
			//spdlog::string_view_t spdlogStringView = msg.logger_name;
			//std::string messageName(msg.logger_name.begin(), msg.logger_name.end());
			DebugConsole::LogMessage(logger,log, msg.level);
		}

		void flush_() override
		{
			std::cout << std::flush;
		}
	};

	#include "spdlog/details/null_mutex.h"
	#include <mutex>
			using ImGuiSinkMT = ImGuiSink<std::mutex>;
			using ImGuiSinkST = ImGuiSink<spdlog::details::null_mutex>;
}