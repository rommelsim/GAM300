#include "acpch.h"
/*!***************************************************************************
	\file			Layer.cpp
	\author			Leong Wai Kit
	\par			l.waikit\@digipen.edu
	\date			Oct 1, 2023
	\brief			This source file consists of function definitions for
					managing rendering layers.

	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/

#include "Rendering/Layer.h"

namespace AeonCore
{
	/*
	 *	LAYER BASE FUNCTION
	 */
	Layer::Layer(const std::string &_name)
		:mLayerName(_name)
	{
	}

	Layer::~Layer()
	{
	}

	/*
	 *	LAYER STACK FUNCTIONS
	 */
	LayerStack::LayerStack()
	{
		mLayerInsertIndex = 0;
	}

	LayerStack::~LayerStack()
	{
		for (Layer* layer : mLayers)
		{
			layer->OnDetach();
			//delete layer;
		}
	}

	void LayerStack::pushLayer(Layer *_layer)
	{
		mLayers.emplace(mLayers.begin() + mLayerInsertIndex, _layer);
		mLayerInsertIndex++;
	}

	void LayerStack::pushOverlay(Layer *_overlay)
	{
		mLayers.emplace_back(_overlay);
	}

	void LayerStack::popLayer(Layer *_layer)
	{
		auto it = std::find(mLayers.begin(), mLayers.end(), _layer);
		if ( it != mLayers.end() )
		{
			mLayers.erase(it);
			mLayerInsertIndex--;
		}
	}

	void LayerStack::popOverlay(Layer *_overlay)
	{
		auto it = std::find(mLayers.begin(), mLayers.end(), _overlay);
		if ( it != mLayers.end() )
			mLayers.erase(it);
	}
}
