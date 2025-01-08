#include "acpch.h"
/*!***************************************************************************
	\file			Engine.cpp
	\author			Chua Yip Xuan
	\par			yipxuan.chua\@digipen.edu 
	\brief			This source file consists of function definitions of a 
					Scene Manager, where the registering of components and 
					systems happens here. As well as the init, update and 
					destroy.
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include "Core/Engine.h"
#include "ECS/Architecture/Coordinator.hpp"
#include "ECS/Components/Behaviour.h"
#include "ECS/Components/Transform.h"
#include "ECS/Components/Collider.h"
#include "ECS/Components/Audio.h"
#include "ECS/Components/AudioListener.h"
#include "ECS/Components/RigidBody.h"
#include "ECS/Components/Render.h"
#include "ECS/Components/Camera.h"
#include "ECS/Components/UI.h"
#include "ECS/Components/Particle.h"

#include "ECS/Systems/PhysicsSystem.h"
#include "ECS/Systems/CollisionSystem.h"
#include "ECS/Systems/ScriptingSystem.h"
#include "ECS/Systems/AudioSystem.h"
#include "ECS/Systems/RenderSystem.h"
#include "ECS/Systems/InputSystem.h"
#include "ECS/Systems/CameraSystem.h"
#include "ECS/Systems/Renderer2DSystem.h"


//Temp:
#include "ECS/Systems/ParticleSystem.h"
#include "Physics/ACPhysicsInternal.hpp"
#include "Rendering/Rendering.h"
#include "Serialization/RegisterManager.hpp"

//#include <cmp_core.h>

namespace AeonCore
{
	//void ShowResults(const char* testcodec, unsigned char src_RGBA[], unsigned char decomp_RGBA[]) {
	//	std::printf("\n[%s]\n", testcodec);
	//	// show the first row of pixels
	//	std::printf("original   : (%3d,%3d,%3d,%3d) (%3d,%3d,%3d,%3d) (%3d,%3d,%3d,%3d) (%3d,%3d,%3d,%3d)\n",
	//		src_RGBA[0], src_RGBA[1], src_RGBA[2], src_RGBA[3],
	//		src_RGBA[4], src_RGBA[5], src_RGBA[6], src_RGBA[7],
	//		src_RGBA[8], src_RGBA[9], src_RGBA[10], src_RGBA[11],
	//		src_RGBA[12], src_RGBA[13], src_RGBA[14], src_RGBA[15]);

	//	std::printf("decompress : (%3d,%3d,%3d,%3d) (%3d,%3d,%3d,%3d) (%3d,%3d,%3d,%3d) (%3d,%3d,%3d,%3d)\n",
	//		decomp_RGBA[0], decomp_RGBA[1], decomp_RGBA[2], decomp_RGBA[3],
	//		decomp_RGBA[4], decomp_RGBA[5], decomp_RGBA[6], decomp_RGBA[7],
	//		decomp_RGBA[8], decomp_RGBA[9], decomp_RGBA[10], decomp_RGBA[11],
	//		decomp_RGBA[12], decomp_RGBA[13], decomp_RGBA[14], decomp_RGBA[15]);

	//	// Calculate a sum of image diffs:to see how well the codec compressed
	//	int diffSum = 0;
	//	for (int i = 0; i < 64; i++) {
	//		diffSum += abs(src_RGBA[i] - decomp_RGBA[i]);
	//	}
	//	std::printf("Image Diff Sum = %d\n", diffSum);
	//}

	void Engine::Init()
	{
		Coordinator& gCoordinator = Coordinator::GetInstance(); // this is how you get the instance of coordinator 
		gCoordinator.Init();									//init coordinator should only be here

		//	REGISTER COMPONENT HERE
		{
			RegisterManager::Instance().RegisterComponents();

			gCoordinator.RegisterComponent<Transform>();
			gCoordinator.RegisterComponent<Render>();
			gCoordinator.RegisterComponent<Camera>();
			gCoordinator.RegisterComponent<Behaviour>();
			gCoordinator.RegisterComponent<AudioComponent>();
			gCoordinator.RegisterComponent<AudioListenerComponent>();
			gCoordinator.RegisterComponent<Collider>();
			gCoordinator.RegisterComponent<RigidBody>();
			gCoordinator.RegisterComponent<Font>();
			gCoordinator.RegisterComponent<UI>();
			gCoordinator.RegisterComponent<Lighting>();
			gCoordinator.RegisterComponent<Particles>();
		}

		//	REGISTER SYSTEM HERE
		{
			gCoordinator.RegisterSystem<PhysicsSystem>();
			{
				Signature signature;
				signature.set(gCoordinator.GetComponentType<Transform>());
				signature.set(gCoordinator.GetComponentType<Collider>());
				signature.set(gCoordinator.GetComponentType<RigidBody>());
				gCoordinator.SetSystemSignature<PhysicsSystem>(signature);
			}

			gCoordinator.RegisterSystem<AudioSystem>();
			{
				Signature signature;
				signature.set(gCoordinator.GetComponentType<AudioComponent>());
				//signature.set(gCoordinator.GetComponentType<AudioListenerComponent>());
				//signature.set(gCoordinator.GetComponentType<Transform>());
				gCoordinator.SetSystemSignature<AudioSystem>(signature);
			}

			gCoordinator.RegisterSystem<ScriptingSystem>();
			{
				Signature scriptSign;
				scriptSign.set(gCoordinator.GetComponentType<Behaviour>());
				gCoordinator.SetSystemSignature<ScriptingSystem>(scriptSign);
			}

			gCoordinator.RegisterSystem<RenderSystem>();
			{
				Signature signature;
				signature.set(gCoordinator.GetComponentType<Lighting>());
				signature.set(gCoordinator.GetComponentType<Transform>());
				gCoordinator.SetSystemSignature<RenderSystem>(signature);
			}

			gCoordinator.RegisterSystem<InputSystem>();

			gCoordinator.RegisterSystem<CameraSystem>();
			{
				Signature signature;
				signature.set(gCoordinator.GetComponentType<Camera>());
				signature.set(gCoordinator.GetComponentType<Transform>());
				gCoordinator.SetSystemSignature<CameraSystem>(signature);
			}

			gCoordinator.RegisterSystem<Renderer2DSystem>();
			{
				Signature signature;
				signature.set(gCoordinator.GetComponentType<UI>());
				//signature.set(gCoordinator.GetComponentType<Transform>());
				gCoordinator.SetSystemSignature<Renderer2DSystem>(signature);
			}

			gCoordinator.RegisterSystem<ParticleSystem>();
			{
				Signature signature;
				signature.set(gCoordinator.GetComponentType<Particles>());
				signature.set(gCoordinator.GetComponentType<Transform>());
				gCoordinator.SetSystemSignature<ParticleSystem>(signature);
			}

			gCoordinator.RegisterSystem<LightManager>();
			{
				Signature signature;
				signature.set(gCoordinator.GetComponentType<Lighting>());
				signature.set(gCoordinator.GetComponentType<Transform>());
				gCoordinator.SetSystemSignature<LightManager>(signature);
			}
		}

		gCoordinator.GetSystem<PhysicsSystem>()->Init();				//then init your system
		gCoordinator.GetSystem<AudioSystem>()->Init();
		gCoordinator.GetSystem<ScriptingSystem>()->Init();
		gCoordinator.GetSystem<RenderSystem>()->Init();
		gCoordinator.GetSystem<InputSystem>()->Init();
		gCoordinator.GetSystem<CameraSystem>()->Init();
		gCoordinator.GetSystem<Renderer2DSystem>()->Init();
		gCoordinator.GetSystem<ParticleSystem>()->Init();
		gCoordinator.GetSystem<LightManager>()->Init();

		// // Test shape0 for  BC1, BC2, BC3, BC7
		// unsigned char shape0_RGBA[64] = {
		// 	// GREEN          GREEN        RED             RED           Pixels
		// 		0,255, 0,255,  0,255, 0,255,  255, 0, 0,255,  255, 0, 0,255,
		// 		0,255, 0,255,  0,255, 0,255,  255, 0, 0,255,  255, 0, 0,255,
		// 		0,255, 0,255,  0,255, 0,255,  255, 0, 0,255,  255, 0, 0,255,
		// 		0,255, 0,255,  0,255, 0,255,  255, 0, 0,255,  255, 0, 0,255
		// };
		// 
		// unsigned char  cmpBuffer16[16] = { 0 };  // Compression buffer for BC5,BC6 and BC7
		// unsigned char  imgBuffer[64] = { 0 };  // Results buffer for decompressed  shape0_RGBA
		// CompressBlockBC2(shape0_RGBA, 16, cmpBuffer16, NULL);
		// DecompressBlockBC2(cmpBuffer16, imgBuffer, NULL);
		// ShowResults("BC2", shape0_RGBA, imgBuffer);
	}

	void Engine::Update()
 	{
		

		Coordinator& gCoordinator = Coordinator::GetInstance();
		gCoordinator.GetSystem<InputSystem>()->Update();
		gCoordinator.GetSystem<PhysicsSystem>()->Update(); 
		gCoordinator.GetSystem<CameraSystem>()->Update();
		gCoordinator.GetSystem<ScriptingSystem>()->Update(); 
		gCoordinator.GetSystem<AudioSystem>()->Update();
		gCoordinator.GetSystem<Renderer2DSystem>()->Update();
		gCoordinator.GetSystem<ParticleSystem>()->Update();
		gCoordinator.GetSystem<LightManager>()->Update();
		gCoordinator.GetSystem<RenderSystem>()->Update();
		//Entity SplashScreen = *gCoordinator.GetEntityByName("Splash Screen");

		/*if (AeonCore::Input::IsKeyTriggered(AC_KEY_T))
		{
			Entity* ent = gCoordinator.GetEntityByName("Splash Screen");
			if (ent != nullptr)
			{
				gCoordinator.DestroyEntity(*ent);

			}
		}*/
	}
	void Engine::Destroy()
	{
		Coordinator& gCoordinator = Coordinator::GetInstance();
		gCoordinator.Destroy();

		gCoordinator.GetSystem<PhysicsSystem>()->Destroy(); 
		gCoordinator.GetSystem<ScriptingSystem>()->Destroy(); 
		gCoordinator.GetSystem<AudioSystem>()->Destroy(); 
		gCoordinator.GetSystem<Renderer2DSystem>()->Destroy();
		gCoordinator.GetSystem<RenderSystem>()->Destroy();
		gCoordinator.GetSystem<InputSystem>()->Destroy();
		gCoordinator.GetSystem<CameraSystem>()->Destroy();
		gCoordinator.GetSystem<ParticleSystem>()->Destroy();
		gCoordinator.GetSystem<LightManager>()->Destroy();
	}

}
