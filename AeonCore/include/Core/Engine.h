#pragma once
/*!***************************************************************************
	\file			Engine.h
	\author			Chua Yip Xuan
	\par			yipxuan.chua\@digipen.edu
	\brief			This header file consists of function declarations of a
					Scene Manager, where the registering of components and
					systems happens here. As well as the init, update and
					destroy.
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include "Core/Singleton.hpp"

namespace AeonCore
{
	class Engine : public Singleton<Engine>
	{
	public:
		/*!***************************************************************************
		*   \brief			Initialise the Engine
			\return			void
		*******************************************************************************/
		void Init();

		/*!***************************************************************************
		*   \brief			Updates the Engine
			\return			void
		*******************************************************************************/
		void Update();

		/*!***************************************************************************
		*   \brief			Destroys the Engine
			\return			void
		*******************************************************************************/
		void Destroy();
	private:
		friend class Singleton<Engine>;
	};
}