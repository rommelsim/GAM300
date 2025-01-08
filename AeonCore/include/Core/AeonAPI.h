#pragma once
/*!***************************************************************************
	\file			AeonAPI.h
	\author			Chua Yip Xuan
	\par			yipxuan.chua\@digipen.edu
	\brief			This header file consists of define macros for declspec 
					dllexport 
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/

#ifdef AEON_PLATFORM_WINDOWS
	#ifdef AEON_BUILD_DLL
		#define AEON_API __declspec(dllexport)
	#else 
		#define AEON_API __declspec(dllimport)
	#endif
#else
	#error AeonCore only supports Windows!
#endif

#define BIT(_x) (1 << _x)

#define AEON_BIND_EVENT_FUNC(_func) std::bind(&_func, this, std::placeholders::_1)