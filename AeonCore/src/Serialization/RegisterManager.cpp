#include "acpch.h"
/*!***************************************************************************
	\file			RegisterManager.cpp
	\author			Oh Swee Leng Valerie
	\par			o.sweelengvalerie\@digipen.edu
	\date			Oct 26, 2023
	\brief			This source file consists of function definitions to
					register RTTR components in a fixed list.
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include "Serialization/RegisterManager.hpp"

namespace AeonCore
{
	RegisterManager& RegisterManager::Instance()
	{
		static RegisterManager instance;
		return instance;
	}

	/*
	* Adds each component to be registered with RTTR in a fixed priority
	*/
	void RegisterManager::AddRegister(std::function<void()> _func, int _list)
	{
		mRegistered.push_back({ _func, _list });
	}

	/*
	* Execute RTTR registration
	*/
	void RegisterManager::RegisterComponents()
	{
		std::sort(mRegistered.begin(), mRegistered.end(),
			[](const auto& a, const auto& b) {
				return a.second < b.second;
			});
		for (const auto& [_func, _] : mRegistered)
		{
			_func();
		}
	}
}