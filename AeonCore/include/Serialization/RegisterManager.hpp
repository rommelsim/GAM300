#pragma once
/*!***************************************************************************
	\file			RegisterManager.h
	\author			Oh Swee Leng Valerie
	\par			o.sweelengvalerie\@digipen.edu
	\date			Oct 25, 2023
	\brief			This header file consists of function declarations to
					RegisterManager.cpp, where the functions will register RTTR
					components in a fixed list.
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/

#include "Core/Log.h"

namespace AeonCore 
{
	class AEON_API RegisterManager
	{
		public:
			static RegisterManager& Instance();
			void AddRegister(std::function<void()> _func, int _list = 0);
			void RegisterComponents();

		private:
			std::vector<std::pair<std::function<void()>, int>> mRegistered;
	};
}