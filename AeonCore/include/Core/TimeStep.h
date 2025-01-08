/*****************************************************************//**
 * \file   TimeStep.h
 * \brief  Contains function to handle internal engine clock. 
 *         Computes DT and Elasped Time.
 *
 *
 * \author Rommel
 * \date   October 2023
 *
 * Copyright (C) 2023 Digipen Insitute Of Technology
 * Reproduction or disclosure of this file or its contents without
 * the prior consent of Digipen Insitute Of Technology is prohibited.
 *********************************************************************/

#ifndef TIMESTEP_H
#define TIMESTEP_H

#include "Core/AeonAPI.h"
#include "Core/Singleton.hpp"
#include "GLFW/glfw3.h"

#include <iostream>
#include <chrono>
#include <Tracy.hpp>

namespace AeonCore
{
    class AEON_API TimerStep : public Singleton<TimerStep>
    {
    public:
        void update() {
           
            ZoneScopedN("Timer Step Update");
        /*    auto now = std::chrono::high_resolution_clock::now();
            deltaTime = std::chrono::duration<float>(now - lastTime).count();
            lastTime = now;
            m_UpdateTimer += deltaTime;
            m_UpdateTimer = std::min(m_UpdateTimer, 0.333f);
            if (m_UpdateTimer > 1.f/ 60.0f) {
               
                g_Config.m_CurrentFrameRate = 1.f / m_UpdateTimer;
                m_UpdateTimer = 0.f;
            }*/
            auto now = std::chrono::high_resolution_clock::now();
            deltaTime = std::chrono::duration<float>(now - lastTime).count();
            lastTime = now;

            // Limit the frame rate to 60 fps
            constexpr float targetFrameRate = 60.0f;
            constexpr float targetFrameTime = 1.0f / targetFrameRate;

            m_UpdateTimer += deltaTime;

            // Ensure that m_UpdateTimer is within a reasonable range
            m_UpdateTimer = std::min(m_UpdateTimer, targetFrameTime * 2.0f);

            // Update frame rate-related variables only when the target frame time is reached
            while (m_UpdateTimer >= targetFrameTime) {
                m_UpdateTimer -= targetFrameTime;
            }
        }

        float elapsed_time() const {
            return std::chrono::duration<float>(std::chrono::high_resolution_clock::now() - startTime).count();
        }

        float get_delta_time() const {
            return deltaTime;
        }

        std::chrono::time_point<std::chrono::high_resolution_clock> get_frame_start_time() const {
	        return lastTime;
        }

        static std::chrono::time_point<std::chrono::high_resolution_clock> get_time_now() {
	        return std::chrono::high_resolution_clock::now();
        }

        static float calc_time_duration(const std::chrono::time_point<std::chrono::high_resolution_clock>& start, const std::chrono::time_point<std::chrono::high_resolution_clock>& end) {
	        return std::chrono::duration<float>(end - start).count();
        }

    private:
        friend class Singleton<TimerStep>;

        TimerStep() : 
            lastTime(std::chrono::high_resolution_clock::now()), 
            deltaTime(0.0f), 
            startTime(std::chrono::high_resolution_clock::now()) {}

        std::chrono::time_point<std::chrono::high_resolution_clock> startTime{};
        std::chrono::time_point<std::chrono::high_resolution_clock> lastTime{};
        float deltaTime = 0.0f;
        float m_UpdateTimer{};
    };


}


#endif // !TIMESTEP_H
