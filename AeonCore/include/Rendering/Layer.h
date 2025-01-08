#pragma once
/*!***************************************************************************
	\file			Layer.h
	\author			Leong Wai Kit
	\par			l.waikit\@digipen.edu
	\date			Oct 1, 2023
	\brief			This source file consists of function declarations for
					managing rendering layers.

	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include "Core/AeonAPI.h"
#include "Events/Event.h"

namespace AeonCore
{
	class AEON_API Layer
	{
	public:
		Layer(const std::string &_name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& _event) { (void)_event; }
		
		inline const std::string &GetName() const { return mLayerName; }

		// for imgui
		virtual void onRender() {}
	private:
		std::string mLayerName;
	};

	AEON_API class LayerStack
	{ 
	public:
		LayerStack();
		~LayerStack();

		void pushLayer(Layer *_layer);
		void pushOverlay(Layer *_overlay);

		void popLayer(Layer *_layer);
		void popOverlay(Layer *_overlay);

		std::vector<Layer *>::iterator begin() { return mLayers.begin(); }
		std::vector<Layer *>::iterator end() { return mLayers.end(); }

	private:
		std::vector<Layer *> mLayers;
		unsigned int mLayerInsertIndex = 0;
	};
}



