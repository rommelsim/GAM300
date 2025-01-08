#include "acpch.h"
#include "ECS/Systems/ParticleSystem.h"

#include "Core/Application.h"
#include "Core/TimeStep.h"

namespace AeonCore
{
	void ParticleSystem::Init()
	{
	}

	void ParticleSystem::Update()
	{
		ZoneScopedN("Particle Update");

		/*if (Application::GetApplication().GetLoadStatus() != LoadStatus::NotRunning)
			return;*/

		float dt = TimerStep::GetInstance().get_delta_time();

		if (Application::GetApplication().IsPaused()) return;

		Coordinator& gCoordinator = Coordinator::GetInstance();
		for (auto entityId : mEntities)
		{

			if (!gCoordinator.HasComponent<Transform>(entityId))
				continue;

			Transform& trans = gCoordinator.GetComponent<Transform>(entityId);

			if (!gCoordinator.HasComponent<Particles>(entityId))
				continue;

			Particles& particles = gCoordinator.GetComponent<Particles>(entityId);

			particles.SetParticleScale(trans.GetScale());
			particles.Update(dt, trans);
		}
	}

	void ParticleSystem::Destroy()
	{
	}
}
