#include "acpch.h"
/*!***************************************************************************
	\file			Render2DSystem.cpp
	\author			Rommel Sim
	\par			zhenpengrommel.sim@digipen.edu
	\date			Oct 1, 2023
	\brief			This header file consists of the function declarations to
					create and manage UI Component for rendering
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include "ECS/Systems/Renderer2DSystem.h"

#include "Core/Application.h"


namespace AeonCore
{
	std::map<UI::Layer, Renderer2DSystem::Interface> Renderer2DSystem::m_Layers;

	void Renderer2DSystem::Init()
	{
		AeonCore_INFO("Sprite Renderer2D System Startup OK");
	}

	void Renderer2DSystem::Update()
	{
		ZoneScopedN("Renderer 2D Update");
		// for all my entities
		// 	get the UI component
		// 		check if Layers have this inside Interfaces

		/*if (Application::GetApplication().GetLoadStatus() != LoadStatus::NotRunning)
			return;*/

		Coordinator& coord = Coordinator::GetInstance();
		for (EntityID Id : mEntities)
		{
			//if (coord.HasComponent<UI>(e))
			{
				UI& UIComponent = coord.GetComponent<UI>(Id);
				UI::Layer _layer = UIComponent.GetLayer();
				//EntityID Id = e.GetEntityID();
				UI::Order order = UIComponent.GetOrder(); (void)order;

				// If Layers contains _layer 
				auto _LayerPtr = m_Layers.find(_layer);
				if (_LayerPtr != m_Layers.end())
				{
					auto find = _LayerPtr->second.find(Id);
					if (find != _LayerPtr->second.end())
					{
						//AeonCore_INFO("Ent Found, skipping");
					}
					else
					{
						_LayerPtr->second.insert({ Id, &UIComponent });
						//AeonCore_INFO("Ent Not Found, adding now");
					}

					auto newInterface = Sort(_layer, _LayerPtr->second);
					_LayerPtr->second.clear();
					_LayerPtr->second = newInterface;
				}
				else
				{
					Interface newInterface;
					newInterface.insert({ Id, &UIComponent });
					m_Layers.insert({ _layer, newInterface });
					AeonCore_INFO("New Layer and Interface added!");
				}

				
			}
		}
	}

	void Renderer2DSystem::Destroy()
	{
		m_Layers.clear();
	}

	void Renderer2DSystem::UpdateLayer(EntityID id, UI* first, UI::Layer _newLayer)
	{
		auto layer = first->GetLayer();
		auto find = m_Layers.find(layer);
		if (find != m_Layers.end())
		{
			auto foundID = find->second.find(id);
			if (foundID != find->second.end())
			{
				find->second.erase(id);
			}
		}

		first->SetLayer(_newLayer);
		find = m_Layers.find(_newLayer);
		if (find != m_Layers.end())
		{
			find->second.emplace(id, first);
		}
	}

	void Renderer2DSystem::RemoveLayer(UI* ui)
	{
		auto EntID = ui->GetEntityID();
		auto layer = ui->GetLayer();

		auto find = m_Layers.find(layer);
		if (find != m_Layers.end())
		{
			find->second.erase(EntID);
		}
	}

	std::map<UI::Layer, Renderer2DSystem::Interface>& Renderer2DSystem::GetLayers()
	{
		return m_Layers;
	}

	bool Renderer2DSystem::Comparator(std::pair<EntityID, UI*>& left, std::pair<EntityID, UI*>& right)
	{
		return left.second->GetOrder() < right.second->GetOrder();
	}

	Renderer2DSystem::Interface Renderer2DSystem::Sort(UI::Layer _layer, Interface& _in)
	{
		std::vector<std::pair<EntityID, UI*>> vec(_in.begin(), _in.end());
		std::sort(vec.begin(), vec.end(), Comparator);

		Interface interface;
		for (int i = 0; i < vec.size(); ++i)
		{
			interface.insert({ vec[i].first, vec[i].second });
		}
		return interface;
	}

}
