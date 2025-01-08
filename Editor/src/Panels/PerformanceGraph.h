#pragma once
/*!***************************************************************************
	\file			PerformanceGraph.h
	\author			Chew Shi Ru
	\par			shiru.chew\@digipen.edu
	\date			Oct 1, 2023
	\brief			This header file consists of function declaration to show
					the performance of various system on the editor. 
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include <iostream>
#include <vector>
namespace Editor {

	class PerformanceGraph {

	public:
		/*!***************************************************************************
		*   \brief			Constructor
		*******************************************************************************/
		PerformanceGraph();
		/*!***************************************************************************
		*   \brief			Destructor
		*******************************************************************************/
		~PerformanceGraph();
		/*!***************************************************************************
		*   \brief			To draw and show graph
			\param			nil
			\return			void
		*******************************************************************************/
		void Show();
		/*!***************************************************************************
		*   \brief			Clear Vector
			\param			nil
			\return			void
		*******************************************************************************/
		void ShutDown();
	private:
		 std::vector<float> frames;
		int max_fps = 60;
	};
}
